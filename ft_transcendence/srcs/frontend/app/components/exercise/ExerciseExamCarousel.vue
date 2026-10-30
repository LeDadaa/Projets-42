<script setup lang="ts">
import type { Topic } from '~/types'

const { exams, selectable } = defineProps<{ exams: Topic[]; selectable?: boolean }>()
const { t } = useI18n()
const { childrenOf, questionCount } = useQuestions()
const emit = defineEmits<{ level: [levelId: number] }>()

const selected = useState<number>('exercices-selected-exam', () => 0)
const total = computed(() => exams.length)
const active = computed(() => (total.value ? Math.min(selected.value, total.value - 1) : 0))

function go(step: number) {
  if (total.value) selected.value = (active.value + step + total.value) % total.value
}

function select(index: number) {
  selected.value = index
}

const root = ref<HTMLElement | null>(null)

function onArrowKey(step: number) {
  const fromCard = document.activeElement?.closest('[aria-roledescription="slide"]')
  go(step)
  if (fromCard) {
    nextTick(() =>
      root.value
        ?.querySelector<HTMLElement>(
          '[aria-roledescription="slide"]:not([aria-hidden]) a, [aria-roledescription="slide"]:not([aria-hidden]) button',
        )
        ?.focus(),
    )
  }
}

function offsetOf(index: number) {
  let d = (index - active.value + total.value) % total.value
  if (d > total.value / 2) d -= total.value
  return d
}

const cards = computed(() =>
  exams.map((exam, index) => ({
    exam,
    offset: offsetOf(index),
    detail:
      t('exercices.levelsCount', childrenOf(exam.id).length) +
      ' · ' +
      t('exercices.exercisesCount', questionCount(exam.id)),
    levels: childrenOf(exam.id).map((level) => ({
      id: level.id,
      name: level.name ?? level.slug,
      description: level.description,
      count: questionCount(level.id),
    })),
  })),
)

function cardStyle(offset: number): Record<string, string | number | undefined> {
  if (offset === 0) {
    return { transform: 'translateX(-50%) scale(1)', opacity: 1, zIndex: 3 }
  }
  if (Math.abs(offset) === 1) {
    return {
      transform: `translateX(calc(-50% + ${offset * 72}%)) translateY(-10px) scale(0.82) rotateY(${-offset * 18}deg)`,
      opacity: 0.55,
      zIndex: 2,
    }
  }
  return {
    transform: `translateX(calc(-50% + ${Math.sign(offset) * 110}%)) scale(0.7) rotateY(${-Math.sign(offset) * 30}deg)`,
    opacity: 0,
    zIndex: 1,
    pointerEvents: 'none',
  }
}

let startX: number | null = null

function onPointerDown(event: PointerEvent) {
  startX = event.pointerType === 'mouse' ? null : event.clientX
}

function onPointerUp(event: PointerEvent) {
  if (startX === null) return
  const dx = event.clientX - startX
  startX = null
  if (Math.abs(dx) > 40) go(dx < 0 ? 1 : -1)
}

const announcement = computed(() => {
  const exam = exams[active.value]
  return exam
    ? t('exercices.carousel.current', {
        name: exam.name ?? exam.slug,
        n: active.value + 1,
        total: total.value,
      })
    : ''
})

const ARROW =
  'inline-flex size-7 items-center justify-center rounded-full border border-line-strong bg-surface-raised text-fg-muted transition-colors duration-fast hover:bg-surface-hover hover:text-fg-body focus-visible:shadow-focus focus-visible:outline-none'

const DOT: Record<'active' | 'inactive', string> = {
  active: 'bg-accent',
  inactive: 'bg-line-strong',
}
</script>

<template>
  <section
    ref="root"
    :aria-label="$t('exercices.carousel.label')"
    aria-roledescription="carousel"
    @keydown.left.prevent="onArrowKey(-1)"
    @keydown.right.prevent="onArrowKey(1)"
  >
    <div class="overflow-hidden rounded-xl border border-line-subtle bg-surface-code">
      <div
        class="relative h-132 touch-pan-y"
        :style="{ perspective: '1200px' }"
        @pointerdown="onPointerDown"
        @pointerup="onPointerUp"
      >
        <div
          v-for="(card, index) in cards"
          :key="card.exam.id"
          role="group"
          aria-roledescription="slide"
          :aria-label="$t('exercices.carousel.slide', { n: index + 1, total })"
          :aria-hidden="card.offset !== 0 ? 'true' : undefined"
          :inert="Math.abs(card.offset) > 1"
          class="absolute bottom-6 left-1/2 h-120 w-[min(18rem,calc(100%-2.5rem))] origin-bottom transition-[transform,opacity] duration-slow ease-out motion-reduce:transition-none"
          :style="cardStyle(card.offset)"
        >
          <template v-if="card.offset === 0">
            <template v-if="selectable">
              <ExerciseExamCard
                :name="card.exam.name ?? card.exam.slug"
                :detail="card.detail"
                :levels="card.levels"
                active
                selectable
                @level="emit('level', $event)"
              />
            </template>
            <template v-else>
              <NuxtLink
                :to="`/exercices/${card.exam.slug}`"
                class="block h-full rounded-lg focus-visible:shadow-focus focus-visible:outline-none"
              >
                <ExerciseExamCard
                  :name="card.exam.name ?? card.exam.slug"
                  :detail="card.detail"
                  :levels="card.levels"
                  active
                />
              </NuxtLink>
            </template>
          </template>
          <template v-else>
            <ExerciseExamCard
              :name="card.exam.name ?? card.exam.slug"
              :detail="card.detail"
              :levels="card.levels"
            />
            <button
              type="button"
              tabindex="-1"
              class="absolute inset-0 cursor-pointer rounded-lg"
              @click="select(index)"
            >
              <span class="sr-only">{{ card.exam.name ?? card.exam.slug }}</span>
            </button>
          </template>
        </div>
      </div>
      <div class="flex items-center justify-center gap-3 pb-6">
        <template v-if="total > 1">
          <button
            type="button"
            :class="ARROW"
            :aria-label="$t('exercices.carousel.previous')"
            @click="go(-1)"
          >
            <Icon name="lucide:chevron-left" mode="svg" size="1.125em" aria-hidden="true" />
          </button>
          <div class="flex items-center">
            <button
              v-for="(card, index) in cards"
              :key="card.exam.id"
              type="button"
              class="rounded-full p-2 focus-visible:shadow-focus focus-visible:outline-none"
              :aria-label="card.exam.name ?? card.exam.slug"
              :aria-current="card.offset === 0 ? 'true' : undefined"
              @click="select(index)"
            >
              <span
                class="block size-2 rounded-full"
                :class="DOT[card.offset === 0 ? 'active' : 'inactive']"
              />
            </button>
          </div>
          <button
            type="button"
            :class="ARROW"
            :aria-label="$t('exercices.carousel.next')"
            @click="go(1)"
          >
            <Icon name="lucide:chevron-right" mode="svg" size="1.125em" aria-hidden="true" />
          </button>
        </template>
      </div>
    </div>
    <p class="sr-only" aria-live="polite">{{ announcement }}</p>
  </section>
</template>
