<script setup lang="ts">
defineProps<{
  title: string
  subtitle: string
  altLabel?: string
  altTo?: string
}>()
</script>

<template>
  <div class="flex w-full flex-1 flex-col">
    <AppHeader>
      <NuxtLink v-if="altLabel && altTo" :to="altTo">
        <UiButton variant="ghost" :label="altLabel" />
      </NuxtLink>
    </AppHeader>

    <main class="flex flex-1 items-center justify-center px-4 py-5 sm:p-6">
      <UiCard class="w-full max-w-105 p-5 sm:p-[36px]">
        <div class="mb-6 text-center">
          <h1 class="text-2xl">{{ title }}</h1>
          <p class="text-sm text-fg-muted">{{ subtitle }}</p>
        </div>
        <slot />
      </UiCard>
    </main>
  </div>
</template>
