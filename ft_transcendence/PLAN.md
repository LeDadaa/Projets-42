# Plan de construction

Une entrée par module **restant**. Les quatre modules déjà validés — framework, design system, i18n, ORM — n'y figurent pas ; ils sont dans `MODULES.md` avec leur preuve.

Chaque entrée donne : ce que le sujet exige mot pour mot, ce qui existe déjà, les specs techniques, et **comment le démontrer en soutenance**. Un module qu'on ne sait pas montrer en trois minutes compte pour 0 (`subject.md:258`).

Conventions et pièges : `srcs/backend/BACKEND.md` · `srcs/frontend/FRONTEND.md`.

---

## Ordre d'exécution

Les dépendances sont réelles, pas indicatives : le sujet conditionne explicitement plusieurs modules à l'existence d'un jeu.

```
  0. Réconcilier les deux historiques         ← bloque tout le backend
  1. Fondations d'intégration API             ← bloque tout le frontend
  2. Gestion utilisateur                       (parallèle avec 3)
  3. Temps réel WebSocket                     ← bloque le jeu
  4. Jeu → Joueurs distants → Multijoueur 3+  ← un seul moteur, 6 points
  5. Correcteur LLM                            (parallèle depuis l'étape 1)
  6. 2FA (finition) · Gamification
  7. Réserve : recherche (front) → navigateurs → tournoi → WCAG → API publique
```

---

## 0. Préalables — aucun code

### Deux historiques indépendants à réconcilier — la tâche n°1

Le dépôt `alyssiagalleze/Transcendence` contient un commit `7e67fae` **sans aucun ancêtre commun** avec le `main` local. Aucun des deux n'est un sur-ensemble de l'autre :

| Côté | Ce qu'il a en propre |
|---|---|
| **Dépôt distant** | Tout le domaine **contenu** : 11 tables (`topics`, `questions`, `choices`, leurs traductions, `tags`, `learning_paths`), les 7 routes de rédaction et modération (`questions.py`), le service de **recherche full-text** (GIN + trigrammes + facettes + pagination), l'**i18n backend**, `totp.py`, `workflow.py`, `validation.py`, `audit_log.py`, `recovery_code.py`, un paquet `seeds/`, les tests `test_i18n` et `test_roles`, et 2 migrations |
| **`main` local** | Toute la **2FA branchée** (`backup_code.py`, `schemas/auth.py`, sa migration, les 3 routes `/2fa/*` — le distant n'a que 5 routes dans `auth.py` contre nos 8), la suite de tests `conftest.py`, les 4 documents, `.env.example`, et **la purge des secrets** |

⚠️ **Le dépôt distant versionne `srcs/.env`, `secrets/POSTGRES_PASSWORD` et `secrets/POSTGRES_USER`** — la règle `subject.md:186` y est violée. Les valeurs exposées sont les anciennes, déjà régénérées, donc inutilisables ; mais la purge est à refaire de ce côté avant que le dépôt ne grossisse.

**Conséquence sur le périmètre :** le domaine contenu et la recherche serveur, que ce plan donnait comme à construire, **existent déjà**. Les entrées concernées ci-dessous en tiennent compte.

Ce qui reste absent **des deux côtés** : `attempts`, `user_topic_mastery`, `user_question_state` (progression), `matches` / `match_players` / `match_questions` (jeu), `achievements` (gamification), `notifications`, toute la couche temps réel et toute la couche LLM.

### Le jeu est validé

L'administration laisse l'interprétation à notre charge : **c'est à nous de défendre que le duel de quiz est un jeu au sens de IV.6.** L'équipe a tranché, la partie jeu est au périmètre — les 7 points (3 majeurs + le tournoi) sont fermes.

Ce n'est plus un risque de périmètre, c'est une **exigence de soutenance**. L'argumentaire porte sur les mécaniques et doit être matérialisé dans le produit : une fenêtre de règles dans l'application et la même liste dans le README. Un jeu dont les règles ne sont écrites nulle part se défend mal.

**Mettre le `README.md` à jour** — c'est encore le starter Nuxt. Le sujet impose des sections précises, dont une *Team Information* par personne et les instructions `.env`.

---

## 1. Fondations d'intégration API — prérequis, 0 point

Aucun module ne les rapporte, tous en dépendent. Une seule personne, ~2 jours.

**À créer** dans `srcs/frontend/` :

| Fichier | Rôle |
|---|---|
| `app/plugins/api.ts` | wrapper `$fetch` : `credentials: 'include'`, header `X-CSRF-Token`, intercepteur 401 |
| `app/composables/useAuth.ts` | `login`, `register`, `logout`, `me`, état utilisateur |
| `app/middleware/auth.ts` | protection des pages du layout `dashboard` |
| `app/types/api.ts` | types alignés sur `schemas/user.py` |
| `shared/validation.ts` | règles dupliquées front/back |
| `nuxt.config.ts` | `runtimeConfig.public` |

**Specs, toutes vérifiées dans le code backend :**

- **URLs relatives uniquement.** nginx sert tout depuis `https://localhost` en dev comme en prod. Même origine, pas de CORS.
- **`credentials: 'include'` sur chaque appel**, sinon les cookies ne partent pas.
- **Header `X-CSRF-Token`** sur toute méthode non-safe, valeur lue du cookie `csrf_token` (le seul des trois lisible par JS). Sans lui : **403 sur tous les POST, PATCH, DELETE**. Exemptés : `/auth/login`, `/auth/register`, `/auth/oauth/`.
- **Intercepteur 401 → `POST /auth/refresh` → rejeu.** L'access token expire en **15 minutes**.
- ⚠️ **Sérialiser le refresh derrière une promesse unique partagée.** Le backend fait de la rotation par familles avec détection de réutilisation : deux `/refresh` concurrents révoquent toute la famille et déconnectent l'utilisateur.

**Validation à dupliquer** (le sujet exige les deux côtés) :

| Règle | Valeur backend |
|---|---|
| Mot de passe | 10 à 128 caractères |
| `display_name` | `^[a-zA-Z0-9_-]{3,24}$` — **le front ne le valide pas** |
| Mots de passe courants | liste noire de 8 entrées |
| Mot de passe ≠ email | `model_validator` |
| Conflit | 409 + `"email already used"` / `"display name already used"` — **pas de clé i18n** |

---

## 2. Gestion utilisateur standard — Major, 2 pts

**Le sujet exige** (IV.3) : mise à jour du profil · upload d'avatar avec un avatar par défaut · ajout d'amis **et statut en ligne** · page de profil.

**Existe** : les quatre routes backend (`PATCH /users/me`, avatar, amis) et l'identicon par défaut.

**À construire :**

- Brancher `login.vue`, `sign-up.vue`, `forgotten-password.vue` sur l'API.
- `profil.vue` : informations, avatar, liste d'amis.
- `settings.vue` : `PATCH /users/me` (`display_name`, `bio`, `locale`).
- **Bouton de déconnexion dans la Sidebar** — `POST /auth/logout` existe, aucun bouton ne l'appelle.
- **Le statut en ligne**, seul point qui manque côté backend : `friends.py` renvoie `"online": False` en dur. Il demande le `ConnectionHub` du module 3 — donc à finir **après** le temps réel, avec `last_seen_at` en repli pour afficher « vu il y a 5 min ».
- Upload : 2 Mo maximum, limite posée par nginx **et** par `media.read_limited`.

**Démonstration** : créer un compte, se connecter, changer d'avatar et de bio, ajouter un ami depuis un second navigateur, **voir son statut passer en ligne**, se déconnecter. Laisser la session ouverte **plus de 15 minutes** et continuer à naviguer sans reconnexion — c'est la preuve que l'intercepteur de refresh fonctionne.

---

## 3. Temps réel WebSocket — Major, 2 pts

**Le sujet exige** (IV.1) : mises à jour temps réel entre clients · gestion propre des connexions et déconnexions · diffusion efficace.

**Existe — mergé** (PR #1) : le `ConnectionHub` (`app/services/realtime.py`), l'endpoint `/api/ws` authentifié par cookie, la présence des amis ; côté front, `useRealtime` (reconnexion à backoff exponentiel, ping avec mesure de latence) et `usePresence`. nginx proxifie l'upgrade avec `proxy_read_timeout 3600s`. `MODULES.md` compte le module comme acquis.

**Reste à construire** pour le jeu :

```
protocol.py   les trames en classes Pydantic — aujourd'hui les messages sont des dict non validés
salons        diffuser à un groupe de joueurs, pas seulement aux amis
engine.py     MatchEngine (module 4)
```

**Specs :**

- **Un seul endpoint**, `GET /api/ws`, authentifié par le cookie lu pendant la poignée de main.
- **Forme des messages** : un objet JSON plat, clé `type` puis les données au même niveau — `{ "type": "presence", "user_id": 31, "online": true }`. Convention posée par le code existant, détaillée dans `BACKEND.md`. Une liste doit être nommée (`standings`, `podium`) : un message JSON ne peut pas être une liste.
- Validation Pydantic à la réception. Un `type` inconnu répond un message `error`, **jamais** une exception.
- **Heartbeat** : `ping` toutes les 20 s, plus un `ping`/`pong` applicatif — certains proxys avalent les trames de contrôle et provoquent des déconnexions fantômes.
- Côté client : `wss://${location.host}/api/ws`. La CSP n'autorise que `connect-src 'self' wss://$host`.
- Le `ConnectionHub` alimente le statut en ligne du module 2.

⚠️ L'état vit en mémoire d'**un seul** worker (`--workers 1`). Le mettre derrière un protocole `MatchStore` avec une implémentation `InMemoryMatchStore`, et documenter dans le README que `RedisMatchStore` + pub/sub est le chemin de scaling. C'est la question que posent les évaluateurs.

**Démonstration** : deux navigateurs, une action dans l'un visible instantanément dans l'autre ; couper le réseau et montrer la reconnexion automatique.

---

## 4. Le jeu — Major ×3, 6 pts — **construit**

**Trois modules, un seul moteur.** Le moteur existe : `ConnectionHub` ([realtime.py](srcs/backend/app/services/realtime.py)) pour l'état des salles, [ws.py](srcs/backend/app/api/ws.py) pour le protocole, [match_persistence.py](srcs/backend/app/services/match_persistence.py) pour ce qui reste en base. La preuve module par module est dans `MODULES.md` ; cette section ne garde que **ce qui diverge du plan d'origine** et **ce qu'il reste à faire**.

### Ce qui a été construit autrement que prévu

Le plan décrivait un barème `1000 × bonne_réponse + arrondi(500 × temps_restant / temps_total)`, des multiplicateurs de série et trois jokers. **Rien de tout cela n'existe.** Le barème réel est plus simple et se lit sans explication : **10 points par bonne réponse, plus 5, 3 et 1 aux trois premiers à la trouver**. Pas de jokers, pas de multiplicateur de série, pas d'élimination progressive en multijoueur.

Ce n'est pas un manque : le sujet demande des règles claires et une condition de victoire, pas une mécanique riche. Un barème qu'on explique en une phrase se démontre mieux en soutenance qu'un barème qu'on doit justifier.

Deux choses ont en revanche été ajoutées, absentes du plan :

- **Une pause de révélation de 4 s entre deux questions** (`REVEAL_SECONDS`). Sans elle on enchaînait les questions sans jamais apprendre si l'on avait juste. Le serveur tient cette pause, comme il tient le chrono : côté client, elle aurait mordu sur le temps de réponse de la question suivante et les écrans auraient dérivé. La bonne réponse n'est diffusée qu'une fois la question close pour tout le monde.
- **Un résumé de fin de partie** manche par manche, réservé aux participants d'un match **terminé** — servi en cours de partie, il donnerait les réponses restantes.

Et le créateur d'une salle **choisit les niveaux d'exam** tirés : un, plusieurs, ou tous.

### 4b. Joueurs distants — ce qu'il reste

Le code est fait : chrono et score côté serveur, 60 s de grâce à la déconnexion, `match.state_sync` au retour, forfait écrit en base. **Il ne reste que la démonstration**, qui n'est pas du code.

**Mise en place, sur un réseau local** — la machine qui héberge renseigne son adresse :

```bash
# srcs/.env
SERVER_NAME=192.168.x.y            # `ip -4 addr`
ALLOWED_ORIGINS=https://192.168.x.y:8443
make
```

Les autres joueurs ouvrent `https://192.168.x.y:8443`. L'entrypoint nginx régénère le certificat dès que `SERVER_NAME` change et pose une adresse en `IP:` — écrite en `DNS:`, aucun navigateur ne la reconnaît. Le certificat reste auto-signé, l'avertissement est normal.

⚠️ Trois pièges, tous hors du code :

- Les cookies de session sont `Secure` : **pas de raccourci en `http://`**, la connexion échouerait sans message.
- Certains réseaux Wi-Fi isolent les clients entre eux. À tester dans la salle, pas le jour de la soutenance.
- Les URL de retour OAuth sont déclarées **par hôte** : la connexion par Google, GitHub ou 42 ne marchera que pour l'hôte déclaré. La connexion par mot de passe, elle, marche partout.

**Démonstration** : deux machines réelles, pas deux onglets. Couper le réseau d'un joueur en pleine partie, le laisser revenir, montrer qu'il reprend à la bonne question avec le bon temps restant. C'est le cœur de la note.

### 4c. Multijoueur 3+ — ce qu'il reste

Salles de 2 à 8, même question au même instant, partie lancée quand tout le monde est prêt. L'élimination progressive prévue au plan **n'a pas été écrite** et n'est pas nécessaire : le sujet demande 3 joueurs simultanés et une synchronisation correcte.

**Démonstration** : une partie à 4, en montrant que les quatre écrans changent de question au même instant — et que la pause de révélation tombe au même instant elle aussi.

⚠️ L'état vit en mémoire d'**un seul** worker (`--workers 1`). C'est la question que posent les évaluateurs : la réponse est qu'un `MatchStore` derrière un protocole, avec une implémentation Redis + pub/sub, est le chemin de scaling.

---

## 5. LLM system interface — Major, 2 pts

**Le sujet exige** (IV.4, `subject.md:375-377`), trois critères notés séparément : **génération de texte** à partir d'une entrée utilisateur · **streaming géré correctement** · **gestion d'erreurs et rate limiting**.

⚠️ **Ce n'est pas le majeur RAG**, qui est distinct (`subject.md:370-373`) et exige des questions libres sur un grand corpus. Voir la décision ouverte dans `MODULES.md`.

### Ce qu'on construit

L'élève **rédige** sa réponse à un exercice ; le modèle la corrige avec nuance :

| Réponse | Retour |
|---|---|
| Juste, claire, précise | validée, on passe à la suite |
| Juste mais imprécise | validée, **plus une explication** sur ce qui manquait |
| À côté, ou « je ne sais pas » | oriente et explique selon ce qu'il a écrit |

Pas de champ de question libre, pas d'historique de conversation, pas de fil de discussion. Le LLM n'intervient qu'en mode entraînement.

### État : construit

- ✅ **La grille de correction** — `questions.key_points`, une seule grille par question quelle que soit la langue de l'élève, jamais renvoyée à un élève. Chargée avec le reste du contenu par `python -m seeds`, depuis les fichiers TOML.
- ✅ **La table `attempts`** (migration `f7e2a6c1d9b3`), avec ses quatre colonnes d'accroche (`raw_response`, `grader`, `grader_meta`, `grader_confidence`). `grader` gagne la valeur `self_assessed` pour le repli. `match_id` viendra avec la table `matches`.
- ✅ **Des questions `free_text`** — pilote de 12 questions sur l'Exam Rank 05, niveau 0 (`srcs/backend/seeds/content/`). Les autres rangs et niveaux restent à écrire.
- ✅ **Le correcteur** — `app/services/llm/`, la route `POST /api/v1/questions/{id}/attempts` en flux, les trois limites, le repli en auto-évaluation, et le front (page d'exercice et session d'entraînement). Détail, contrat des événements et banc d'essai : `srcs/backend/BACKEND.md`, « La couche IA ».

### Specs, telles que construites

- **Sortie structurée obligatoire**, contrainte par schéma : `addresses_grader`, `false_statement`, `score`, `outcome` (et `confidence`, demandé mais jamais lu). **Le modèle constate, le code tranche** : un texte adressé au correcteur ou une affirmation fausse ne sont jamais validés, quelle que soit la conclusion du modèle.
- **Pas de seuil de confiance** : tous les modèles testés répondaient 1,0 à chaque fois. L'erreur résiduelle mesurée est du côté indulgent, ce qui était le but du seuil.
- **`POST` et flux NDJSON**, et non `GET` + `EventSource` comme prévu : la réponse de l'élève serait sinon dans l'URL, donc dans les journaux de nginx. Le jeton CSRF part comme pour toute écriture.
- **Verdict d'abord, explication ensuite**, streamée : premier mot en 0,8 s sur le GPU.
- **Trois limites** : 3 essais par question sur 24 h glissantes ; un débit par élève ; un budget horaire global de tokens, lu dans les comptes renvoyés par Ollama.
- **Repli en auto-évaluation** quand `LLM_ENABLED=false` (le défaut), quand le budget est épuisé ou quand la machine ne répond pas : l'élève lit la correction rédigée et dit s'il avait juste.

### Où tourne le modèle

Sur **le PC de Jules** (GPU 16 Go), que chaque instance du projet joint par **Tailscale** : un conteneur `tailscale` en mode userspace sert de proxy aux seuls appels du modèle. Le `Makefile` ne l'active que si `secrets/TS_AUTHKEY` existe ; sans clé, la stack démarre en auto-évaluation.

✅ **Configuré et vérifié** (18/09/2026) :

- **Politique du tailnet** : `tag:maia` ne joint que le port 11434 du PC ; les ports 22, 445 et 3389 sont refusés.
- **Clé** : réutilisable, éphémère, étiquetée `tag:maia`, dans `secrets/TS_AUTHKEY`. Elle expire au bout de 90 jours ; chacun la dépose dans son propre `secrets/` et ajoute les lignes `LLM_` de `srcs/.env.example` à son `.env`, avec l'IP Tailscale du PC (`LLM_BASE_URL=http://100.103.221.34:11434`).
- **Pare-feu Windows** : le port 11434 est bloqué sur la carte réseau locale ; seuls Tailscale et WSL y accèdent.
- **Vérifié de bout en bout** : correction et flux à travers le proxy, et dans l'application.

Le modèle en conteneur prévu à l'origine a été abandonné au banc d'essai : 13 à 40 s par verdict sur processeur pour un 8–9 B, contre 2 s sur le GPU.

⚠️ **La démonstration dépend de cette machine** : allumée, Ollama lancé, GPU libre, et Tailscale joignable depuis le réseau de l'école — **à vérifier tôt sur place**.

### Validé avant d'écrire

Le banc d'essai prévu a été mené et élargi : 20 réponses types (dont 7 tentatives d'injection et une réponse légitime piégée), cinq modèles, six tours. Retenu : **`gemma4:12b`**, 19 verdicts justes sur 20, aucune réponse validée à tort, aucune refusée à tort, 7 injections sur 7 refusées, verdict en 1,9 s. Détail dans `BACKEND.md`.

### L'écran, tel que construit

La correction se lit **comme une conversation** : la réponse de l'élève à droite, celle de Maïa à gauche avec l'explication qui s'écrit au fil de l'eau, une pilule de verdict sous chacune, et la saisie détachée de l'encadré de la question. Les trois essais restent dans le fil. Un indicateur au-dessus annonce les essais restants, **lus sur le serveur** (`GET /questions/{id}/attempts`) à l'ouverture : la limite n'est jamais supposée côté front.

L'entraînement se règle en deux étapes — l'exam et le niveau dans un carrousel, puis le nombre de questions et la durée plafond, « sans limite » comprise — et se termine par un bilan. Le chrono et la position survivent à un rechargement.

### Reste à faire

- **Tailscale depuis le réseau de l'école**, sur un poste du cluster : à vérifier tôt.
- **Renouveler la clé** si la soutenance tombe après son expiration (90 jours).
- **Relire le fil après un rechargement** : les essais sont en base, mais l'explication de Maïa n'y est pas stockée. Une colonne `feedback` sur `attempts` et une lecture des essais passés (~95 lignes, une migration) — à faire après le merge du moteur de jeu, pour ne pas croiser sa migration.

### Démonstration

Rédiger une réponse juste mais vague et montrer qu'elle est **validée avec une nuance**, pas rejetée. Puis une réponse à côté, et montrer que l'explication porte sur ce qui a été écrit. Montrer la réponse **qui s'affiche au fil de l'eau**. Arrêter Ollama et montrer le repli propre en auto-évaluation. Épuiser les trois essais et montrer le refus. Enfin, tenter l'injection de prompt devant l'évaluateur — c'est ce qui distingue une intégration réfléchie d'un appel d'API.

## 6. Modules courts

### 2FA (TOTP) — Minor, 1 pt

**Le sujet exige** : un système 2FA complet pour les utilisateurs.

**Existe** : tout le backend est mergé côté `main` local — `/auth/2fa/setup`, `/2fa/enable`, `/2fa/verify`, secret chiffré au repos, codes de secours hachés, rate limiter dédié.

⚠️ Le dépôt distant porte une implémentation **concurrente** : un `services/totp.py` et un modèle `recovery_code.py`, mais aucune route branchée (son `auth.py` n'a que 5 routes). À la réconciliation, décider laquelle survit — la nôtre est câblée de bout en bout, la leur ne l'est pas.

**À construire** : l'écran d'activation dans `settings.vue` (QR code depuis `TwoFASetupResponse`, saisie du code, affichage unique des codes de secours), et l'étape de vérification dans le flux de connexion.

**Démonstration** : activer, scanner avec une app d'authentification, se déconnecter, se reconnecter avec le code, puis se reconnecter avec un code de secours.

### Gamification — Minor, 1 pt

**Le sujet exige** : au moins **3** mécaniques parmi succès, badges, classements, XP/niveaux, défis quotidiens, récompenses · **persistance en base** · retour visuel · règles de progression claires.

**Existe** : l'affichage seulement. XP, niveau et série viennent de `utils/demo.ts` — **rien n'est persisté, donc le module vaut 0 en l'état**.

**À construire** : tables `achievements` / `user_achievements`, XP gagné en fin de match, calcul de niveau, série quotidienne. Trois mécaniques bien faites valent mieux que six bâclées — le sujet le dit explicitement.

**Démonstration** : gagner de l'XP en jouant, **redémarrer la base**, montrer que l'XP est toujours là. C'est la persistance qui est notée.

---

## 7. Réserve — 9 points

À ramener si l'équipe est en avance, ou **en remplacement** si le jeu est refusé au point 0.

### Recherche avancée — Minor, 1 pt · **le point le moins cher du projet**

**Exige** : filtres, tri, **pagination**.

**Le backend est déjà écrit** (`services/search.py`, 197 lignes) : recherche full-text avec index GIN, correspondance par trigrammes `pg_trgm` pour tolérer les fautes de frappe, facettes par difficulté, et pagination.

**Où on en est** : le front lit bien l'API (`useQuestions` demande tri et pagination au serveur), et la recherche par notion vit sur la page d'un exam, limitée à ses exercices. Mais **le texte cherché est filtré dans le navigateur** : le paramètre `q` de l'endpoint n'est jamais utilisé, donc ni la tolérance aux fautes de frappe ni les facettes ne sont démontrables. Passer la requête au serveur suffit — une vingtaine de lignes, et le point est pris.

### Navigateurs supplémentaires — Minor, 1 pt · coût faible

**Exige** : compatibilité complète avec **au moins 2 navigateurs** de plus · tester et corriger toutes les fonctionnalités · **documenter les limitations** · UI cohérente.

Surtout du test et de la documentation. Attention au WebSocket et aux cookies `Secure` sur Safari.

### Tournoi — Minor, 1 pt · faible une fois le jeu écrit

**Exige** : ordre des affrontements et **système de bracket** · savoir qui joue contre qui · **matchmaking** · inscription et gestion. *Exige qu'un jeu existe.*

### OAuth 2.0 — Minor, 1 pt

**Exige** : authentification distante (Google, GitHub, 42).

Les trois boutons du front ciblent déjà `/api/v1/auth/oauth/`, chemin **déjà exempté** par le middleware CSRF. Aucune route backend n'existe : les boutons sont inertes. Table `oauth_accounts` avec `UNIQUE (provider, provider_account_id)`. `password_hash` est déjà nullable pour les comptes créés uniquement par OAuth.

### Notifications — Minor, 1 pt · coût moyen-élevé

**Exige** : un système complet pour **toutes** les actions de création, mise à jour et suppression. Plus large qu'il n'y paraît : table, diffusion WebSocket, badge de non-lus, toasts sur chaque mutation.

### WCAG 2.1 AA — Major, 2 pts

**Exige** : conformité complète · **lecteur d'écran** · navigation clavier · technologies d'assistance.

Le front est déjà construit avec (contrastes mesurés, focus visible, labels associés, ARIA, éléments natifs). Reste l'audit complet et le test réel au lecteur d'écran.

⚠️ **Le chronomètre du duel s'oppose au critère 2.2.1 « Timing Adjustable »**, qui impose de pouvoir désactiver, ajuster ou prolonger toute limite de temps. À traiter **à la conception de l'arène** : le mode solo sans chrono est une réponse, un réglage de durée dans le lobby en est une autre. Si ça surface après, on perd un des deux modules.

### API publique — Major, 2 pts

**Exige** : **clé d'API sécurisée** · rate limiting · **documentation** · au moins 5 endpoints couvrant GET, POST, **PUT**, DELETE.

`/api/docs` existe déjà (OpenAPI). Manquent la clé d'API, un `PUT` (l'API n'expose que `PATCH`), et surtout : **les 4 routes de `friends.py` n'ont pas de `response_model`, donc elles sont absentes du schéma OpenAPI**. À corriger avant de réclamer le module.
