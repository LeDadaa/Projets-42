<script setup lang="ts">
const emit = defineEmits<{ start: [topicId: number, count: number, minutes: number | null] }>()

const { exams, questionsOf, questionCount, topicLabel } = useQuestions()

const level = ref<number | null>(null)
const size = ref<number | 'all'>(10)
const minutes = ref<number | null>(60)

const DURATIONS = [
  { minutes: 30, key: 'training.setup.duration30' },
  { minutes: 60, key: 'training.setup.duration60' },
  { minutes: 120, key: 'training.setup.duration120' },
  { minutes: null, key: 'training.setup.durationNone' },
]

const availableExams = computed(() => exams.value.filter((exam) => questionCount(exam.id) > 0))

const available = computed(() => (level.value === null ? 0 : questionsOf(level.value).length))

const sizes = computed(() => [5, 10].filter((n) => n < available.value))

const effectiveSize = computed(() => {
  const num = typeof size.value === 'number' ? size.value : available.value
  return num >= available.value ? 'all' : num
})

const root = ref<HTMLElement | null>(null)
const heading = ref<HTMLElement | null>(null)

function choose(levelId: number) {
  level.value = levelId
  size.value = 10
  nextTick(() => heading.value?.focus())
}

function back() {
  level.value = null
  nextTick(() =>
    root.value
      ?.querySelector<HTMLElement>('[aria-roledescription="slide"]:not([aria-hidden]) button')
      ?.focus(),
  )
}

function handleStart() {
  if (level.value === null) return
  const count = effectiveSize.value === 'all' ? available.value : effectiveSize.value
  emit('start', level.value, count, minutes.value)
}
</script>

<template>
  <div ref="root">
    <template v-if="level === null">
      <p class="mb-1.5 font-mono text-xs text-fg-subtle">
        {{ $t('training.setup.step', { n: 1 }) }}
      </p>
      <p class="mb-5 text-fg-muted">{{ $t('training.setup.intro') }}</p>
      <ExerciseExamCarousel :exams="availableExams" selectable @level="choose" />
    </template>
    <UiCard v-else pad>
      <p class="mb-1.5 font-mono text-xs text-fg-subtle">
        {{ $t('training.setup.step', { n: 2 }) }}
      </p>
      <h2 ref="heading" tabindex="-1" class="mb-1 text-xl font-semibold focus-visible:outline-none">
        {{ topicLabel(level) }}
      </h2>
      <p class="mb-5 text-sm text-fg-muted">
        {{ $t('exercices.exercisesCount', available) }}
      </p>
      <div class="flex flex-col gap-5">
        <fieldset>
          <legend class="mb-1.5 text-sm font-semibold">
            {{ $t('training.setup.count') }}
          </legend>
          <div class="flex flex-wrap gap-2">
            <UiChip
              v-for="n in sizes"
              :key="n"
              :label="String(n)"
              :active="size === n"
              @click="size = n"
            />
            <UiChip
              :label="$t('training.setup.all', { n: available })"
              :active="effectiveSize === 'all'"
              @click="size = 'all'"
            />
          </div>
        </fieldset>

        <fieldset>
          <legend class="mb-1.5 text-sm font-semibold">
            {{ $t('training.setup.duration') }}
          </legend>
          <div class="flex flex-wrap gap-2">
            <UiChip
              v-for="(option, index) in DURATIONS"
              :key="index"
              :label="$t(option.key)"
              :active="minutes === option.minutes"
              @click="minutes = option.minutes"
            />
          </div>
        </fieldset>

        <div class="flex flex-wrap items-center justify-between gap-3">
          <UiButton variant="secondary" :label="$t('training.setup.back')" @click="back">
            <Icon name="lucide:arrow-left" mode="svg" size="1em" />
          </UiButton>
          <UiButton :label="$t('training.setup.start')" @click="handleStart" />
        </div>
      </div>
    </UiCard>
  </div>
</template>
