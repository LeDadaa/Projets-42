<script setup lang="ts">
const { name, description, detail, icon } = defineProps<{
  name: string
  description?: string | null
  detail: string
  icon: string
}>()
</script>

<template>
  <UiCard pad interactive class="h-full">
    <div class="flex h-full items-center gap-3.5">
      <span
        aria-hidden="true"
        class="flex size-8.5 flex-none items-center justify-center rounded-md bg-accent-soft text-accent"
      >
        <Icon :name="icon" mode="svg" size="1.125em" />
      </span>

      <div class="min-w-0 flex-1">
        <p class="mb-0.75 font-semibold">{{ name }}</p>
        <p v-if="description" class="mb-0.75 text-sm text-fg-muted">{{ description }}</p>
        <p class="text-xs text-fg-subtle">{{ detail }}</p>
      </div>

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
