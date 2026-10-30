<script setup lang="ts">
import type { AttemptQuota, FallbackReason, GraderEvent, GraderOutcome } from '~/types'
import { GraderError } from '~/composables/useGrader'

interface Exchange {
  answer: string
  verdict: { outcome: GraderOutcome; flagged: boolean } | null
  text: string
  streaming: boolean
  streamFailed: boolean
  reference: string | null
  fallback: { reason: FallbackReason; attemptId: number } | null
  selfAssessed: boolean
  selfFailed: boolean
  error: string | null
}

type Tone = 'success' | 'warning' | 'danger'

const { questionId } = defineProps<{ questionId: number }>()
const answer = defineModel<string>({ default: '' })
const emit = defineEmits<{ result: [accepted: boolean] }>()

const { t } = useI18n()
const { api } = useApi()
const { submit, selfAssess } = useGrader()
const { user } = useAuth()
const { announce } = useAchievements()
const id = useId()

const PILL: Record<Tone, string> = {
  success: 'bg-success-soft text-success',
  warning: 'bg-warning-soft text-warning',
  danger: 'bg-danger-soft text-danger',
}

const PILL_ICON: Record<Tone, string> = {
  success: 'lucide:circle-check-big',
  warning: 'lucide:circle-alert',
  danger: 'lucide:circle-x',
}

const VERDICT_TONE: Record<GraderOutcome, Tone> = {
  correct: 'success',
  imprecise: 'warning',
  off_target: 'danger',
}

const DOT: Record<'used' | 'left', string> = {
  used: 'bg-accent',
  left: 'bg-line-strong',
}

const thread = ref<Exchange[]>([])
const busy = ref(false)
const closed = ref(false)
const attemptsLeft = ref(0)
const limit = ref(0)

const current = computed(() => thread.value.at(-1) ?? null)

const announcement = computed(() =>
  current.value?.verdict ? t(`grader.verdict.${current.value.verdict.outcome}`) : '',
)

const disabled = computed(
  () =>
    busy.value ||
    closed.value ||
    current.value?.fallback != null ||
    answer.value.trim().length === 0,
)

const attemptsKnown = computed(() => limit.value > 0)

const attemptsUsed = computed(() => limit.value - attemptsLeft.value)

const attemptsHint = computed(() => (closed.value ? t('grader.noAttemptsLeft') : ''))

function toneOf(exchange: Exchange): Tone {
  if (!exchange.verdict) return 'danger'
  return exchange.verdict.flagged ? 'danger' : VERDICT_TONE[exchange.verdict.outcome]
}

function iconOf(exchange: Exchange): string {
  if (exchange.verdict?.flagged) return 'lucide:shield-alert'
  return PILL_ICON[toneOf(exchange)]
}

function handle(event: GraderEvent) {
  const last = current.value
  if (!last) return
  switch (event.type) {
    case 'verdict':
      last.verdict = { outcome: event.outcome, flagged: event.flagged }
      attemptsLeft.value = event.attempts_left
      last.streaming = !event.flagged && event.outcome !== 'correct'
      emit('result', !event.flagged && event.outcome !== 'off_target')
      announce(event.achievements)
      break
    case 'delta':
      last.text += event.text
      break
    case 'error':
      last.streamFailed = true
      break
    case 'done':
      last.streaming = false
      closed.value = event.closed
      last.reference = event.explanation
      break
    case 'self_assessment':
      last.fallback = { reason: event.reason, attemptId: event.attempt_id }
      attemptsLeft.value = event.attempts_left
      last.reference = event.explanation
      emit('result', false)
      break
  }
}

function describe(err: unknown): string {
  if (!(err instanceof GraderError)) return t('grader.errors.generic')
  if (err.status === 429 && err.detail === 'attempts exhausted') {
    closed.value = true
    return t('grader.errors.exhausted')
  }
  if (err.status === 429) return t('grader.errors.tooFast', { s: err.retryAfter ?? 20 })
  if (err.status === 404) return t('grader.errors.notFound')
  return t('grader.errors.generic')
}

async function send() {
  if (disabled.value) return
  busy.value = true
  const newExchange: Exchange = {
    answer: answer.value.trim(),
    verdict: null,
    text: '',
    streaming: true,
    streamFailed: false,
    reference: null,
    fallback: null,
    selfAssessed: false,
    selfFailed: false,
    error: null,
  }
  thread.value.push(newExchange)
  answer.value = ''
  try {
    await submit(questionId, newExchange.answer, handle)
  } catch (err) {
    newExchange.error = describe(err)
  } finally {
    busy.value = false
    newExchange.streaming = false
  }
}

watch(
  () => questionId,
  async (id) => {
    thread.value = []
    closed.value = false
    limit.value = 0
    try {
      const quota = await api<AttemptQuota>(`/api/v1/questions/${id}/attempts`)
      limit.value = quota.limit
      attemptsLeft.value = quota.left
      closed.value = quota.left === 0
    } catch {
      limit.value = 0
    }
  },
  { immediate: true },
)

async function assess(correct: boolean) {
  const last = current.value
  if (!last || !last.fallback) return
  last.selfFailed = false
  try {
    announce(await selfAssess(last.fallback.attemptId, correct))
    last.selfAssessed = true
    emit('result', correct)
  } catch {
    last.selfFailed = true
  }
}
</script>

<template>
  <div>
    <div v-if="attemptsKnown" class="mb-3 flex items-center gap-2 text-xs text-fg-subtle">
      <span v-if="attemptsKnown" aria-hidden="true" class="flex items-center gap-1">
        <span
          v-for="n in limit"
          :key="n"
          class="block size-1.5 rounded-full"
          :class="DOT[n <= attemptsUsed ? 'used' : 'left']"
        />
      </span>
      <span>
        {{ attemptsLeft > 0 ? $t('grader.attemptsLeft', { n: attemptsLeft }, attemptsLeft) : '' }}
      </span>
    </div>

    <div
      v-if="thread.length"
      class="mb-5 flex flex-col gap-3.5"
      :aria-label="$t('grader.thread')"
      role="log"
    >
      <template v-for="(exchange, index) in thread" :key="index">
        <div class="flex justify-end gap-2.5">
          <p
            class="max-w-[85%] rounded-lg rounded-tr-xs bg-accent-2-soft px-3.5 py-2.5 text-sm break-words whitespace-pre-wrap sm:max-w-[78%]"
          >
            <UiCodeText :text="exchange.answer" />
          </p>
          <UiAvatar :name="user?.display_name ?? ''" :src="user?.avatar_path ?? null" :size="30" />
        </div>

        <div class="flex gap-2.5">
          <span
            aria-hidden="true"
            class="flex size-7.5 flex-none items-center justify-center rounded-full bg-accent-soft text-accent"
          >
            <Icon name="lucide:sparkles" mode="svg" size="1em" />
          </span>
          <div class="flex min-w-0 flex-col gap-2">
            <div
              v-if="exchange.verdict?.flagged || exchange.text || exchange.streaming"
              class="w-fit max-w-full rounded-lg rounded-tl-xs bg-accent-soft px-4 py-3 break-words sm:max-w-[78%]"
            >
              <p class="mb-1 text-xs font-semibold text-accent">Maïa</p>
              <template v-if="exchange.verdict?.flagged">
                <p class="text-sm">{{ $t('grader.flagged') }}</p>
              </template>
              <template v-else-if="exchange.text || exchange.streaming">
                <p
                  v-if="exchange.text"
                  :aria-busy="exchange.streaming"
                  class="text-sm whitespace-pre-wrap"
                >
                  <UiCodeText :text="exchange.text" />
                </p>
                <p v-else class="text-sm">{{ $t('grader.grading') }}</p>
              </template>
              <p v-if="exchange.streamFailed" class="mt-2 text-sm text-danger" role="alert">
                {{ $t('grader.explanationFailed') }}
              </p>
            </div>

            <div
              v-if="exchange.reference"
              class="w-fit max-w-full rounded-lg rounded-tl-xs bg-accent-soft px-4 py-3 break-words sm:max-w-[78%]"
            >
              <p class="mb-1 text-xs font-semibold text-fg-muted">
                {{ $t('grader.referenceTitle') }}
              </p>
              <p class="text-sm whitespace-pre-wrap">
                <UiCodeText :text="exchange.reference" />
              </p>
            </div>

            <div
              v-if="exchange.fallback"
              class="w-fit max-w-full rounded-lg rounded-tl-xs bg-accent-soft px-4 py-3 break-words sm:max-w-[78%]"
            >
              <p class="text-sm">{{ $t(`grader.fallback.${exchange.fallback.reason}`) }}</p>
              <p class="text-sm">{{ $t('grader.fallback.prompt') }}</p>
            </div>

            <p
              v-if="exchange.verdict"
              class="flex items-center gap-2 self-start rounded-full px-3 py-1.5 text-sm font-semibold"
              :class="PILL[toneOf(exchange)]"
            >
              <Icon :name="iconOf(exchange)" mode="svg" size="1em" aria-hidden="true" />
              {{ $t(`grader.verdict.${exchange.verdict.outcome}`) }}
            </p>

            <template v-if="exchange.fallback && !exchange.selfAssessed">
              <div class="flex flex-wrap gap-3">
                <UiButton
                  variant="secondary"
                  size="sm"
                  :label="$t('grader.selfWrong')"
                  @click="assess(false)"
                />
                <UiButton size="sm" :label="$t('grader.selfCorrect')" @click="assess(true)" />
              </div>
            </template>
            <p v-if="exchange.fallback && exchange.selfAssessed" class="text-sm text-fg-muted">
              {{ $t('grader.selfThanks') }}
            </p>
            <p
              v-if="exchange.fallback && exchange.selfFailed"
              class="flex items-center gap-2 self-start rounded-full bg-danger-soft px-3 py-1.5 text-sm font-semibold text-danger"
              role="alert"
            >
              <Icon name="lucide:triangle-alert" mode="svg" size="1em" aria-hidden="true" />
              {{ $t('grader.errors.selfFailed') }}
            </p>

            <p
              v-if="exchange.error"
              class="flex items-center gap-2 self-start rounded-full bg-danger-soft px-3 py-1.5 text-sm font-semibold text-danger"
              role="alert"
            >
              <Icon name="lucide:triangle-alert" mode="svg" size="1em" aria-hidden="true" />
              {{ exchange.error }}
            </p>
          </div>
        </div>
      </template>
    </div>

    <p class="sr-only" aria-live="polite">{{ announcement }}</p>

    <div class="rounded-lg border border-line-subtle bg-surface-card p-3.5">
      <label :for="id" class="sr-only">{{ $t('exercise.answerLabel') }}</label>
      <UiTextarea
        :id="id"
        v-model="answer"
        rows="4"
        maxlength="2000"
        :placeholder="$t('exercise.answerPlaceholder')"
        :disabled="closed"
      />
      <div class="mt-3 flex flex-wrap items-center justify-between gap-3">
        <p v-if="attemptsHint" class="text-xs text-fg-subtle">{{ attemptsHint }}</p>
        <span v-else />
        <UiButton
          :label="busy ? $t('grader.grading') : $t('grader.submit')"
          :disabled="disabled"
          @click="send"
        />
      </div>
    </div>
  </div>
</template>
