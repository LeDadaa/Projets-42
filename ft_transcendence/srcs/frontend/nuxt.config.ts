// https://nuxt.com/docs/api/configuration/nuxt-config
import tailwindcss from '@tailwindcss/vite'

export default defineNuxtConfig({
  compatibilityDate: '2025-07-15',
  devtools: { enabled: true },

  css: [
    '@fontsource-variable/ibm-plex-sans/wght.css',
    '@fontsource/ibm-plex-mono/latin-400.css',
    '@fontsource/ibm-plex-mono/latin-500.css',
    '@fontsource/ibm-plex-mono/latin-600.css',
    '~/assets/css/main.css',
  ],
  ssr: false,
  vite: {
    plugins: [tailwindcss()],
  },
  modules: ['@nuxt/icon', '@nuxtjs/color-mode', '@nuxt/eslint', '@nuxtjs/i18n'],
  // SPA build: no Nitro server to serve icons, and the CSP forbids the Iconify
  // API. Every `lucide:*` name found in the sources ships in the client bundle.
  icon: {
    clientBundle: { scan: true },
  },
  colorMode: {
    preference: 'dark',
    fallback: 'dark',
    dataValue: 'theme',
    classSuffix: '',
  },
  i18n: {
    defaultLocale: 'fr',
    strategy: 'no_prefix',
    locales: [
      { code: 'fr', name: 'Français', language: 'fr-FR', file: 'fr.json' },
      { code: 'en', name: 'English', language: 'en-US', file: 'en.json' },
      { code: 'es', name: 'Español', language: 'es-ES', file: 'es.json' },
    ],
    detectBrowserLanguage: {
      useCookie: true,
      cookieKey: 'maia_locale',
      alwaysRedirect: true,
      fallbackLocale: 'fr',
    },
  },
})
