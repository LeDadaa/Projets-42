<script setup lang="ts">
import type { QuestionSummary } from '~/types'

const { question, topic } = defineProps<{
  question: QuestionSummary
  topic: string
}>()

const difficulty = computed(() => difficultyOf(question.difficulty_rating))
const tagType = computed(() => DIFFICULTY_TAG[difficulty.value])
</script>

<template>
  <UiCard pad interactive class="h-full">
    <div class="mb-2.5 flex items-center justify-between gap-3">
      <UiTag :type="tagType" :label="$t(`exercise.difficulty.${difficulty}`)" />
      <span class="truncate font-mono text-2xs text-fg-subtle">{{ topic }}</span>
    </div>
    <p
      class="mb-3 truncate font-semibold"
      :title="question.title ?? undefined"
      :lang="question.is_fallback ? 'en' : undefined"
    >
      {{ question.title }}
    </p>
    <ExerciseTags :tags="question.tags" />
  </UiCard>
</template>
