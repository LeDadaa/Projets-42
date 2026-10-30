<script setup lang="ts">
definePageMeta({ layout: 'default' })

const PITCH = [
  { key: 'socratic', icon: 'lucide:message-circle-question' },
  { key: 'exams', icon: 'lucide:code' },
  { key: 'progress', icon: 'lucide:flame' },
]

const STEPS = [
  { key: 'pick', icon: 'lucide:list-checks' },
  { key: 'answer', icon: 'lucide:message-circle-question' },
  { key: 'validate', icon: 'lucide:trophy' },
]

const EYEBROW = 'text-xs font-semibold tracking-caps uppercase text-fg-subtle'
</script>

<template>
  <div class="flex w-full flex-col">
    <AppHeader>
      <NuxtLink to="/login" class="hidden sm:block">
        <UiButton variant="ghost" :label="$t('auth.login')" />
      </NuxtLink>
      <NuxtLink to="/sign-up" class="hidden sm:block">
        <UiButton :label="$t('auth.signUp')" />
      </NuxtLink>
    </AppHeader>

    <main class="flex flex-1 flex-col items-center px-4 pt-5 pb-5 sm:px-8 sm:pt-7">
      <section class="flex w-full max-w-350 flex-wrap items-center gap-8 sm:gap-12">
        <div class="min-w-0 flex-1 basis-105">
          <UiTag :label="$t('landing.badge')" />
          <h1 class="mb-4 text-3xl tracking-tight sm:text-4xl">{{ $t('landing.title') }}</h1>
          <p class="mb-5 max-w-140 text-lg leading-relaxed text-fg-muted">
            {{ $t('landing.subtitle') }}
          </p>
          <div class="flex flex-wrap gap-3">
            <NuxtLink to="/sign-up">
              <UiButton size="lg" :label="$t('landing.ctaPrimary')">
                <template #trailing>
                  <Icon name="lucide:arrow-right" mode="svg" size="1em" />
                </template>
              </UiButton>
            </NuxtLink>
            <NuxtLink to="/login">
              <UiButton size="lg" variant="secondary" :label="$t('landing.ctaSecondary')" />
            </NuxtLink>
          </div>
        </div>

        <ul class="flex min-w-0 flex-1 basis-85 flex-col gap-3.5">
          <li v-for="item in PITCH" :key="item.key">
            <UiCard pad>
              <div class="flex items-start gap-4">
                <span
                  aria-hidden="true"
                  class="flex size-11 flex-none items-center justify-center rounded-md bg-accent-soft text-accent"
                >
                  <Icon :name="item.icon" mode="svg" size="1.375em" />
                </span>
                <div>
                  <h3 class="mb-1 text-md">{{ $t(`landing.pitch.${item.key}.title`) }}</h3>
                  <p class="text-sm leading-normal text-fg-muted">
                    {{ $t(`landing.pitch.${item.key}.body`) }}
                  </p>
                </div>
              </div>
            </UiCard>
          </li>
        </ul>
      </section>

      <section class="mt-12 w-full max-w-350 sm:mt-16">
        <h2 class="mb-1.5 text-2xl font-semibold">{{ $t('landing.preview.title') }}</h2>
        <p class="mb-6 max-w-150 text-md text-fg-muted">
          {{ $t('landing.preview.subtitle') }}
        </p>

        <div class="mb-11 grid items-center gap-6 lg:grid-cols-[minmax(0,1fr)_minmax(0,620px)]">
          <div class="overflow-hidden rounded-lg border border-line-subtle shadow-lg">
            <img
              src="/img/02-dashboard.png"
              :alt="$t('landing.preview.dashboardAlt')"
              width="924"
              height="540"
              class="block h-auto w-full max-w-190"
            />
          </div>
          <div>
            <p class="mb-2" :class="EYEBROW">{{ $t('landing.preview.dashboardEyebrow') }}</p>
            <p class="text-md leading-relaxed text-fg-muted">
              {{ $t('landing.preview.dashboardBody') }}
            </p>
          </div>
        </div>

        <div class="mb-11 ml-auto max-w-190">
          <p class="mb-1.5" :class="EYEBROW">{{ $t('landing.how.eyebrow') }}</p>
          <h3 class="mb-3 text-xl font-semibold">{{ $t('landing.how.title') }}</h3>
          <ol class="flex flex-col">
            <li
              v-for="(step, index) in STEPS"
              :key="step.key"
              class="flex items-start gap-4 py-4"
              :class="index < STEPS.length - 1 ? 'border-b border-line-subtle' : ''"
            >
              <span
                aria-hidden="true"
                class="flex size-7 flex-none items-center justify-center rounded-md bg-accent-soft font-mono font-semibold text-accent"
              >
                {{ index + 1 }}
              </span>
              <div>
                <div class="mb-1 flex items-center gap-2">
                  <Icon
                    :name="step.icon"
                    mode="svg"
                    size="1.0625em"
                    aria-hidden="true"
                    class="text-accent"
                  />
                  <h4 class="text-md">{{ $t(`landing.how.${step.key}.title`) }}</h4>
                </div>
                <p class="text-sm leading-normal text-fg-muted">
                  {{ $t(`landing.how.${step.key}.body`) }}
                </p>
              </div>
            </li>
          </ol>
        </div>

        <div class="flex max-w-190 flex-col gap-3.5">
          <p :class="EYEBROW">{{ $t('landing.reasoning.eyebrow') }}</p>
          <p class="text-md leading-normal text-fg-muted">{{ $t('landing.reasoning.body') }}</p>
          <div
            class="overflow-hidden rounded-lg border border-line-subtle bg-surface-page shadow-lg"
          >
            <div class="flex flex-col gap-3.5 p-4 sm:p-5">
              <div class="flex justify-end gap-2.5">
                <p
                  class="max-w-[78%] rounded-lg rounded-tr-xs bg-accent-2-soft px-3.5 py-2.5 text-sm"
                >
                  {{ $t('landing.reasoning.userFirst') }}
                </p>
                <UiAvatar name="Nour" :size="30" />
              </div>
              <div class="flex gap-2.5">
                <span
                  aria-hidden="true"
                  class="flex size-7.5 flex-none items-center justify-center rounded-full bg-accent-soft text-accent"
                >
                  <Icon name="lucide:sparkles" mode="svg" size="1em" />
                </span>
                <div class="w-fit max-w-[78%] rounded-lg rounded-tl-xs bg-accent-soft px-4 py-3">
                  <p class="mb-1 text-xs font-semibold text-accent">Maïa</p>
                  <i18n-t keypath="landing.reasoning.maia" tag="p" class="text-sm leading-normal">
                    <template #str>
                      <code class="font-mono">str</code>
                    </template>
                    <template #nullValue>
                      <code class="font-mono">NULL</code>
                    </template>
                  </i18n-t>
                </div>
              </div>
              <div class="flex justify-end gap-2.5">
                <p
                  class="max-w-[78%] rounded-lg rounded-tr-xs bg-accent-2-soft px-3.5 py-2.5 text-sm"
                >
                  {{ $t('landing.reasoning.userSecond') }}
                </p>
                <UiAvatar name="Nour" :size="30" />
              </div>
              <p
                class="flex items-center gap-2 self-start rounded-full bg-success-soft px-3 py-1.5 text-sm font-semibold text-success"
              >
                <Icon name="lucide:circle-check-big" mode="svg" size="1em" aria-hidden="true" />
                {{ $t('landing.reasoning.success') }}
              </p>
            </div>
          </div>
        </div>
      </section>
    </main>
  </div>
</template>
