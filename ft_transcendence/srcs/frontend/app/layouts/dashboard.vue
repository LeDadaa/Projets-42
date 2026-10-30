<script setup lang="ts">
/* Realtime only makes sense once authenticated: we open the socket here, on
   the layout for private pages, and close it on leave. Conversations and the
   friends graph are loaded once so the sidebar badges are live everywhere. */
const realtime = useRealtime()
const presence = usePresence()
const { fetchMe } = useAuth()
const chat = useChat()
const { refresh: refreshFriends } = useFriends()
const { announce } = useAchievements()
const route = useRoute()

/* Below `lg` the sidebar leaves the flow and slides in as a drawer, opened
   from the top bar. Navigating anywhere closes it. */
const menuOpen = ref(false)

watch(
  () => route.fullPath,
  () => {
    menuOpen.value = false
  },
)

function onKeydown(event: KeyboardEvent) {
  if (event.key === 'Escape') menuOpen.value = false
}

const stopAchievements = realtime.on('achievements.unlocked', (message) => {
  if (Array.isArray(message.codes)) announce(message.codes)
})

onMounted(async () => {
  window.addEventListener('keydown', onKeydown)
  await fetchMe()
  realtime.start()
  presence.hydrate()
  chat.init()
  refreshFriends()
})

onBeforeUnmount(() => {
  window.removeEventListener('keydown', onKeydown)
  stopAchievements()
  realtime.stop()
})
</script>

<template>
  <div class="flex h-dvh w-full">
    <div
      v-if="menuOpen"
      aria-hidden="true"
      class="fixed inset-0 z-40 bg-black/50 lg:hidden"
      @click="menuOpen = false"
    />
    <LayoutSidebar
      id="app-sidebar"
      class="fixed inset-y-0 left-0 z-50 transition-[transform,visibility] duration-base ease-out motion-reduce:transition-none lg:static lg:translate-x-0"
      :class="menuOpen ? 'translate-x-0 shadow-lg' : 'max-lg:invisible -translate-x-full'"
    />
    <div class="flex min-w-0 flex-1 flex-col">
      <header
        class="flex flex-none items-center gap-3 border-b border-line-strong bg-surface-card px-4 py-2 lg:hidden"
      >
        <button
          type="button"
          class="inline-flex size-8 items-center justify-center rounded-md text-fg-body transition-colors duration-fast hover:bg-surface-hover focus-visible:shadow-focus focus-visible:outline-none"
          :aria-label="menuOpen ? $t('nav.closeMenu') : $t('nav.openMenu')"
          :aria-expanded="menuOpen"
          aria-controls="app-sidebar"
          @click="menuOpen = !menuOpen"
        >
          <Icon :name="menuOpen ? 'lucide:x' : 'lucide:menu'" mode="svg" size="1.5em" />
        </button>
        <BrandLogo to="/dashboard" />
      </header>
      <main
        class="flex flex-1 flex-col overflow-auto px-4 pt-5 sm:px-6 sm:pt-6 lg:px-11 lg:pt-[36px]"
      >
        <slot />
        <AppFooter class="mt-auto pt-9" />
      </main>
    </div>
    <UiToaster />
  </div>
</template>
