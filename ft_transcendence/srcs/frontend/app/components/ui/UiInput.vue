<script setup lang="ts">
defineOptions({ inheritAttrs: false })

type InputType = 'text' | 'email' | 'password' | 'search' | 'number' | 'tel' | 'url'

const { type = 'text', icon } = defineProps<{
  type?: InputType
  icon?: string
}>()

const model = defineModel<string>()

const el = ref<HTMLInputElement>()

defineExpose({ el })

const BASE =
  'h-7 w-full rounded-md border border-line-strong bg-surface-raised font-sans text-sm text-fg-body transition-[border-color,box-shadow] duration-fast placeholder:text-fg-subtle focus-visible:border-accent-2 focus-visible:shadow-focus focus-visible:outline-none'

const PADDING = {
  withIcon: 'pr-3 pl-[36px]',
  plain: 'px-3',
}
</script>

<template>
  <div class="relative w-full">
    <Icon
      v-if="icon"
      :name="icon"
      mode="svg"
      size="1em"
      aria-hidden="true"
      class="pointer-events-none absolute top-1/2 left-3 -translate-y-1/2 text-fg-subtle"
    />
    <input
      ref="el"
      v-model="model"
      :type="type"
      :class="[BASE, icon ? PADDING.withIcon : PADDING.plain]"
      v-bind="$attrs"
    />
  </div>
</template>
