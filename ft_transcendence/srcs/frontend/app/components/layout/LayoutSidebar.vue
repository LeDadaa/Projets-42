<script setup lang="ts">
const colorMode = useColorMode()

const { logout } = useAuth()

const isDark = computed(() => colorMode.value === 'dark')

function toggleTheme() {
  colorMode.preference = isDark.value ? 'light' : 'dark'
}

const { unreadTotal } = useChat()
const { pendingCount } = useFriends()
</script>

<template>
  <div
    class="flex justify-between flex-col bg-surface-card w-64 shrink-0 overflow-y-auto border-r border-line-strong h-full"
  >
    <div class="w-full">
      <BrandLogo class="ml-5 px-2 py-2.5 my-2" />
      <ul class="flex flex-col gap-1 px-3 mt-3">
        <li>
          <LayoutNavLink :label="$t('nav.dashboard')" to="/dashboard">
            <Icon name="lucide:layout-dashboard" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.exercices')" to="/exercices" active-prefix="/exercice">
            <Icon name="lucide:list-checks" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.trainingSession')" to="/training-session">
            <Icon name="lucide:timer" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.multiplayer')" to="/multiplayer">
            <Icon name="lucide:swords" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.messages')" to="/messages" :badge="unreadTotal">
            <Icon name="lucide:message-circle" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.friends')" to="/friends" :badge="pendingCount">
            <Icon name="lucide:users" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
        <li>
          <LayoutNavLink :label="$t('nav.profil')" to="/profil">
            <Icon name="lucide:user" mode="svg" size="1.5em" />
          </LayoutNavLink>
        </li>
      </ul>
    </div>
    <div class="flex flex-col w-full">
      <div class="flex flex-col gap-1 px-3">
        <LayoutNavButton
          :label="isDark ? $t('theme.light') : $t('theme.dark')"
          @click="toggleTheme"
        >
          <Icon :name="isDark ? 'lucide:sun' : 'lucide:moon'" mode="svg" size="1.5em" />
        </LayoutNavButton>
        <LanguageSwitcher class="px-2 py-1" />
      </div>
      <LayoutNavButton :label="$t('nav.logout')" @click="logout">
        <Icon name="lucide:log-out" mode="svg" size="1.5em" />
      </LayoutNavButton>
      <LayoutSettingsButton />
      <LayoutConnectionStatus class="border-t border-line-subtle" />
    </div>
  </div>
</template>
