<script setup lang="ts">
import type { QuestionSummary } from '~/types'

type Status = 'validated' | 'toReview' | 'unanswered'

const props = defineProps<{
  questions: QuestionSummary[]
  results: Record<number, boolean>
  timedOut: boolean
}>()

const emit = defineEmits<{ restart: [] }>()

const heading = ref<HTMLElement | null>(null)

const STATUS_TAG: Record<Status, 'success' | 'warning' | 'neutral'> = {
  validated: 'success',
  toReview: 'warning',
  unanswered: 'neutral',
}

const questionsWithStatus = computed(() =>
  props.questions.map((q) => {
    let status: Status
    if (props.results[q.id] === true) {
      status = 'validated'
    } else if (q.id in props.results) {
      status = 'toReview'
    } else {
      status = 'unanswered'
    }
    return { question: q, status }
  }),
)

const counts = computed(() => ({
  validated: questionsWithStatus.value.filter((x) => x.status === 'validated').length,
  toReview: questionsWithStatus.value.filter((x) => x.status === 'toReview').length,
  unanswered: questionsWithStatus.value.filter((x) => x.status === 'unanswered').length,
}))

onMounted(() => {
  heading.value?.focus()
})
</script>

<template>
  <UiCard pad>
    <h2 ref="heading" tabindex="-1" class="mb-3 text-xl font-semibold focus-visible:outline-none">
      {{ $t('training.recap.title') }}
    </h2>

    <UiCallout :tone="timedOut ? 'hint' : 'info'" icon="lucide:clock" class="mb-5">
      {{ timedOut ? $t('training.recap.timeUp') : $t('training.recap.finished') }}
    </UiCallout>

    <div class="mb-5 grid grid-cols-3 gap-3 sm:gap-4">
      <UiStat :value="counts.validated" :label="$t('training.recap.validated')" />
      <UiStat :value="counts.toReview" :label="$t('training.recap.toReview')" />
      <UiStat :value="counts.unanswered" :label="$t('training.recap.unanswered')" />
    </div>

    <ul class="mb-5 flex flex-col gap-2">
      <li
        v-for="item in questionsWithStatus"
        :key="item.question.id"
        class="flex flex-wrap items-center justify-between gap-3"
      >
        <NuxtLink
          :to="`/exercice-${item.question.slug}`"
          class="min-w-0 text-sm break-words hover:text-accent focus-visible:shadow-focus focus-visible:outline-none rounded-sm"
        >
          {{ item.question.title ?? item.question.slug }}
        </NuxtLink>
        <UiTag :type="STATUS_TAG[item.status]" :label="$t(`training.recap.${item.status}`)" />
      </li>
    </ul>

    <div class="flex justify-end">
      <UiButton :label="$t('training.recap.restart')" @click="emit('restart')" />
    </div>
  </UiCard>
</template>
