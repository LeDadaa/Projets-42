# Frontend — état, conventions et ce qu'il reste à construire

Référence de travail du frontend. Le périmètre en points est dans `MODULES.md` à la racine, le contrat d'API dans `srcs/backend/BACKEND.md`.

**Toutes les commandes se lancent depuis `srcs/frontend`**, jamais depuis la racine : il n'y a pas de `package.json` à la racine, et un `pnpm add` lancé de là en crée un parasite.

```bash
pnpm lint          # eslint .
pnpm lint:fix
pnpm format        # prettier --write .
pnpm format:check
pnpm typecheck     # vue-tsc — PAS dans le serveur de dev, à lancer à la main
```

---

## État actuel

`eslint` 0 problème · `prettier --check` conforme · `vue-tsc` exit 0.

37 composants, 14 pages, 273 clés i18n par locale. **Couche d'intégration API** : `useApi` (fetch same-origin + header CSRF depuis le cookie `csrf_token`), `useAuth` (compte courant, connexion, 2FA, profil) et `useMe`, son alias pour le chat. `middleware/auth.ts` protège les pages du layout `dashboard`. `runtimeConfig` manque toujours — inutile tant que toutes les URLs sont relatives. Le module « interaction utilisateur » (messages, amis, profils publics) consomme déjà le backend via cette couche. Les exercices, le tableau de bord et la session d'entraînement lisent les vraies questions via `useQuestions`, un cache partagé entre les pages et rechargé seulement quand la langue change. La page Exercices se parcourt exam → niveau → exercice, une URL par étape : les exams s'y choisissent dans un carrousel à effet de profondeur, et la recherche par notion vit sur la page d'un exam, où elle ne cherche que dans ses exercices.

**La session d'entraînement se règle en deux étapes** : le même carrousel pour choisir l'exam puis le niveau, ensuite le nombre de questions et la durée plafond (30 min, 1 h, 2 h ou sans limite). La session vit dans `sessionStorage` (`useTrainingSession`) : un rechargement ne remet à zéro ni le chrono, ni la position, ni les brouillons. À l'expiration ou sur « Terminer », un bilan liste les questions validées, à revoir et sans réponse.

**La correction se lit comme une conversation** (`ExerciseGrader`) : les réponses de l'élève à droite, celles de Maïa à gauche, une pilule de verdict sous chacune, et la saisie détachée de l'encadré de la question. Tous les essais restent dans le fil, qui repart à zéro en changeant de question. Un indicateur au-dessus du fil affiche les essais restants, lus sur le serveur à l'ouverture — jamais supposés côté front.

```
app/
  app.vue                 coquille : NuxtLayout > NuxtPage
  layouts/
    default.vue           pages publiques : châssis + ThemeToggle fixe
    dashboard.vue         Sidebar + <main> (porte le scroll et le padding)
  middleware/auth.ts      protege les pages du layout dashboard
  pages/                  index · login · sign-up · forgotten-password
                          dashboard · exercices/index · exercices/[topic] · exercice-[slug]
                          profil · settings (squelettes) · training-session
                          messages · friends · users/[id]  (module interaction)
  components/
    ui/          (14)     UiButton UiTag UiChip UiCard UiAvatar UiStat UiPresenceDot
                          UiInput UiTextarea UiSelect UiProgress UiCallout UiCodeText UiBreadcrumb
    auth/        (3)      AuthShell AuthProviders AuthDivider
    layout/      (5)      LayoutSidebar NavLink NavButton SettingsButton ConnectionStatus
    exercise/    (7)      ExerciseTile · ExerciseRow · ExerciseTags · ExerciseTopicCard · ExerciseGrader
                          ExerciseExamCard · ExerciseExamCarousel (selecteur d'exam en carrousel)
    training/    (2)      TrainingSetup (les deux etapes) · TrainingRecap (le bilan)
    chat/        (2)      ChatWindow · ChatConversationList
    racine       (4)      AppHeader BrandLogo LanguageSwitcher ThemeToggle
  composables/            useApi (fetch + CSRF) · useAuth · useMe · useChat · useFriends
                          useRealtime · usePresence · useQuestions · useNotions · useGrader
                          useTrainingSession (session chronometree, sessionStorage)
  types/index.ts          Difficulty · Topic · QuestionSummary · QuestionPage · AttemptQuota · User
                          UserSummary · Profile · Friend · PendingRequest
                          DirectMessage · Conversation · GraderOutcome · FallbackReason · GraderEvent
  utils/demo.ts           DEMO_USER (auto-importé)
  utils/exercise.ts       difficultyOf · DIFFICULTY_TAG — note de difficulté → libellé et apparence
  utils/text.ts           foldText — comparaison sans accents ni casse, pour la recherche
  assets/css/
    maia.token.css        tokens bruts en oklch, thèmes sombre et clair
    main.css              @theme Tailwind + @layer base
i18n/locales/             fr.json · en.json · es.json
```

## Stack

Nuxt 4.5 · Vue 3.5 · TypeScript · **SPA (`ssr: false`)** · Tailwind CSS v4 via `@tailwindcss/vite` (pas de config JS) · `@nuxtjs/i18n` v10 · `@nuxtjs/color-mode` · `@nuxt/icon` + `@iconify-json/lucide` · `@fontsource` IBM Plex Sans/Mono · ESLint 10 + Prettier 3.

**En dev, tout passe par `https://localhost`** — pas `localhost:3000`. Le service `web-dev` n'expose aucun port ; nginx proxifie `/` vers Nuxt (HMR compris) et `/api` vers l'API. Même origine qu'en production, donc les cookies `Secure` et l'absence de CORS se comportent pareil dans les deux environnements. Le certificat est auto-signé : l'avertissement du navigateur est normal, et `curl` a besoin de `-k`.

---

# Conventions

## Ne pas ajouter de bibliothèque de composants

Le module « Custom-made design system » exige un design system **fait maison**. Installer Nuxt UI, Vuetify ou PrimeVue **détruit ce point**. Les composants complexes (Modal, Toast, Table) s'écrivent à la main — c'est un choix assumé, pas un oubli.

Corollaire : un composant inutilisé n'est pas du code mort, c'est un livrable du module.

## Le pattern variant-map

Tout composant à variantes suit ce schéma :

```ts
type Variant = 'primary' | 'secondary' | 'ghost'
const BASE = 'inline-flex items-center gap-2 rounded-md border …'
const VARIANTS: Record<Variant, string> = {
  primary: 'border-transparent bg-accent text-on-accent hover:bg-accent-hover',
  secondary: 'border-line-strong bg-surface-raised text-fg-body hover:bg-surface-hover',
  ghost: 'border-transparent text-fg-muted hover:bg-surface-hover hover:text-fg-body',
}
```

Deux raisons :

1. **Tailwind n'évalue pas le JavaScript.** Il scanne le source à la recherche de chaînes ressemblant à des classes. Une classe interpolée (`` `text-${type}` ``) n'est **jamais générée**. Une constante contenant la chaîne littérale complète, en revanche, est bien vue.
2. **`Record<Variant, …>` est exhaustif.** Ajouter une valeur au type sans l'ajouter à la map est une erreur TypeScript — encore faut-il que `pnpm typecheck` tourne, il n'est pas dans le serveur de dev.

## Les classes vivent dans le composant, pas au point d'appel

Un composant porte ses couleurs, paddings et graisses. Le point d'appel ne passe que du **positionnement** (`mx-3`, `fixed`, `z-50`). Sinon on obtient trois paddings différents pour le même bouton.

## Jamais deux classes Tailwind sur la même propriété

Deux utilitaires qui touchent la **même propriété CSS** ont la même spécificité. Ce n'est donc pas l'ordre dans l'attribut `class` qui tranche, mais l'ordre d'émission de Tailwind — hors de notre contrôle et susceptible de changer de version en version.

Trois occurrences rencontrées, toutes silencieuses (CSS valide, lint vert) :

| Superposition                                                                    | Symptôme               |
| -------------------------------------------------------------------------------- | ---------------------- |
| `text-fg-body` (base) + `text-accent-press` (`active-class`)                     | lien actif resté blanc |
| `border-transparent` (base de `UiButton`) + `border-line-subtle` (point d'appel) | bordure invisible      |
| `border-transparent` (base) + `border-line-strong` (variant `secondary`)         | variant sans bordure   |

Une propriété se déclare à **un seul endroit**. Si elle varie selon un état, les classes doivent être **mutuellement exclusives** — chaque entrée de map porte sa propre valeur.

Deux contournements quand l'exclusivité est impossible :

- **L'héritage** : poser la valeur par défaut sur le parent. Une règle directe sur l'enfant bat toujours une valeur héritée, quel que soit l'ordre du CSS. C'est ce que fait la Sidebar pour la couleur de ses liens.
- **Une prop dédiée** : `UiInput` gère son padding en interne selon `icon`, plutôt que de laisser la page passer `pl-9`.

## L'échelle d'espacement n'est PAS celle de Tailwind

`main.css` redéfinit `--spacing-0` à `--spacing-10` avec une échelle non linéaire. Tailwind utilise ces valeurs nommées en priorité et ne retombe sur son calcul `N × 4px` qu'à partir de l'indice **11**.

| classe           | valeur réelle | valeur Tailwind standard |
| ---------------- | ------------- | ------------------------ |
| `-4`             | 16px          | 16px                     |
| `-5`             | 24px          | 20px                     |
| `-6`             | 32px          | 24px                     |
| `-7`             | 40px          | 28px                     |
| `-8`             | 48px          | 32px                     |
| `-9`             | 64px          | 36px                     |
| `-10`            | **80px**      | 40px                     |
| `-11` et au-delà | N × 4px       | idem                     |

L'échelle n'est pas seulement décalée, elle est **non monotone à la frontière** : `h-10` vaut 80px mais `h-11` vaut 44px. Écrire `p-8` en pensant « 32px » se trompe de 50 %.

Pour 32 / 40 / 48px de hauteur : `h-6` / `h-7` / `h-8`.

**Les indices fractionnaires échappent à l'échelle.** `--spacing-8.5` n'existe pas, donc `size-8.5` retombe sur `calc(var(--spacing) * 8.5)` = 34px — `--spacing` n'étant pas redéfini, il vaut la valeur Tailwind par défaut de 4px. C'est la porte de sortie quand une valeur du mock ne tombe pas sur l'échelle : `gap-2.5` = 10px, `h-11.5` = 46px, `mb-0.75` = 3px.

Ce piège a été reproduit plusieurs fois, y compris après avoir été documenté. Ne pas se fier à sa mémoire : vérifier.

## Vérifier une classe dans le CSS réellement servi

En dev, `main.css` est servi comme **module JS**, donc les échappements CSS y sont doublés : la classe `.py-[3px]` apparaît sous la forme `.py-\\[3px\\]`. Chercher le sélecteur produit des faux négatifs — c'est arrivé quatre fois. Chercher la **propriété attendue** à la place :

```bash
curl -sk "https://localhost/_nuxt/app/app/assets/css/main.css" | grep -c "height: 46px"
```

Une classe inventée (`resize-vertical` au lieu de `resize-y`) ne produit aucune règle et ne déclenche ni erreur de lint ni erreur de type.

## Tokens sémantiques uniquement

Jamais de couleur en dur. Toujours les alias `@theme` : `bg-surface-card`, `text-fg-muted`, `border-line-subtle`, `text-accent`, `shadow-focus`.

Les valeurs du thème clair ne sont **pas** celles du sombre : elles ont été réassombries pour tenir un contraste ≥ 5:1 sur fond clair. Ne pas « simplifier » en réutilisant les mêmes.

`*.css` est exclu de Prettier (`.prettierignore`) : les échelles oklch sont alignées à la main sur 2 décimales, et Prettier tronque les zéros non significatifs.

## Texte posé sur un fond accent : `text-on-accent`

`--on-accent` suit `--text-inv`, donc il s'inverse avec le thème. Toute icône ou texte placé sur `bg-accent` doit l'utiliser, sinon il hérite de la couleur du corps de page et le contraste s'effondre dans l'un des deux thèmes.

Mesuré sur `BrandLogo` : **2,47:1** en sombre et **3,14:1** en clair avec l'héritage, contre **7,42:1** et **5,09:1** avec `text-on-accent` (seuil WCAG AA pour un graphique : 3:1). Le défaut existait dans les deux thèmes.

## Ne pas redéclarer une couleur déjà héritée

`main.css` pose `body { color: var(--text-body) }`. Ajouter `text-fg-body` sur un élément est **redondant** — et nuisible, puisque la classe entre en collision avec toute classe d'état qui veut changer la couleur.

Les états d'un lien passent par `active-class` et `exact-active-class` sur `NuxtLink`. Pas besoin de `useLink` ni de map d'états.

## `$t` n'existe que dans les templates

Dans un `<script setup>`, `$t` est une **variable libre** : le module compilé la référence sans jamais la définir, d'où un `ReferenceError` au premier appel. Dans un script, il faut `const { t } = useI18n()` puis `t('cle')`.

Rien ne l'attrape : ESLint passe, `vue-tsc` passe (le module i18n déclare `$t` globalement pour les templates), la page répond 200. Le défaut n'apparaît qu'à l'exécution de la fonction concernée.

## Une valeur dérivée d'une prop va dans un `computed`

`const x = MAP[props.y]` hors `computed` n'est évalué **qu'une fois** : le composant ne réagit plus aux changements de prop.

## Règles ESLint qui mordent

- **Une seule racine par page.** `vue/no-multiple-template-root` s'applique aux `pages/` uniquement — les transitions de page en dépendent.
- **`vue/multi-word-component-names` lit le nom du fichier, pas le nom résolu par Nuxt.** `ui/Button.vue` la déclenche même si Nuxt l'expose comme `<UiButton>`. D'où les fichiers préfixés (`ui/UiButton.vue`) : Nuxt dédoublonne le segment, les noms d'usage restent `<UiButton>`, et la règle reste active.
- **`vue/attributes-order`** impose `class` et les liaisons **avant** les gestionnaires : `:class`, puis `:aria-label`, puis `@click`. `pnpm lint:fix` les règle.

## i18n : aucune chaîne visible en dur

Tout texte affiché passe par `$t()` dans les templates, `useI18n().t()` dans les scripts. Les trois locales restent **synchronisées** : une clé ajoutée dans `fr.json` doit l'être dans `en.json` et `es.json`.

Clés par domaine : `nav`, `theme`, `language`, `landing`, `dashboard`, `exercices`, `exercise`, `training`, `notions`, `auth` (+ `auth.errors`, `auth.providers`), `chat`, `user`.

Stratégie `no_prefix` : les URLs ne contiennent pas la langue, elle est persistée dans le cookie `maia_locale`.

**Le `@` doit être échappé.** Dans vue-i18n, `@` introduit un _message lié_ (`@:autre.cle`). Une adresse e-mail écrite telle quelle casse la compilation — et pas seulement la clé fautive : **toute la locale est rejetée**, donc plus aucune traduction ne s'affiche. Écrire `prenom{'@'}student.42.fr`. Même précaution pour `|` (pluralisation).

Ce défaut est invisible à `JSON.parse`, à ESLint, à `vue-tsc` et à un `curl`. Il n'apparaît que dans les logs du serveur de dev. **Après toute modification des locales, lire `make logs`.**

Attention aux longueurs : `nav.trainingSession` fait 16 caractères en anglais et 23 en espagnol.

## Accessibilité

Le module WCAG 2.1 AA est une contrainte sur **chaque** composant écrit, pas une phase ajoutée à la fin.

- Focus visible obligatoire : `focus-visible:shadow-focus focus-visible:outline-none`.
- Icône décorative accompagnée d'un texte : `aria-hidden="true"`.
- Champ de formulaire : un `<label for>` associé, pas seulement un placeholder.
- Bouton sans texte visible : `aria-label` explicite.
- Bouton à bascule : `aria-pressed`.
- Préférer les éléments natifs : `LanguageSwitcher` est un `<select>`, donc navigable au clavier sans une ligne de JS.
- Un élément cliquable est un `<button>` ou un lien, **jamais** un `<div>` avec `@click`.
- Un bouton qui navigue est un lien : `<UiButton to="/exercices" …>` rend un `NuxtLink` avec le style du bouton, plutôt qu'un `<button>` qui appelle `navigateTo`.
- Animations : respecter `prefers-reduced-motion`.

⚠️ Le chronomètre du duel entre en conflit direct avec le critère **2.2.1 « Timing Adjustable »**. À traiter dès la conception de l'arène.

## Pas de commentaires explicatifs

Le code reste nu. Les justifications vont dans ce fichier ou dans les messages de commit. Le backend suit la convention **opposée** (commentaires denses expliquant les pièges de sécurité) — ne pas l'aligner sur le front, c'est délibéré des deux côtés.

## Style

Prettier : pas de point-virgule, guillemets simples, 100 colonnes. `pnpm format` avant de committer.

---

# Pièges de l'environnement

Ils ont tous coûté du temps au moins une fois.

**Le répertoire de travail persiste entre deux commandes shell.** Toujours vérifier où on est avant un `pnpm` ou un `git`.

**Ne jamais lancer `pnpm install` dans le container pendant que `pnpm dev` tourne.** pnpm réécrit les liens de `/app/node_modules` sous les pieds du serveur et le casse (`ERR_MODULE_NOT_FOUND` sur `defu`, `nuxt`…). La réparation est un `make re`.

**`node_modules`, `.nuxt` et `.pnpm-store` sont des volumes nommés**, donc distincts entre l'hôte et le container. C'est voulu : `.nuxt` contient des chemins absolus. Les volumes gardent aussi le store pnpm chaud entre deux builds — sans eux, les 780 paquets se retéléchargent à chaque démarrage. Conséquence : un paquet installé sur l'hôte n'existe pas dans le container avant un rebuild.

**Après un déplacement ou un renommage de composant, `make re`.** Le HMR de Vite ne suit pas les `git mv` : son graphe garde les anciens chemins et le navigateur reçoit un `error loading dynamically imported module`. Un rechargement dur (Ctrl+Shift+R) est nécessaire en plus.

**`curl` ne suffit pas à valider une page.** En `ssr: false`, `curl /` renvoie 200 sur une coquille HTML vide même si l'application est cassée. Pour vérifier : tester l'URL du module, lire `make logs`, ou ouvrir la page.

**pnpm bloque les scripts d'installation par défaut** (protection supply-chain depuis pnpm 10). Un paquet à binaire natif doit être autorisé dans `pnpm-workspace.yaml` → `allowBuilds`. Actuellement : `esbuild`, `unrs-resolver`. Si pnpm écrit `set this to true or false` dans ce fichier, l'install échoue jusqu'à ce que la valeur soit tranchée.

**Pas de `typescript.typeCheck` dans `nuxt.config.ts`.** Il passe par `vite-plugin-checker`, incompatible avec Vite 8, et ralentirait le HMR. Le typecheck se lance via `pnpm typecheck`.

**TypeScript reste en 5.x.** La 7.x casse `@typescript-eslint` 8, qui exige `<6.1.0`.
