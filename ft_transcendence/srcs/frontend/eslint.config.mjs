// @ts-check
import prettier from 'eslint-config-prettier'
import withNuxt from './.nuxt/eslint.config.mjs'

/* `withNuxt` vient de @nuxt/eslint : il génère une config adaptée au projet
   (globals des auto-imports, règles Vue, résolution des composants).
   `prettier` doit rester en DERNIER : son seul rôle est de désactiver les
   règles de formatage d'ESLint — surtout celles d'eslint-plugin-vue, comme
   vue/html-indent ou vue/max-attributes-per-line — qui se battraient
   sinon avec Prettier. */
export default withNuxt(prettier, {
  name: 'maia/overrides',
  rules: {
    /* Redondant en TypeScript : `label?: string` déclare déjà que la prop peut
       être absente, et Tags s'appuie sur ce `undefined` (fallback `label ?? type`). */
    'vue/require-default-prop': 'off',
  },
})
