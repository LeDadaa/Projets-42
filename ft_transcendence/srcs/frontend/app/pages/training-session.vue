<script setup lang="ts">
import { formatClock } from '~/composables/useTrainingSession'

definePageMeta({ layout: 'dashboard', middleware: 'auth' })

const { questions, loading, failed, load, questionsOf, topicLabel } = useQuestions()
const { session, remaining, elapsed, ended, unlimited, timedOut, start, finish, reset, record } =
  useTrainingSession()

const byId = computed(() => new Map(questions.value.map((q) => [q.id, q])))

const sessionQuestions = computed(() => {
  if (!session.value) return []
  return session.value.questionIds.flatMap((id) => {
    const question = byId.value.get(id)
    return question ? [question] : []
  })
})

const index = computed(() =>
  Math.min(session.value?.index ?? 0, Math.max(0, sessionQuestions.value.length - 1)),
)
const current = computed(() => sessionQuestions.value[index.value])
const difficulty = computed(() =>
  current.value ? difficultyOf(current.value.difficulty_rating) : 'easy',
)
const tagType = computed(() => DIFFICULTY_TAG[difficulty.value])

const answer = computed({
  get: () =>
    current.value && session.value ? (session.value.answers[current.value.id] ?? '') : '',
  set: (value: string) => {
    if (current.value && session.value) {
      session.value.answers[current.value.id] = value
    }
  },
})

const inSetup = computed(() => !session.value || !sessionQuestions.value.length)

const clock = computed(() =>
  formatClock(
    unlimited.value ? Math.floor(elapsed.value / 1000) : Math.ceil(remaining.value / 1000),
  ),
)

function goTo(position: number) {
  if (session.value && sessionQuestions.value[position]) {
    session.value.index = position
  }
}

function begin(topicId: number, count: number, minutes: number | null) {
  start(
    topicId,
    questionsOf(topicId).map((q) => q.id),
    count,
    minutes,
  )
}
</script>

<template>
  <div class="mx-auto w-full" :class="inSetup ? 'max-w-260' : 'max-w-200'">
    <h1 class="mb-5 text-2xl sm:text-3xl">{{ $t('nav.trainingSession') }}</h1>

    <UiCard v-if="loading" pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('training.loading') }}</p>
    </UiCard>

    <div v-else-if="failed" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('training.loadFailed') }}
      </UiCallout>
      <UiButton variant="secondary" size="sm" :label="$t('training.retry')" @click="load" />
    </div>

    <UiCard v-else-if="!questions.length" pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('training.empty') }}</p>
    </UiCard>

    <TrainingSetup v-else-if="!session || !sessionQuestions.length" @start="begin" />

    <TrainingRecap
      v-else-if="ended"
      :questions="sessionQuestions"
      :results="session.results"
      :timed-out="timedOut"
      @restart="reset"
    />

    <template v-else-if="current">
      <div class="mb-4">
        <div class="mb-2 flex flex-wrap items-baseline justify-between gap-3">
          <p class="text-sm text-fg-muted">{{ topicLabel(current.topic_id) }}</p>
          <div class="flex flex-wrap items-center gap-3">
            <p class="font-mono text-xs text-fg-subtle">
              {{ $t('training.progress', { n: index + 1, total: sessionQuestions.length }) }}
            </p>
            <p role="timer" class="flex items-center gap-1.5 font-mono text-sm">
              <Icon name="lucide:timer" mode="svg" size="1em" aria-hidden="true" />
              <span class="sr-only">{{
                unlimited ? $t('training.elapsed') : $t('training.remaining')
              }}</span>
              {{ clock }}
            </p>
            <UiButton variant="ghost" size="sm" :label="$t('training.finish')" @click="finish" />
          </div>
        </div>
        <UiProgress :value="index + 1" :max="sessionQuestions.length" />
      </div>

      <UiCard pad>
        <div class="mb-3 flex flex-wrap items-center gap-2">
          <UiTag :type="tagType" :label="$t(`exercise.difficulty.${difficulty}`)" />
          <UiTag v-if="current.is_fallback" type="neutral" :label="$t('exercise.notTranslated')" />
        </div>

        <h2
          class="mb-4 text-lg leading-snug font-semibold break-words sm:text-xl"
          :lang="current.is_fallback ? 'en' : undefined"
        >
          <UiCodeText :text="current.stem ?? ''" />
        </h2>

        <ExerciseTags :tags="current.tags" />
      </UiCard>

      <ExerciseGrader
        :key="current.id"
        v-model="answer"
        :question-id="current.id"
        class="mt-5"
        @result="record(current.id, $event)"
      />

      <div class="mt-5 flex flex-wrap items-center gap-3">
        <UiButton
          variant="secondary"
          :label="$t('training.previous')"
          :disabled="index === 0"
          @click="goTo(index - 1)"
        >
          <Icon name="lucide:arrow-left" mode="svg" size="1em" />
        </UiButton>
        <UiButton
          variant="secondary"
          :label="$t('training.next')"
          :disabled="index >= sessionQuestions.length - 1"
          class="sm:ml-auto"
          @click="goTo(index + 1)"
        >
          <template #trailing>
            <Icon name="lucide:arrow-right" mode="svg" size="1em" />
          </template>
        </UiButton>
      </div>
    </template>
  </div>
</template>
