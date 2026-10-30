# Modules — état réel du dépôt

Le sujet exige **14 points** (`subject.md:221`). Major = 2 pts, Minor = 1 pt.
Ce document reflète ce qui est **réellement présent sur `main`** au 30/09/2026 (merge de la PR #10 `chore/remove-dead-code`, commit `eb3e75f`), vérifié fichier par fichier — pas ce qui est prévu dans `PLAN.md`. Les deux documents divergent volontairement : `PLAN.md` est la feuille de route, celui‑ci est le constat.

> Méthode : chaque ligne « acquis » a été vérifiée dans le code (route, modèle, écran), et le jeu a été joué de bout en bout dans Chromium à deux joueurs. Un module sans route/écran vérifiable est classé « non commencé », même s'il apparaît dans `PLAN.md`.

## Score actuel : **21 / 14 points**

| | Majeurs | Mineurs | Points |
|---|---|---|---|
| Acquis | 8 | 5 | **21** |

Marge de 7 points au‑dessus du seuil. Les 6 points du jeu reposent sur un quiz : c'est un jeu recevable (règles, chrono, score, gagnant), mais c'est aussi le point le plus susceptible d'être discuté en soutenance — même sans eux, le total reste à 15.

Les deux exigences qui bloquaient le projet (README et pages légales) sont levées. Il reste des points à corriger avant la soutenance, voir « À corriger avant la soutenance ».

---

## Acquis — 21 points

### Majeurs (8 × 2 = 16 pts)

| Module | Section | Preuve |
|---|---|---|
| Framework frontend + backend | IV.1 | Nuxt 4 ([nuxt.config.ts](srcs/frontend/nuxt.config.ts)) + FastAPI ([main.py](srcs/backend/app/main.py)) |
| Temps réel WebSocket | IV.1 | [ws.py](srcs/backend/app/api/ws.py) + `ConnectionHub` ([realtime.py](srcs/backend/app/services/realtime.py)) : présence multi‑onglets, ping/pong, relai de frappe, broadcast aux amis, salles de match |
| Interaction utilisateur (chat + profil + amis) | IV.1 | Chat ([chat.py](srcs/backend/app/api/v1/chat.py)), profil ([users.py](srcs/backend/app/api/v1/users.py)), amis ([friends.py](srcs/backend/app/api/v1/friends.py)) |
| Gestion utilisateur standard | IV.3 | Modification profil, avatar ré‑encodé + identicon ([media.py](srcs/backend/app/services/media.py)), amis + statut en ligne (`hub.is_online`), page profil |
| Interface LLM — la correction des réponses rédigées | IV.4 | Moteur Ollama joint par Tailscale ([llm/](srcs/backend/app/services/llm/)), verdict structuré puis explication en flux NDJSON, repli en auto‑évaluation, limites (3 essais/question, débit par élève, budget de tokens). Route [attempts.py](srcs/backend/app/api/v1/attempts.py), écran [ExerciseGrader.vue](srcs/frontend/app/components/exercise/ExerciseGrader.vue) |
| **Jeu web complet** — quiz multijoueur | IV.6 | Règles affichées, 20 s par question, 10 questions par partie, classement en direct, gagnant et écran de fin. Barème : 10 pts par bonne réponse + 5/3/1 aux trois premiers à la trouver. **Pause de révélation de 4 s entre deux questions** : la bonne réponse n'est nommée qu'une fois la question close pour tout le monde, et c'est le serveur qui tient cette pause, pas les clients ([realtime.py](srcs/backend/app/services/realtime.py)). **Résumé de fin de partie** manche par manche — la réponse de chaque joueur, sa justesse, son temps et ses points — réservé aux participants d'un match terminé ([matches.py](srcs/backend/app/api/v1/matches.py)). Écran [multiplayer.vue](srcs/frontend/app/pages/multiplayer.vue) et [components/multiplayer/](srcs/frontend/app/components/multiplayer/) |
| **Joueurs distants** | IV.6 | Serveur seul maître du chrono et du score. Déconnexion : 60 s de grâce, puis forfait — **écrit en base**, comme toute fin de partie ; au retour, `match.state_sync` rend la question en cours, le temps restant, le classement et, pendant la pause, la bonne réponse ([ws.py](srcs/backend/app/api/ws.py), [useMultiplayer.ts](srcs/frontend/app/composables/useMultiplayer.ts)). Joueurs marqués « Reconnexion… » dans la salle. **Mise en place réseau** : `SERVER_NAME` reçoit l'adresse de la machine hôte, l'entrypoint nginx régénère alors le certificat en la posant en `IP:` ([entrypoint.sh](srcs/nginx/entrypoint.sh)), les autres joueurs ouvrent `https://<adresse>:8443` |
| **Multijoueur (3 joueurs et plus)** | IV.6 | Salles de 2 à 8 joueurs, même question envoyée à tous, partie lancée quand tous sont prêts. **Le créateur choisit les niveaux d'exam tirés** — un, plusieurs, ou tous ([matches.py](srcs/backend/app/api/v1/matches.py) `GET /matches/topics`, [MultiplayerSetup.vue](srcs/frontend/app/components/multiplayer/MultiplayerSetup.vue)). Couvert par [test_ws_multiplayer.py](srcs/backend/tests/test_ws_multiplayer.py), [test_multiplayer.py](srcs/backend/tests/test_multiplayer.py) et [test_match_questions.py](srcs/backend/tests/test_match_questions.py) |

### Mineurs (5 × 1 = 5 pts)

| Module | Section | Preuve |
|---|---|---|
| ORM | IV.1 | SQLAlchemy 2.0 async ([db.py](srcs/backend/app/db.py)), 14 migrations Alembic, 24 tables |
| i18n — 3 langues | IV.2 | fr/en/es complets (430 clés chacun), sélecteur, détection + cookie. **Le contenu lui‑même est traduit** : chaque question et chacune de ses propositions ont leurs trois versions |
| Design system sur mesure | IV.1 | 15 composants `Ui*` + 46 composants au total, tokens CSS dédiés ([maia.token.css](srcs/frontend/app/assets/css/maia.token.css)) — au‑dessus du minimum de 10 |
| 2FA (TOTP) | IV.3 | Setup QR (émetteur « Maia ») + secret chiffré, codes de récupération, vérification au login, désactivation ([auth.py](srcs/backend/app/api/v1/auth.py) `/2fa/*`) |
| **OAuth 2.0** | IV.3 | Google, GitHub et 42 ([oauth.py](srcs/backend/app/services/oauth.py)), e‑mails vérifiés seulement, boutons sur les écrans de connexion ([AuthProviders.vue](srcs/frontend/app/components/auth/AuthProviders.vue)), [test_oauth.py](srcs/backend/tests/test_oauth.py). Les clés de chaque fournisseur sont à renseigner dans `srcs/.env` |

### Le contenu, qui porte à la fois l'IA et le jeu

**157 questions** dans des fichiers TOML relus en pull request ([seeds/content/](srcs/backend/seeds/content/)), chargées au démarrage de l'API quand `SEED_DEMO_DATA=true` (valeur par défaut de `.env.example`) :
- **127 questions rédigées** (`free_text`), 5 rangs, 11 niveaux — corrigées par le LLM, visibles dans Exercices et l'entraînement. **Chacune porte aussi ses quatre propositions** (une correcte, trois erreurs classiques, en fr/en/es) : la même question sert la correction rédigée *et* le duel, soit **508 propositions** et un vivier de 127 questions jouables.
- **30 QCM** (`single_choice`, [multiplayer-quiz.toml](srcs/backend/seeds/content/multiplayer-quiz.toml)) écrits d'abord pour le quiz, **tous archivés** (`status = "archived"`) depuis que les questions rédigées portent leurs propres propositions : chaque notion n'est posée qu'une fois. Le fichier reste versionné, un mot suffit à en réactiver une.

⚠️ La grille de correction n'est **jamais** envoyée au modèle avec les propositions : le duel et le correcteur lisent la même question mais pas les mêmes champs.

Jamais un texte de sujet d'exam, seulement les notions.

---

## Exigences obligatoires — levées

| Exigence | Constat |
|---|---|
| **Politique de confidentialité et CGU** (III.2) | Pages [privacy.vue](srcs/frontend/app/pages/privacy.vue) et [terms.vue](srcs/frontend/app/pages/terms.vue), traduites, liées depuis le pied de page ([AppFooter.vue](srcs/frontend/app/components/AppFooter.vue)) et depuis l'inscription. ⚠️ La politique promet la suppression et l'export des données, mais aucune route ne le permet (voir plus bas) |
| **README.md** (III.1) | [README.md](README.md) à la racine, avec les sections imposées, l'équipe, et la justification des 13 modules réclamés |

---

## À corriger avant la soutenance

Relevés par l'audit du 30/09/2026.

| Priorité | Point | Constat |
|---|---|---|
| 🔴 | Blocage contournable | Un utilisateur bloqué peut « bloquer » en retour, prendre la main sur la ligne, puis lever le blocage ([friends.py](srcs/backend/app/api/v1/friends.py) `POST /friends/block`) |
| 🔴 | OAuth contourne la 2FA | Le callback OAuth ouvre une session complète sans regarder `totp_enabled` ([auth.py](srcs/backend/app/api/v1/auth.py) `oauth_callback`) |
| 🔴 | Connexions Postgres | Chaque WebSocket ouverte garde sa session, donc une connexion du pool (30 au plus) : une trentaine d'onglets suffisent à bloquer l'API ([ws.py](srcs/backend/app/api/ws.py)) |
| 🟠 | Lint frontend | `pnpm lint` échoue : 3 erreurs dans [plugins/api.ts](srcs/frontend/app/plugins/api.ts) |
| 🟠 | Droits promis par la politique | Suppression et export des données annoncés, aucune route `DELETE /users/me` ni export |
| 🟠 | Mot de passe oublié | [forgotten-password.vue](srcs/frontend/app/pages/forgotten-password.vue) est liée depuis la connexion mais ne fait rien |
| 🟠 | Langue des questions en multijoueur | Tirées dans la langue du créateur pour toute la salle ; le sélecteur du header ne change pas `user.locale` |
| 🟠 | Responsive (III.3) | La sidebar a une largeur fixe (`w-64`) sans point de rupture |
| 🟠 | Reconnexion WebSocket | Aucun rafraîchissement du jeton avant de se reconnecter : après 15 min, une coupure en pleine partie peut finir en forfait |
| 🟡 | Console sans erreur (III.2) | Build prod vérifié sans erreur de CSP. Pas d'audit page par page au‑delà |
| 🟡 | `make re` | Efface la base (`down -v`). Sans `SEED_DEMO_DATA=true`, plus d'exercices ni de quiz |
| 🟡 | Écart modèles / migrations | `alembic check` signale une contrainte unique de `match_attempts` |

Réglé depuis le 28/09 : l'accès par `:8443` ne perd plus le port (`absolute_redirect off` dans [nginx.conf](srcs/nginx/nginx.conf)), le code mort a été retiré, et mypy passe sans erreur (branche `chore/remove-duplicates`, pas encore mergée).

---

## Construit, mais pas réclamable en l'état

| Module | Ce qui existe | Ce qui manque |
|---|---|---|
| Recherche avancée (IV.1, Minor) | Backend complet : filtres, tri, pagination, facettes, trigrammes ([search.py](srcs/backend/app/services/search.py)) | **Le paramètre `q` n'est jamais envoyé** par le front ([useQuestions.ts](srcs/frontend/app/composables/useQuestions.ts)) : la recherche se fait dans le navigateur, sans tolérance aux fautes ni facettes. ~20 lignes pour prendre le point |
| Statistiques et historique de match (IV.3, Minor) | Chaque partie est enregistrée : `matches`, `match_players` (score, rang final), `match_questions`, `match_attempts` ([match_persistence.py](srcs/backend/app/services/match_persistence.py)) | Aucune route ni aucun écran ne les lit : pas d'historique, pas de victoires/défaites, pas de classement général |
| Chat avancé (IV.6, Minor) | Blocage (qui interdit les messages), historique persistant, indicateur de frappe, accusés de lecture | Pas d'invitation à une partie depuis le chat, pas de notification de match, pas de lien vers le profil depuis la conversation |
| Gamification (IV.6, Minor) | Mergée sur `main` : XP et niveaux, série de jours, succès (tables `achievements` / `user_achievements`), `record_activity` appelé après chaque réponse et chaque partie ([gamification.py](srcs/backend/app/services/gamification.py)), règles exposées par `GET /gamification/rules`, barre d'XP et succès sur le tableau de bord, toasts au déblocage | Pas encore réclamé dans le README. Le sujet demande 3 éléments de sa liste : succès et XP/niveaux y sont, la série de jours n'y figure pas. Un **classement** (les XP sont déjà en base) fournirait le troisième sans discussion |
| API publique (IV.1, Major) | Routes questions et thèmes avec pagination ([questions.py](srcs/backend/app/api/v1/questions.py)), rôles déjà en place | Pas de clé d'API, pas de limite de débit dédiée, pas d'écriture (`PUT`/`DELETE`) sur le contenu, pas de documentation publique distincte de `/api/docs` |

---

## Non commencé

| Catégorie | Constat |
|---|---|
| Tournoi, personnalisation, spectateur, adversaire IA (IV.6 / IV.4) | Rien — tous deviennent possibles maintenant que le jeu existe |
| **Artificial Intelligence (IV.4)** — RAG, recommandation | L'interface LLM est prise ; le RAG est un module distinct, hors périmètre assumé |
| **User Management (IV.3)** — permissions avancées, organisations | `Role` (guest→admin) existe dans le modèle, aucun endpoint d'administration |
| **Notifications (IV.1, Minor)** | Aucune table, aucun canal WS dédié |
| **Accessibility (IV.2)** — WCAG 2.1 AA, RTL, navigateurs additionnels | Focus visible, rôles ARIA, `aria-live` sur le jeu, mouvement réduit, durée réglable, mais aucun audit d'ensemble |
| **Cybersecurity (IV.5)**, **Devops (IV.7)**, **Data & Analytics (IV.8)**, **Blockchain (IV.9)** | Rien |

---

## Branches distantes

Au 30/09/2026, par rapport à `origin/main` :

| Branche | État |
|---|---|
| `origin/chore/remove-duplicates` | En avance, à merger : doublons retirés (`GET /friends/requests`, constantes OAuth), typage corrigé, émetteur 2FA « Maia » |
| `origin/game-engine` | 6 commits d'avance, 47 de retard |
| `origin/gamification` | 1 commit d'avance, 46 de retard : contenu repris sur `main`, à supprimer |
| `origin/user_management` | 8 commits d'avance, 64 de retard |
| `origin/chore/remove-dead-code`, `origin/connection_security`, `origin/feat/exercices-and-llm`, `origin/feat/multiplayer-enhance`, `origin/user_management-gamification` | Mergées, sans commit propre : à supprimer |

---

## Ce qui reste à faire, par priorité

1. **Les trois points 🔴** de « À corriger avant la soutenance » : blocage, 2FA via OAuth, connexions Postgres.
2. **Lint frontend** et **droits de la politique de confidentialité** (route de suppression de compte, ou texte reformulé).
3. **Sidebar responsive** (exigence III.3, pas un module) et page **mot de passe oublié** (implémenter ou retirer le lien).
4. **Gamification** : ajouter un classement et la réclamer dans le README — **+1 pt**.
5. **Recherche avancée** : envoyer `q` au serveur — **+1 pt** pour ~20 lignes.
6. **Historique de match** : une route `GET /users/{id}/matches` et une section sur le profil — **+1 pt**, les données sont déjà là.
7. **Chat avancé** : invitation à une partie depuis le chat + lien vers le profil — **+1 pt**.
