<script setup lang="ts">
import type { AchievementOut, GamificationRules, UserStats } from '~/types/api'

definePageMeta({ layout: 'dashboard', middleware: 'auth' })

const RECOMMENDED_COUNT = 3

const { questions, topicLabel } = useQuestions()
const recommended = computed(() => questions.value.slice(0, RECOMMENDED_COUNT))

const { user, fetchMe } = useAuth()
const { api } = useApi()

const achievements = ref<AchievementOut[]>([])
const earnedCount = computed(() => achievements.value.filter((a) => a.earned_at).length)

const xpToNext = computed(() => (user.value ? user.value.xp_max - user.value.level_xp : 0))

const rules = ref<GamificationRules | null>(null)
const stats = ref<UserStats | null>(null)

onMounted(async () => {
  // `user` is cached since login: without a reload, XP won since would not show
  const [, list, fetchedRules, fetchedStats] = await Promise.all([
    fetchMe(true),
    // A failure leaves the card empty, the rest of the page still works
    api<AchievementOut[]>('/api/v1/users/me/achievements').catch(() => []),
    api<GamificationRules>('/api/v1/gamification/rules').catch(() => null),
    api<UserStats>('/api/v1/users/me/stats').catch(() => null),
  ])
  achievements.value = list
  rules.value = fetchedRules
  stats.value = fetchedStats
})
</script>

<template>
  <div v-if="user" class="mx-auto w-full max-w-260">
    <header class="mb-6 flex flex-wrap items-end justify-between gap-x-5 gap-y-3">
      <div>
        <p class="text-sm text-fg-subtle">{{ $t('dashboard.greeting') }}</p>
        <h1 class="text-2xl break-words sm:text-3xl">{{ user.display_name }} 👋</h1>
      </div>
      <UiTag
        v-if="user.streak > 0"
        type="streak"
        dot
        :label="$t('dashboard.streak', { n: user.streak })"
      />
    </header>

    <div class="mb-5 grid gap-5 lg:grid-cols-[2fr_1fr]">
      <UiCard pad>
        <div class="mb-3.5 flex flex-wrap items-baseline justify-between gap-x-4 gap-y-1">
          <p class="text-lg font-semibold">{{ $t('dashboard.level', { n: user.level }) }}</p>
          <p class="font-mono text-xs text-fg-subtle">
            {{ $t('dashboard.xp', { current: user.level_xp, max: user.xp_max }) }}
          </p>
        </div>
        <UiProgress :value="user.level_xp" :max="user.xp_max" tone="xp" size="lg" />
        <p class="mt-2.5 text-xs text-fg-subtle">
          {{ $t('dashboard.xpToNext', { n: xpToNext, level: user.level + 1 }) }}
        </p>
        <details v-if="rules" class="mt-3 text-sm">
          <summary class="cursor-pointer text-fg-subtle">{{ $t('dashboard.rules.title') }}</summary>
          <ul class="mt-2 space-y-1">
            <li>{{ $t('dashboard.rules.correctLlm', { n: rules.xp.correct_llm }) }}</li>
            <li>{{ $t('dashboard.rules.correctSelf', { n: rules.xp.correct_self }) }}</li>
            <li>{{ $t('dashboard.rules.matchPlayed', { n: rules.xp.match_played }) }}</li>
            <li>{{ $t('dashboard.rules.matchWon', { n: rules.xp.match_won }) }}</li>
            <li>{{ $t('dashboard.rules.perLevel', { n: rules.xp_per_level }) }}</li>
            <li>{{ $t('dashboard.rules.streak') }}</li>
          </ul>
        </details>
      </UiCard>

      <UiCard v-if="stats" pad>
        <div class="flex gap-5">
          <UiStat :value="stats.solved" :label="$t('dashboard.statsSolved')" />
          <UiStat :value="stats.to_review" :label="$t('dashboard.statsToReview')" />
        </div>
      </UiCard>
    </div>

    <UiCard v-if="achievements.length" pad class="mb-5">
      <div class="mb-3.5 flex flex-wrap items-baseline justify-between gap-x-4 gap-y-1">
        <h2 class="text-lg font-semibold">{{ $t('dashboard.achievements') }}</h2>
        <p class="font-mono text-xs text-fg-subtle">
          {{ $t('dashboard.achievementsCount', { n: earnedCount, total: achievements.length }) }}
        </p>
      </div>
      <ul class="grid grid-cols-2 gap-3 sm:grid-cols-3 lg:grid-cols-5">
        <li
          v-for="achievement in achievements"
          :key="achievement.code"
          class="flex flex-col items-center gap-1 rounded-lg p-3 text-center"
          :class="achievement.earned_at ? 'bg-surface-raised' : 'opacity-40 grayscale'"
        >
          <span class="text-3xl" aria-hidden="true">
            {{ ACHIEVEMENT_ICONS[achievement.code] ?? '🏅' }}
          </span>
          <span class="text-sm font-medium">{{ $t(`achievements.${achievement.code}.name`) }}</span>
          <span class="text-xs text-fg-subtle">{{ $t(`achievements.${achievement.code}.desc`) }}</span>
          <span v-if="!achievement.earned_at" class="sr-only">{{ $t('dashboard.locked') }}</span>
        </li>
      </ul>
    </UiCard>

    <template v-if="recommended.length">
      <h2 class="mb-3.5 text-xl font-semibold">{{ $t('dashboard.recommended') }}</h2>
      <ul class="grid gap-4 md:grid-cols-2 xl:grid-cols-3">
        <li v-for="question in recommended" :key="question.id">
          <NuxtLink
            :to="`/exercice-${question.slug}`"
            class="block h-full rounded-lg focus-visible:shadow-focus focus-visible:outline-none"
          >
            <ExerciseTile :question="question" :topic="topicLabel(question.topic_id)" />
          </NuxtLink>
        </li>
      </ul>
    </template>
  </div>
</template>