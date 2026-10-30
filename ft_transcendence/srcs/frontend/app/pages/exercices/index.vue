<script setup lang="ts">
definePageMeta({ layout: 'dashboard', middleware: 'auth' })

const { exams, loading, failed, load } = useQuestions()
</script>

<template>
  <div class="mx-auto w-full max-w-260">
    <h1 class="mb-1.5 text-2xl sm:text-3xl">{{ $t('exercices.title') }}</h1>
    <p class="mb-5 text-fg-muted">{{ $t('exercices.subtitle') }}</p>

    <UiCard v-if="loading" pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('exercices.loading') }}</p>
    </UiCard>

    <div v-else-if="failed" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('exercices.loadFailed') }}
      </UiCallout>
      <UiButton variant="secondary" size="sm" :label="$t('exercices.retry')" @click="load" />
    </div>

    <ExerciseExamCarousel v-else-if="exams.length" :exams="exams" />

    <UiCard v-else pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('exercices.empty') }}</p>
    </UiCard>
  </div>
</template>
