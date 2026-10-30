<script setup lang="ts">
definePageMeta({ layout: 'dashboard', middleware: 'auth' })

const route = useRoute()
const { t } = useI18n()
const { loading, failed, load, topicBySlug, childrenOf, questionsOf, questionCount, lineage } =
  useQuestions()
const { notionLabel } = useNotions()

const topic = computed(() => topicBySlug(String(route.params.topic)))
const children = computed(() => (topic.value ? childrenOf(topic.value.id) : []))
const exercises = computed(() => (topic.value ? questionsOf(topic.value.id) : []))

const query = ref('')
const needle = computed(() => foldText(query.value.trim()))

watch(
  () => route.params.topic,
  () => {
    query.value = ''
  },
)

const examQuestions = computed(() => children.value.flatMap((level) => questionsOf(level.id)))

const results = computed(() =>
  examQuestions.value.filter(
    (question) =>
      foldText(question.title ?? '').includes(needle.value) ||
      question.tags.some((tag) => foldText(notionLabel(tag)).includes(needle.value)),
  ),
)

const examples = computed(() => {
  const counts = new Map<string, number>()
  for (const question of examQuestions.value)
    for (const tag of question.tags) counts.set(tag, (counts.get(tag) ?? 0) + 1)
  return [...counts.entries()]
    .sort((a, b) => b[1] - a[1])
    .slice(0, 2)
    .map(([tag]) => notionLabel(tag))
    .join(', ')
})

const breadcrumb = computed(() => [
  { label: t('nav.exercices'), to: '/exercices' },
  ...(topic.value ? lineage(topic.value.id) : []).map((node) => ({
    label: node.name ?? node.slug,
    to: node.id === topic.value?.id ? undefined : `/exercices/${node.slug}`,
  })),
])

function levelDetail(levelId: number) {
  return t('exercices.exercisesCount', questionCount(levelId))
}

function levelName(topicId: number) {
  return lineage(topicId).at(-1)?.name ?? ''
}
</script>

<template>
  <div class="mx-auto w-full max-w-260">
    <UiCard v-if="loading" pad>
      <p class="text-center text-sm text-fg-muted">{{ $t('exercices.loading') }}</p>
    </UiCard>

    <div v-else-if="failed" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('exercices.loadFailed') }}
      </UiCallout>
      <UiButton variant="secondary" size="sm" :label="$t('exercices.retry')" @click="load" />
    </div>

    <div v-else-if="!topic" class="flex flex-col items-start gap-3">
      <UiCallout tone="danger" icon="lucide:triangle-alert" class="w-full">
        {{ $t('exercices.topicNotFound') }}
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
      <div
        class="flex flex-wrap items-center justify-between gap-3"
        :class="topic.description ? 'mb-1.5' : 'mb-5'"
      >
        <h1 class="text-2xl sm:text-3xl">{{ topic.name ?? topic.slug }}</h1>
        <UiButton
          v-if="children.length"
          variant="secondary"
          size="sm"
          to="/exercices"
          :label="$t('exercices.backToExams')"
        >
          <Icon name="lucide:arrow-left" mode="svg" size="1em" />
        </UiButton>
      </div>
      <p v-if="topic.description" class="mb-5 text-fg-muted">{{ topic.description }}</p>

      <div v-if="children.length" class="mb-5 max-w-120">
        <label for="topic-search" class="sr-only">{{ $t('exercices.searchLabel') }}</label>
        <UiInput
          id="topic-search"
          v-model="query"
          type="search"
          icon="lucide:search"
          :placeholder="$t('exercices.searchPlaceholder', { examples })"
        />
      </div>

      <template v-if="children.length && needle">
        <p class="mb-3 text-xs text-fg-subtle" aria-live="polite">
          {{ $t('exercices.resultsCount', results.length) }}
        </p>
        <ul v-if="results.length" class="grid gap-4 xl:grid-cols-2">
          <li v-for="question in results" :key="question.id">
            <NuxtLink
              :to="`/exercice-${question.slug}`"
              class="block h-full rounded-lg focus-visible:shadow-focus focus-visible:outline-none"
            >
              <ExerciseRow :question="question" :path="levelName(question.topic_id)" />
            </NuxtLink>
          </li>
        </ul>
        <UiCard v-else pad>
          <p class="text-center text-sm text-fg-muted">{{ $t('exercices.noResults') }}</p>
        </UiCard>
      </template>

      <ul v-else-if="children.length" class="grid gap-4 md:grid-cols-2">
        <li v-for="child in children" :key="child.id">
          <NuxtLink
            :to="`/exercices/${child.slug}`"
            class="block h-full rounded-lg focus-visible:shadow-focus focus-visible:outline-none"
          >
            <ExerciseTopicCard
              :name="child.name ?? child.slug"
              :description="child.description"
              :detail="levelDetail(child.id)"
              icon="lucide:list-checks"
            />
          </NuxtLink>
        </li>
      </ul>

      <ul v-else-if="exercises.length" class="grid gap-4 xl:grid-cols-2">
        <li v-for="question in exercises" :key="question.id">
          <NuxtLink
            :to="`/exercice-${question.slug}`"
            class="block h-full rounded-lg focus-visible:shadow-focus focus-visible:outline-none"
          >
            <ExerciseRow :question="question" />
          </NuxtLink>
        </li>
      </ul>

      <UiCard v-else pad>
        <p class="text-center text-sm text-fg-muted">{{ $t('exercices.emptyTopic') }}</p>
      </UiCard>
    </template>
  </div>
</template>
