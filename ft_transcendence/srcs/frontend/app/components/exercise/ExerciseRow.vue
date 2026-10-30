<script setup lang="ts">
import type { QuestionSummary } from '~/types'

const { question, path } = defineProps<{
  question: QuestionSummary
  path?: string
}>()

const difficulty = computed(() => difficultyOf(question.difficulty_rating))
const tagType = computed(() => DIFFICULTY_TAG[difficulty.value])
</script>

<template>
  <UiCard pad interactive class="h-full">
    <div class="flex h-full items-center gap-3 sm:gap-3.5">
      <span
        aria-hidden="true"
        class="hidden size-8.5 flex-none sm:flex items-center justify-center rounded-md bg-surface-raised text-fg-muted"
      >
        <Icon name="lucide:message-circle-question-mark" mode="svg" size="1.125em" />
      </span>

      <div class="min-w-0 flex-1">
        <p
          class="mb-1.5 truncate font-semibold"
          :title="question.title ?? undefined"
          :lang="question.is_fallback ? 'en' : undefined"
        >
          {{ question.title }}
        </p>
        <p v-if="path" class="mb-2 text-xs text-fg-subtle">{{ path }}</p>
        <ExerciseTags :tags="question.tags" />
      </div>

      <UiTag :type="tagType" :label="$t(`exercise.difficulty.${difficulty}`)" class="flex-none" />
      <Icon
        name="lucide:chevron-right"
        mode="svg"
        size="1em"
        aria-hidden="true"
        class="flex-none text-fg-subtle"
      />
    </div>
  </UiCard>
</template>
