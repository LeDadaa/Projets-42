<script setup lang="ts">
const { tags } = defineProps<{
  tags: readonly string[]
}>()

const { notionLabel } = useNotions()
</script>

<template>
  <ul class="flex flex-wrap gap-1.5">
    <li
      v-for="tag in tags"
      :key="tag"
      class="rounded-sm border border-line-subtle bg-surface-raised px-2 py-[3px] text-xs text-fg-muted"
    >
      {{ notionLabel(tag) }}
    </li>
  </ul>
</template>
