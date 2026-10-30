<script setup lang="ts">
interface Level {
  id: number
  name: string
  description: string | null
  count: number
}

defineProps<{
  name: string
  detail: string
  levels: Level[]
  active?: boolean
  selectable?: boolean
}>()

const emit = defineEmits<{ level: [levelId: number] }>()
</script>

<template>
  <UiCard pad class="h-full">
    <div class="flex h-full flex-col">
      <span
        aria-hidden="true"
        class="mb-4 flex size-8.5 items-center justify-center rounded-md bg-accent-soft text-accent"
      >
        <Icon name="lucide:graduation-cap" mode="svg" size="1.125em" />
      </span>
      <p class="mb-1 text-xl font-semibold break-words">{{ name }}</p>
      <p class="mb-4 text-sm text-fg-muted">{{ detail }}</p>
      <ul :class="['flex flex-col', active && selectable ? 'gap-1' : 'gap-3']">
        <li v-for="level in levels" :key="level.id">
          <template v-if="active && selectable">
            <button
              type="button"
              class="w-full rounded-md p-2 text-left transition-colors duration-fast hover:bg-surface-hover focus-visible:shadow-focus focus-visible:outline-none disabled:pointer-events-none disabled:opacity-50"
              :disabled="level.count === 0"
              @click="emit('level', level.id)"
            >
              <span class="block text-sm font-semibold">
                {{ level.name }}
                <span class="font-normal text-fg-subtle">
                  · {{ $t('exercices.exercisesCount', level.count) }}
                </span>
              </span>
              <span v-if="level.description" class="line-clamp-2 text-xs text-fg-subtle">
                {{ level.description }}
              </span>
            </button>
          </template>
          <template v-else>
            <p class="text-sm font-semibold">
              {{ level.name }}
              <span class="font-normal text-fg-subtle">
                · {{ $t('exercices.exercisesCount', level.count) }}
              </span>
            </p>
            <p v-if="level.description" class="line-clamp-2 text-xs text-fg-subtle">
              {{ level.description }}
            </p>
          </template>
        </li>
      </ul>
      <p
        v-if="active && !selectable"
        class="mt-auto flex items-center gap-1.5 pt-4 text-sm font-semibold text-accent"
      >
        {{ $t('exercices.carousel.open') }}
        <Icon name="lucide:arrow-right" mode="svg" size="1em" aria-hidden="true" />
      </p>
    </div>
  </UiCard>
</template>
