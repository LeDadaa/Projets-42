<script setup lang="ts">
definePageMeta({ layout: 'dashboard', middleware: 'auth' })

const route = useRoute()
const { t } = useI18n()
const { questions, loading, failed, load, lineage } = useQuestions()

const answer = ref('')

const question = computed(() =>
  questions.value.find((item) => item.slug === String(route.params.slug)),
)
const difficulty = computed(() =>
  question.value ? difficultyOf(question.value.difficulty_rating) : 'easy',
)
const tagType = computed(() => DIFFICULTY_TAG[difficulty.value])

const breadcrumb = computed(() => [
  { label: t('nav.exercices'), to: '/exercices' },
  ...(question.value ? lineage(question.value.topic_id) : []).map((node) => ({
    label: node.name ?? node.slug,
    to: `/exercices/${node.slug}`,
  })),
  { label: question.value?.title ?? '' },
])
</script>

<template>
  <div class="mx-auto w-full max-w-200">
    <UiCard v-if="loading" pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('exercise.loading') }}</p>
    </UiCard>

    <div v-else-if="failed" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('exercise.loadFailed') }}
      </UiCallout>
      <UiButton variant="secondary" size="sm" :label="$t('exercise.retry')" @click="load" />
    </div>

    <div v-else-if="!question" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('exercise.notFound') }}
      </UiCallout>
      <NuxtLink
        to="/exercices"
        class="rounded-xs text-sm font-semibold text-accent hover:underline focus-visible:shadow-focus focus-visible:outline-none"
      >
        {{ $t('exercices.backToExams') }}
      </NuxtLink>
    </div>

    <template v-else>
      <UiBreadcrumb :items="breadcrumb" :label="$t('exercices.breadcrumb')" class="mb-3" />
      <h1
        class="mb-5 text-2xl break-words sm:text-3xl"
        :lang="question.is_fallback ? 'en' : undefined"
      >
        {{ question.title }}
      </h1>

      <UiCard pad>
        <div class="mb-3 flex flex-wrap items-center gap-2">
          <UiTag :type="tagType" :label="$t(`exercise.difficulty.${difficulty}`)" />
          <UiTag v-if="question.is_fallback" type="neutral" :label="$t('exercise.notTranslated')" />
        </div>

        <p
          class="mb-4 text-md leading-snug break-words sm:text-lg"
          :lang="question.is_fallback ? 'en' : undefined"
        >
          <UiCodeText :text="question.stem ?? ''" />
        </p>

        <ExerciseTags :tags="question.tags" />
      </UiCard>

      <ExerciseGrader v-model="answer" :question-id="question.id" class="mt-5" />
    </template>
  </div>
</template>
