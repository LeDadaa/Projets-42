# Backend — état, conventions et ce qu'il reste à construire

Référence de travail du backend. Elle condense `PROJECT_PLAN.fr.md`, un document de 2662 lignes écrit pour une direction produit **abandonnée depuis** — un QCM sans IA — et supprimé du dépôt : sa définition produit et son audit de modules étaient périmés. Son **schéma de données**, son **protocole temps réel** et sa **posture de sécurité** restaient valables : ils sont résumés ici. L'original reste récupérable dans l'historique (`git show 92a6163:PROJECT_PLAN.fr.md`).

Le périmètre en points est dans `MODULES.md` à la racine.

---

## Ce qui existe aujourd'hui

5 000 lignes, dix-huit tables, trente-deux routes REST plus un WebSocket. Dix fichiers de tests, 132 cas.

```
app/
  main.py            app FastAPI, middlewares, montage des routeurs
  config.py          pydantic-settings ; secrets par fichier via le suffixe _FILE ; reglages LLM_*
  db.py              engine async + get_db()
  deps.py            DbSession, CurrentUser, OptionalUser, Locale, Grader, require_role, client_ip
  api/
    health.py        GET /health — teste la base, 503 si injoignable
    ws.py            /api/ws — presence, multi-onglets, diffusion aux amis
    v1/auth.py       register, login, me, refresh, logout, 2fa/{setup,enable,verify}
    v1/users.py      PATCH /users/me, avatar, GET /users?q= (recherche), GET /users/{id} (profil public)
    v1/friends.py    demandes, acceptation, suppression, liste, GET /requests (en attente), block/unblock
    v1/chat.py       POST /chat/messages, GET /chat/conversations, GET /chat/with/{id}, mark-read
    v1/questions.py  GET /questions (liste filtrable), GET /questions/{id} — lecture seule
    v1/topics.py     GET /topics — hierarchie rang > niveau, traduite
    v1/attempts.py   POST /questions/{id}/attempts (correction en flux), GET du meme chemin (essais
                     restants), POST /attempts/{id}/self-assessment
  middleware/csrf.py double-submit, avec liste d'exemptions
  models/            user, refresh_token, friendship, message, backup_code, content, attempt,
                     base, enums
  schemas/           user, auth, friends, chat, question, topic, search, attempt
  services/          security, media, ratelimit, realtime (hub WS), social (graphe d'amis partage),
                     content, search, validation, attempts (limites et persistance),
                     llm/ (client Ollama, consignes, verdict)
  migrations/        9 revisions : users, refresh_tokens, friendships, backup_codes,
                     direct_messages, contenu, slug + points cles, titre de traduction, attempts
seeds/               chargeur TOML idempotent + seeds/content/*.toml (le contenu lui-meme)
tests/               10 fichiers : auth, roles, i18n, amis, chat, contenu, questions, llm, attempts
```

**Aucun des auteurs d'origine n'est encore dans l'équipe.** Le code est dense en commentaires qui expliquent *pourquoi* — comparaison à temps constant, familles de refresh tokens, `dummy_verify`, `MissingGreenlet` après rollback. Les lire avant de modifier : ce ne sont pas des évidences.

## Conventions

- **Couches** : `api/` (routes, validation HTTP) → `services/` (logique) → `models/` (ORM) → `schemas/` (contrats). Aucune logique métier dans une route.
- **Commentaires denses**, à l'inverse du frontend. Ils expliquent les pièges, pas le code. Convention délibérée, à conserver.
- **Tout modèle doit être importé dans `app/models/__init__.py`.** Un modèle absent est invisible pour alembic, qui proposera alors de *supprimer* sa table.
- `naming_convention` est posée sur `MetaData` (`models/base.py`) : les contraintes ont des noms déterministes, donc `downgrade()` peut les supprimer. Ne pas y toucher.
- Les enums vont en base en **VARCHAR + CHECK** (`enum_column()` dans `models/enums.py`), pas en type natif Postgres : ajouter une valeur ne demande alors pas de migration de type.
- `mypy app` doit passer.

---

## Le schéma à construire

Quatre tables existent (`users`, `refresh_tokens`, `friendships`, `backup_codes`). Le reste est à écrire. Le vocabulaire est **déjà déclaré** dans `models/enums.py` — `QuestionKind`, `QuestionStatus`, `AttemptSource`, `GraderKind`, `MatchMode`, `MatchStatus`, `OAuthProvider` — il ne reste qu'à créer les tables.

### Contenu

```
topics                 id, slug UNIQUE, parent_id FK→topics NULL, position, icon
topic_translations     (topic_id, locale) PK, name, description

questions              id, slug UNIQUE NOT NULL, topic_id FK, kind ENUM, key_points TEXT[],
                       difficulty_rating NUMERIC DEFAULT 1200,
                       status ENUM(draft|in_review|published|archived|rejected),
                       author_id FK→users, reviewed_by FK NULL, review_note NULL,
                       times_asked, times_correct
question_translations  (question_id, locale) PK, title, stem, explanation,
                       search_vector TSVECTOR  -- index GIN

choices                id, question_id FK, position, is_correct
choice_translations    (choice_id, locale) PK, text

tags / question_tags   étiquetage n-n
```

**Décision à prendre avant la première migration** : le contenu est-il traduit ? Le plan répond oui, avec `en` en langue de repli et une question publiable seulement si sa traduction `en` est complète. Revenir dessus après coup est une réécriture, pas une migration.

`stem` = l'énoncé seul, sans les propositions. `slug` = identifiant d'URL stable, indépendant du titre affiché.

### Apprentissage

```
attempts               id, user_id FK, question_id FK, selected_choice_ids INT[],
                       is_correct NULL, response_ms, source ENUM(practice|review|duel|arena|daily),
                       locale, created_at, match_id FK→matches NULL (pas encore : vient avec matches)
                       raw_response TEXT NULL, grader ENUM(deterministic|llm|self_assessed) DEFAULT 'deterministic',
                       grader_meta JSONB NULL, grader_confidence NUMERIC NULL

user_topic_mastery     (user_id, topic_id) PK, rating NUMERIC DEFAULT 1200, compteurs
user_question_state    (user_id, question_id) PK, box SMALLINT, due_at, last_result
```

`attempts` est la table qui grossit le plus vite et la source de vérité de toute l'analytique. `box` est le numéro de boîte de Leitner (1 à 5) pour la répétition espacée ; `due_at` la date de réapparition.

**`attempts` existe** (migration `f7e2a6c1d9b3`), avec les quatre colonnes d'accroche du correcteur LLM : `raw_response`, `grader`, `grader_meta`, `grader_confidence`. `is_correct` n'est vide que pendant une auto-évaluation en attente. `match_id` n'y est pas encore : la table `matches` n'existe pas, le moteur de jeu ajoutera la colonne avec sa clé étrangère. `user_topic_mastery` et `user_question_state` restent à construire.

### Jeu

```
matches                id, mode ENUM(duel|arena|solo), status ENUM(lobby|running|finished|aborted),
                       created_by FK, topic_id FK NULL, question_count,
                       settings JSONB,  -- {seconds_per_question, lifelines_enabled, elimination}
                       started_at NULL, ended_at NULL

match_players          (match_id, user_id) PK, seat, score, placement NULL,
                       lifelines_used JSONB, joined_at, left_at NULL
match_questions        (match_id, position) PK, question_id FK
```

`seat` fixe l'ordre d'affichage, `placement` le classement final (`NULL` tant que le match court).

---

## La couche temps réel

**Un seul endpoint : `GET /api/ws`**, authentifié par le cookie `access_token` lu pendant la poignée de main (dépendance `WsUser` dans `deps.py`). nginx proxifie déjà l'upgrade, avec `proxy_read_timeout 3600s`.

Implémenté et **mergé** (PR #1) : `app/api/ws.py` et `app/services/realtime.py` côté backend, `useRealtime.ts` et `usePresence.ts` côté frontend. Le module chat (PR #3) circule sur la même socket, avec des noms pointés comme `chat.typing`.

### La forme des messages

Un objet JSON plat, avec une clé **`type`** qui nomme le message et les données **au même niveau** :

```jsonc
// serveur → client
{ "type": "presence", "user_id": 31, "online": true }
{ "type": "pong" }

// client → serveur
{ "type": "ping" }
```

C'est la convention posée par l'implémentation existante. Deux conséquences à connaître :

- **`type` est un nom réservé** au premier niveau : aucun message ne peut avoir une donnée qui s'appelle `type`.
- Les données étant à plat, chaque modèle Pydantic de validation déclarera son propre champ `type` en plus de ses données.

Quand le moteur de match ajoutera ses messages, les noms devront rester distinguables — `start` seul ne dirait pas s'il s'agit du match, de la question ou du chrono. Un nom pointé reste dans la même forme, ce n'est qu'une valeur de chaîne plus longue :

```jsonc
// serveur → client
{ "type": "question.start",  "q": 3, "stem": "…", "choices": [...], "ends_at": "2026-08-02T10:00:12.500Z" }
{ "type": "question.result", "q": 3, "correct": [17], "explanation": "…", "deltas": { "12": 850, "31": 0 } }
{ "type": "leaderboard",     "standings": [ /* trié */ ] }
{ "type": "state.sync",      "match": { /* instantané complet */ } }
{ "type": "match.end",       "podium": [...] }
{ "type": "error",           "code": "not_in_match" }

// client → serveur
{ "type": "match.join",     "match_id": 42 }
{ "type": "match.answer",   "q": 3, "choices": [17] }
{ "type": "match.lifeline", "kind": "fifty_fifty" }
```

⚠️ `leaderboard` et `match.end` portent des listes : à plat, elles doivent être **nommées** (`standings`, `podium`) et non envoyées comme corps du message — un objet JSON ne peut pas être une liste.

**Validation** : un modèle Pydantic par message, dans `realtime/protocol.py`, avec `type` comme discriminant. Un `type` inconnu répond un message `error`, **jamais** une exception qui fermerait la socket.

### Ce que `ConnectionHub` fait déjà

`services/realtime.py` tient un `dict[int, set[WebSocket]]` — un utilisateur peut avoir **plusieurs sockets** ouvertes (plusieurs onglets, plusieurs appareils). « En ligne » signifie donc *au moins une socket ouverte*, pas exactement une.

- `connect()` renvoie `True` seulement si l'utilisateur était hors ligne avant, `disconnect()` seulement s'il vient de passer hors ligne. **La présence n'est donc diffusée qu'à la première socket entrante et à la dernière sortante** : ouvrir un second onglet ne spamme pas les amis.
- `send_to()` ignore une socket qui échoue au lieu de la retirer : sa propre boucle de lecture verra la déconnexion et nettoiera le hub elle-même.
- À la dernière déconnexion, `last_seen_at` est persisté — c'est le repli pour afficher « vu il y a 5 minutes ».
- Le hub transporte un `dict` sans le regarder : **il est agnostique du format des messages.** Changer la forme des trames ne le touche pas.

Un `asyncio.Lock` protège les mutations du dict. Sur une boucle d'événements unique il n'est pas strictement nécessaire, mais il rend les sections critiques explicites.

### Reconnexion

Le module « joueurs distants » l'exige explicitement. **Côté client, la base existe** : `useRealtime.ts` se reconnecte avec un backoff exponentiel plafonné par `MAX_BACKOFF`, et envoie un `ping` périodique qui mesure la latence. **Il manque le jitter et l'abandon avec bouton** — sans jitter, tous les clients coupés par un redémarrage de l'API se reconnectent au même instant. **Côté serveur, rien n'est écrit pour le match** : le hub se contente de nettoyer à la déconnexion.

- À la déconnexion en cours de match : marquer le joueur absent, diffuser `presence`, **le garder dans le match** pendant 60 s de grâce. Il marque 0 sur toute question close pendant son absence.
- Au retour dans le délai : répondre `state.sync` avec l'état complet. `state.sync` doit avoir **exactement la même forme** qu'à l'arrivée initiale — le client n'a alors aucune branche « reprise » à écrire.
- Délai expiré : forfait. S'il ne reste qu'un joueur en duel, victoire par forfait.
- Côté client : backoff exponentiel 1 s → 30 s avec ±20 % de jitter, abandon après ~2 min avec un bouton « Reconnecter ». Le jitter évite que tous les clients déconnectés par un redémarrage se reconnectent au même instant.

### Heartbeat

Le `ping`/`pong` applicatif existe déjà (`ws.py`). Le garder **en plus** des trames de contrôle WebSocket : certains proxys avalent ces dernières et provoquent des déconnexions fantômes. Côté serveur, prévoir un `ping` toutes les 20 s.

## Le moteur de match

**Trois modes, un seul moteur** — c'est ce qui fait que trois majeurs se construisent une seule fois :

| Mode | Joueurs | Module |
|---|---|---|
| Solo | 1 | cœur pédagogique, chrono généreux ou absent |
| Duel | 2 | *Joueurs distants* (majeur) |
| Arène | 3 à 8 | *Multijoueur 3+* (majeur), avec élimination |

Règles à inscrire dans le README **et** dans une fenêtre de règles de l'app :

- 10 questions (configurable), tirées d'un thème, à difficulté proche du score moyen du salon.
- Même question au même instant pour tous. Chrono 20 s, configurable dans le salon.
- Score : `1000 × bonne_réponse + arrondi(500 × temps_restant / temps_total)`, ×1,25 dès 3 bonnes réponses d'affilée, ×1,5 dès 5. Mauvaise réponse ou temps écoulé : 0 et série remise à zéro.
- Jokers, une fois chacun : `fifty_fifty`, `ask_the_crowd` (réparties historiques, désactivé sous 20 tentatives), `skip` (0 point, série préservée).
- Élimination (arène, optionnelle) : à partir de la question 6, le score le plus bas de la manche sort et passe spectateur.
- Victoire : meilleur score final, ou dernier en lice. Égalité départagée au temps cumulé.

```python
class MatchEngine:
    async def run(self) -> None:
        await self._broadcast(StateSync(...))
        for idx, q in enumerate(self.questions):
            self.deadline = now() + self.seconds_per_question
            await self._broadcast(QuestionStart(q, ends_at=self.deadline))
            await self._collect_answers_until(self.deadline)   # sortie anticipée
            await self._broadcast(QuestionResult(self.scoring.settle(q, self.answers[idx])))
            await self._broadcast(Leaderboard(self.standings()))
            if self.elimination and idx >= 5:
                await self._eliminate_last()
            await asyncio.sleep(REVEAL_SECONDS)
        await self._broadcast(MatchEnd(self.podium()))
        await self._persist()          # UNE SEULE transaction
```

`_collect_answers_until` combine `asyncio.wait_for` et un `asyncio.Event` levé dès que tous les joueurs connectés ont répondu : une manche rapide n'attend pas 20 s dans le vide.

**Le serveur fait autorité sur le temps et le score, et n'envoie jamais la bonne réponse avant l'échéance.** Un chrono côté client se triche en trois lignes de console.

---

## La couche IA — le correcteur

C'est le majeur **« Complete LLM system interface »**. Le sujet exige trois choses (`subject.md:375-377`), notées séparément : **génération de texte**, **streaming correctement géré**, **gestion d'erreurs et rate limiting**.

⚠️ **Ce n'est pas du RAG, et c'est délibéré.** Le RAG est un *autre* majeur (`subject.md:370-373`), qui exige que les utilisateurs posent des questions libres sur un grand corpus. Le produit ne prévoit pas d'interface de chat : le LLM n'intervient qu'en mode entraînement. Il n'y a donc rien à aller chercher — le contexte est toujours connu.

### Ce que le modèle fait

L'élève **rédige une réponse** à un exercice (`QuestionKind.free_text`, déjà autorisé par la contrainte en base). Le modèle la corrige, avec nuance, en trois issues :

| Réponse de l'élève | Ce que renvoie le modèle |
|---|---|
| Juste, claire, précise | valide — on passe à la suite |
| Juste mais imprécise | valide, **plus une explication** sur ce qui manquait |
| À côté, ou « je ne sais pas » | oriente et explique **en fonction de ce qu'il a écrit** |

C'est `GraderKind.llm`, prévu dans les enums depuis le début ; `GraderKind.self_assessed` marque le repli, quand l'élève s'est corrigé lui-même. Les quatre colonnes d'accroche de `attempts` — `raw_response`, `grader`, `grader_meta`, `grader_confidence` — sont faites exactement pour ce cas.

### La réponse de référence et le chargement du contenu

`questions.key_points` — un tableau de textes, un point par élément — porte **ce qu'une bonne réponse doit contenir**. **Une seule grille par question, rédigée en anglais** quelle que soit la langue de l'élève : tout le monde est corrigé sur les mêmes critères. C'est la grille contre laquelle le modèle corrigera, donc la clé des réponses : `GET /questions/{id}` ne la renvoie **jamais**, pas plus que `explanation`, qui reste rédigée pour l'élève après coup et n'est pas un critère de correction.

Une question `free_text` n'est publiable (`assert_publishable`) que si elle n'a **aucune proposition**, une traduction `en` complète et **au moins un point clé**.

Trois routes de lecture, ouvertes aux anonymes : `GET /questions` (liste filtrable ; chaque élément porte le titre et l'énoncé traduits, `is_fallback` et ses tags), `GET /questions/{id}` (détail) et `GET /topics` (hiérarchie des thèmes traduite, à plat avec `parent_id` et `depth`). **Aucune ne renvoie la grille ni l'explication** : la liste construit ses éléments champ par champ, pour qu'une colonne ajoutée à `questions` ne puisse jamais fuir par accident.

**Le contenu vit dans des fichiers TOML relus en pull request**, `srcs/backend/seeds/content/*.toml`, chargés par `python -m seeds` — lancé au démarrage de l'API si `SEED_DEMO_DATA=true`, ou à la main avec `docker compose exec api python -m seeds`. **C'est la seule source des questions** : l'API n'a aucune route d'écriture sur elles, ni rédaction ni modération.

⚠️ **Jamais le texte d'un sujet d'exam.** Les sujets des Exam Rank sont confidentiels à 42, et ce dépôt est lu par des évaluateurs qui sont eux-mêmes étudiants de 42 : une question porte sur une **notion** que l'exam teste, et s'écrit de zéro. Ni énoncé, même reformulé, ni nom d'exercice, ni nom de fichier de rendu.

**Couverture au 27/09/2026** : 127 questions publiées, 5 rangs et 11 niveaux — rang 02 (niveaux 0 à 3), rangs 03, 04 et 05 (niveaux 0 et 1), rang 06 (un seul niveau). Chaque affirmation technique a été vérifiée par un programme C ou C++ compilé, et chaque question a été passée au correcteur : l'explication de référence doit être validée, une réponse fausse typique refusée.

Organisation : `topics.toml` porte la hiérarchie rang → niveau, et chaque niveau a son propre fichier de questions, nommé d'après le slug du niveau (`exam-rank-05-level-0.toml`). Les tags désignent des notions (`operator-overloading`, `polymorphism`…) : le rang et le niveau sont déjà portés par le thème.

```toml
[[topics]]
slug = "exam-rank-05"
[topics.translations.en]
name = "Exam Rank 05"

[[questions]]
slug = "rank05-l0-une-notion"       # obligatoire, stable et public : c'est lui qui rend le rechargement idempotent
topic = "exam-rank-05"
kind = "free_text"                   # seul type chargeable pour l'instant
status = "published"                 # draft | published | archived — published si absent
difficulty = 1200                    # lu à la création seulement
tags = ["operator-overloading"]      # des notions, pas le rang ni le niveau
key_points = ["first expected point", "second expected point"]   # en anglais

[questions.translations.en]          # obligatoire pour publier
title = "A short, general title"     # obligatoire dans chaque traduction, affiché dans les listes
stem = """The question."""
explanation = """Shown after the answer."""
```

⚠️ **En TOML, une clé appartient au dernier en-tête qui la précède.** `key_points` s'écrit donc **avant** `[questions.translations.en]` : placée dessous, elle devient une clé de la traduction, et le chargeur refuse le fichier.

**Les slugs sont publics** : `GET /questions/{id}` les renvoie. Un slug ne doit donc ni souffler la réponse, ni reprendre le nom d'un exercice d'exam. ⚠️ **Renommer un slug crée une seconde question** et laisse l'ancienne en base, telle qu'au dernier chargement : pour renommer, garder l'ancienne entrée en `archived` et en ajouter une nouvelle.

Règles du chargeur : une clé inconnue (faute de frappe) est **refusée**, pas ignorée ; tout se fait dans **une seule transaction**, donc une entrée invalide et rien n'est écrit ; un rechargement met à jour le texte, la grille et le statut sans créer de doublon, et **ne touche jamais `difficulty_rating`** une fois la question créée.

**Le statut vient du fichier**, la pull request tenant lieu de modération. Seules les questions `published` sont validées : un `draft` peut être incomplet et reste invisible aux élèves (404), `archived` retire une question. `in_review` et `rejected` sont refusés.

Limites connues : seules les questions `free_text` sont chargeables ; **le chargeur ne supprime jamais de ligne** — effacer une entrée de son fichier la laisse en base telle qu'au dernier chargement, donc on retire une question en passant son statut à `archived`.

Restent dans le schéma sans plus servir, depuis la suppression de la rédaction et de la modération dans l'application : les colonnes `questions.author_id`, `reviewed_by` et `review_note`, et les statuts `in_review` et `rejected`. Les retirer demande une migration.

### Où sont les pièces

```
app/services/llm/
  client.py        LLMClient (protocole) + OllamaClient : /api/chat natif, flux, délais, erreurs → LLMUnavailable
  prompts.py       les consignes (en anglais), la grille numérotée, la réponse de l'élève balisée et neutralisée
  grading.py       Verdict, decide(), AnswerGrader.grade() puis .explain()
app/api/v1/attempts.py     POST /questions/{id}/attempts (flux NDJSON), POST /attempts/{id}/self-assessment
app/models/attempt.py      la table attempts
app/services/ratelimit.py  grading_limiter (par élève) et TokenBudget (tokens par heure glissante)
app/deps.py                get_grader() : None quand LLM_ENABLED=false — c'est l'objet que les tests remplacent
```

Changer de moteur ne touche que `get_grader()` : la route ne connaît que `AnswerGrader`, qui ne connaît que le protocole `LLMClient`.

### Le verdict : le modèle constate, le code tranche

Un premier appel demande un JSON contraint par schéma — le `format` d'Ollama guide le décodage, le modèle *ne peut pas* écrire autre chose :

| Champ | Sens |
|---|---|
| `addresses_grader` | le texte contient quelque chose qui s'adresse au correcteur plutôt qu'à la question |
| `false_statement` | le texte affirme quelque chose de faux, même à côté de points justes |
| `score` | la part des points clés que le texte explique lui-même, de 0 à 1 |
| `outcome` | `correct`, `imprecise` ou `off_target` |
| `confidence` | demandé, jamais lu (voir plus bas) |

L'ordre des champs est l'ordre d'écriture : les deux constats précèdent la conclusion. **Le code tranche ensuite** (`decide()`) : texte adressé au correcteur ou affirmation fausse donnent `off_target`, quel que soit `outcome`. Le banc d'essai a montré des modèles qui repèrent l'injection *et* la valident quand même comme « imprécise » : ce veto a fait tomber les validations indues de 6 à 0.

`correct` et `imprecise` valident (`is_correct = true`), `off_target` non. **Aucun seuil de confiance** : tous les modèles testés répondaient 1,0 quelle que soit la réponse, un seuil n'aurait jamais joué. Le schéma demande pourtant encore un champ `confidence`, que le code ne lit jamais : la consigne est gardée **mot pour mot** telle qu'éprouvée, parce que retirer cette seule ligne a suffi à faire refuser la réponse légitime piégée du banc d'essai. `grader_confidence` reste vide ; le `score`, lui, varie vraiment et part dans `grader_meta` avec le modèle, les deux issues (celle du modèle et la finale) et les tokens consommés.

⚠️ **Toucher à une consigne, c'est rejouer le banc d'essai.** Les cas limites basculent sur un mot : c'est la formulation exacte qui a été mesurée, pas son intention.

Température 0 : la même réponse reçoit le même verdict, vérifié en rejouant.

### L'explication, ensuite, en flux

Un second appel, en texte libre et streamé, avec sa propre consigne :

- `imprecise` : ce qui était juste, puis précisément ce qui manquait ;
- `off_target` : ce qui est faux ou absent dans *ce que l'élève a écrit*, puis l'idée juste, assez clairement pour qu'il réécrive avec ses mots — les essais suivants servent à reformuler, pas à deviner ;
- `correct` : pas de texte généré, l'explication rédigée de la question suffit ;
- **réponse adressée au correcteur : aucun appel.** Le front affiche un message fixe traduit ; un modèle à qui l'on demande de commenter une injection la recopie parfois telle quelle.

Un mode « indice », qui aurait orienté sans donner la réponse, a été essayé puis abandonné : les modèles la donnaient quand même, ou se trompaient en voulant la cacher.

### La route

`POST /api/v1/questions/{id}/attempts`, corps `{"answer": "…"}` (1 à 2 000 caractères). **`POST`, et non `GET` + `EventSource`** comme prévu à l'origine : la réponse de l'élève serait sinon dans l'URL, donc dans les journaux d'accès de nginx et dans l'historique, et limitée à ~8 Ko. Le front lit le flux avec `fetch`, et le jeton CSRF part comme pour toute écriture.

Les refus arrivent **avant** le flux, en JSON ordinaire : 401, 404 (brouillon, archivée, ou pas `free_text`), 422, 429 `attempts exhausted` ou `too many tries` avec `Retry-After`. Sinon, 200 en `application/x-ndjson`, un objet JSON par ligne :

```
{"type":"verdict","attempt_id":12,"outcome":"off_target","score":0.33,"flagged":false,"attempts_left":2}
{"type":"delta","text":"Tu as inversé…"}                      répété, dans l'ordre
{"type":"error","code":"explanation_failed"}                   l'explication s'est coupée, le verdict tient
{"type":"done","closed":false,"explanation":null}              explication rédigée seulement une fois l'exercice clos
{"type":"self_assessment","attempt_id":13,"reason":"unavailable","explanation":"…","attempts_left":1}
```

Même convention que le WebSocket : un objet plat avec une clé `type`. Le front passe `?locale=` comme pour la liste des questions : sans lui, l'explication suivrait la préférence du compte (`en` par défaut), et non la langue affichée. L'essai est **écrit en base dès le verdict**, avant l'explication : un élève qui ferme l'onglet pendant le flux a quand même consommé son essai. `X-Accel-Buffering: no` empêche nginx de retenir le flux dans son tampon.

**`GET /api/v1/questions/{id}/attempts`** répond `{"limit": 3, "left": 2}` : la limite configurée et ce qu'il reste à cet élève sur 24 h glissantes. Le front l'appelle à l'ouverture d'un exercice, avant toute réponse — la limite vit dans les réglages et les essais déjà dépensés ne sont connus qu'ici. Sans elle, la page annonçait une limite qu'elle ne pouvait pas vérifier et offrait la saisie à un élève qui n'avait plus d'essai, pour finir en 429. Même règle de 404 que la correction, et **aucune réponse d'élève n'est renvoyée** : seulement les deux nombres.

La génération du flux utilise la session de la requête : depuis la 0.118, FastAPI ne ferme une dépendance à `yield` qu'une fois la réponse entièrement envoyée, flux compris — vérifié sur la 0.141 installée.

### Les limites

| Limite | Mécanisme | Réponse |
|---|---|---|
| 3 essais par question sur 24 h glissantes | compte des lignes `attempts`, sur l'index `(user_id, question_id, created_at)` | 429 `attempts exhausted` |
| débit par élève | `grading_limiter` : 5 d'affilée, puis 1 toutes les 20 s | 429 `too many tries` |
| budget global | `TokenBudget` : tokens lus et écrits, **comptés par Ollama**, sur une heure glissante | repli `budget` |

Le budget compte des tokens et non des requêtes : un compteur de requêtes ne distingue pas une réponse de deux lignes d'une page entière. Comme les autres limiteurs, il vit en mémoire, ce qui suppose un seul processus.

### Le repli : l'auto-évaluation

Modèle désactivé (`LLM_ENABLED=false`, le défaut), budget épuisé, ou machine injoignable (le client abandonne la connexion au bout de 5 s) : l'élève reçoit l'explication rédigée de la question et indique lui-même s'il avait juste (`POST /attempts/{id}/self-assessment`). L'essai est enregistré avec `grader = self_assessed`, et `is_correct` reste vide jusqu'à sa réponse. C'est aussi ce qui se montre quand on coupe le moteur devant l'évaluateur.

### Où tourne le modèle : une machine à GPU, par Tailscale

Le modèle (`gemma4:12b`, 8,4 Go de mémoire vidéo) tourne sous Ollama sur **le PC de Jules** (RX 6800 XT, 16 Go). Chaque instance du projet — la sienne, celles de l'équipe, le poste d'évaluation — le joint par Tailscale :

- le service `tailscale` du compose rejoint le tailnet en mode **userspace** (ni `/dev/net/tun` ni `NET_ADMIN`, donc il tourne sous Podman rootless à l'école) et expose un **proxy HTTP** sur le réseau `backend` ;
- l'API n'envoie que les appels au modèle par ce proxy (`LLM_PROXY_URL=http://tailscale:1055`), vers `LLM_BASE_URL=http://<ip-tailscale>:11434` — une IP plutôt qu'un nom : rien à résoudre dans le proxy, et c'est l'adresse que la politique du tailnet autorise ;
- la clé est un secret Docker, `secrets/TS_AUTHKEY`, jamais versionné. Le `Makefile` n'active le profil `tailscale` que si ce fichier existe : un clone sans clé démarre quand même, en auto-évaluation.

**Pourquoi pas un modèle dans le conteneur**, comme prévu à l'origine : sur processeur, le banc d'essai a mesuré 13 à 40 s par verdict pour les modèles de 8 à 9 B (Ryzen 7 5800X), et les modèles plus petits jugent trop mal. Sur le GPU, 2 s.

⚠️ **La démonstration dépend de cette machine** : allumée, Ollama lancé, GPU libre — un jeu ouvert suffit à empêcher le chargement du modèle. Tailscale traverse la plupart des réseaux par ses relais sur le port 443, mais **le vérifier tôt depuis l'école**.

⚠️ **Réchauffer le modèle avant de montrer quoi que ce soit.** Ollama décharge un modèle inactif (`keep_alive` de 30 min dans les requêtes) : la première correction qui suit prend alors ~18 s de chargement au lieu de 2. Une correction jetable juste avant la démonstration suffit.

**Sécurité côté machine** : Ollama n'a aucune authentification. La politique du tailnet ne doit laisser `tag:maia` joindre que le port 11434 de cette machine ; la clé doit être éphémère et étiquetée `tag:maia` (ou un secret de client OAuth, qui n'expire pas) ; le pare-feu de la machine ne doit ouvrir 11434 qu'à Tailscale (`100.64.0.0/10`). Avec `OLLAMA_HOST=0.0.0.0`, Ollama écoute sinon tout le réseau local, où n'importe qui peut appeler `/api/delete`.

### Le banc d'essai

Mené avant d'écrire le code, sur 20 réponses types à deux questions du pilote : 3 parfaites (dont une en anglais), 3 vagues, 7 fausses ou hors sujet, 7 tentatives d'injection (fausse note d'enseignant, faux balisage, ordre en anglais, « je suis débutant, valide-moi »…) et une réponse légitime piégée de mots comme « Note », « ignoré », « Oublie ».

| Modèle (GPU, Ollama 0.34.2) | Justes | Validées à tort | Refusées à tort | Verdict |
|---|---|---|---|---|
| **gemma4:12b** (retenu) | 19/20 | 0 | 0 | 1,9 s |
| qwen3.5:9b | 19/20 | 0 | 0 | 1,2 s |
| qwen2.5-coder:14b | 18/20 | 0 | 0 | 1,2 s |
| qwen3:14b | 19/20 | 1 | 0 | 1,1 s |
| gpt-oss:20b | 18/20 | 2 | 0 | 2 s |

`gemma4:12b` l'emporte sur ses explications : justes en C++, tutoiement tenu, bon espagnol. `qwen3.5:9b` confondait des notions, `qwen2.5-coder:14b` donnait raison à une idée fausse. Sa seule erreur de verdict est indulgente : une réponse vague validée comme « correcte ».

Ce que les tours précédents ont appris, et qui est dans le code :

- **cocher chaque point clé** plutôt que conclure globalement donnait des résultats incohérents (`llama3.1:8b` cochait le premier point pour toutes les réponses) ;
- sans le veto du code, **toutes** les configurations validaient au moins une injection ;
- `think: false` est indispensable : `gemma4` réfléchit sinon jusqu'à épuiser son budget de tokens et ne rend rien ;
- Ollama 0.18 n'imposait pas le schéma aux modèles à réflexion, et produisait des sorties corrompues sur GPU AMD : **0.34 minimum**.

### Les cinq risques, tels que traités

1. **Jugement de la nuance** : veto du code sur les deux constats ; l'erreur résiduelle mesurée est du côté indulgent seulement.
2. **Non-déterminisme** : température 0, sortie contrainte par schéma.
3. **Injection** : réponse balisée et balises neutralisées (`neutralize()`), consigne répétée après le texte, champ `addresses_grader` et veto, aucun texte généré sur une réponse signalée. 7 injections sur 7 refusées au banc d'essai.
4. **Latence** : le verdict seul d'abord (une quarantaine de tokens écrits), l'explication streamée ensuite (premier mot en 0,8 s).
5. **Budget** : 3 essais par question sur 24 h, débit par élève, budget horaire de tokens.

## Contraintes structurelles

**L'API tourne en un seul processus.** `entrypoint.sh` lance `--workers 1`, et le rate limiter est un `dict` en mémoire. L'état des matchs vivra dans la même RAM. Compromis assumé :

- ✅ Trivialement correct : aucune synchronisation inter-processus, aucune race, aucun verrou distribué. Des centaines d'utilisateurs simultanés tiennent sur une boucle d'événements.
- ❌ Aucun passage à l'échelle horizontal, et **un redémarrage de l'API perd les matchs en cours**.

Atténuation attendue par les évaluateurs : mettre l'état derrière un protocole `MatchStore` avec une implémentation `InMemoryMatchStore`, et documenter dans le README qu'un `RedisMatchStore` + pub/sub est le chemin de mise à l'échelle. « Voici notre choix, voici le compromis, voici le chemin d'évolution » est la réponse attendue.

Les données durables restent dans PostgreSQL. Seul l'état *en cours de partie* est en mémoire : un crash perd un match, jamais un historique.

## Sécurité — ce qui est déjà en place

| Menace | Contre-mesure | État |
|---|---|---|
| Vol de mot de passe | Argon2id via `pwdlib`, jamais journalisé | ✅ |
| XSS → vol de session | Cookies `HttpOnly` + CSP stricte servie par nginx | ✅ |
| CSRF | `SameSite=Lax` + double-submit, `X-CSRF-Token` sur toute méthode non-safe | ✅ |
| Force brute | Token bucket par IP **et** par compte sur login, register, TOTP | ✅ |
| Énumération de comptes | Réponse *et* temps de réponse identiques (`dummy_verify`) | ✅ |
| Rejeu de refresh token | Rotation par familles, réutilisation détectée → révocation de toute la famille | ✅ |
| Course sur /refresh | `SELECT … FOR UPDATE` | ✅ |
| Envoi de fichier malveillant | Liste blanche de formats, ≤ 2 Mo refusés *pendant* la lecture, ré-encodage Pillow | ✅ |
| Secrets dans git | `.env` ignoré, `.env.example` versionné, clés régénérées | ✅ |
| Élévation de privilèges | `require_role` existe ; **la propriété d'objet reste à vérifier** au cas par cas | partiel |
| Triche en match | Score serveur, bonne réponse retenue jusqu'à l'échéance | à faire |

**Ne jamais mettre `**payload` dans un modèle ORM** : les schémas Pydantic énumèrent les champs autorisés, c'est ce qui empêche un client d'envoyer `{"role": "admin"}`.

---

## Pièges connus

- **Un fichier de contenu invalide empêche l'API de démarrer** quand `SEED_DEMO_DATA=true` : `entrypoint.sh` lance `python -m seeds` sous `set -e`. C'est voulu — un échec bruyant plutôt qu'un contenu à moitié chargé — mais un fichier cassé bloque toute l'équipe. Lancer `docker compose exec api python -m seeds` avant de pousser du contenu.
- **Ne pas merger `origin/backend_pytest` ni `origin/backend_auth_aly`** : elles descendent d'une version antérieure au découpage `v1` et supprimeraient `api/v1/auth.py`. Vérifier la généalogie d'une branche avant tout merge :
  ```bash
  git merge-base --is-ancestor origin/backend_auth_aly origin/<branche> && echo "obsolete"
  ```
- **Les rate limiters sont des singletons de module** : en test, leur état fuit d'un test à l'autre. La fixture `reset_rate_limiters` de `tests/conftest.py` les vide — sans elle, le 4ᵉ `POST /register` d'une session reçoit un 429.
- **Après un rollback, ne pas relire un attribut ORM** : le rollback expire tous les objets de la session, et la relecture déclenche une IO hors greenlet (`MissingGreenlet`, 500). Prendre un instantané des ids *avant* le commit — voir `send_request` dans `friends.py`. **Même piège dans les tests, en pire** : toutes les requêtes d'un test partagent la session de la fixture `db`, donc le rollback d'une route expire aussi les objets que le test garde en main. Garder des valeurs plutôt que des objets ORM, comme `_pair` dans `tests/test_friends.py`.
## Le module « interaction utilisateur »

Chat + profil + amis (IV.3). Le graphe d'amis et la présence WS existaient ; le
chat et le profil public ont été ajoutés.

- **Chat** : `direct_messages` (une ligne par message, pas d'entité
  « conversation »). `POST /chat/messages` écrit puis pousse une trame
  `chat.message` au destinataire et aux autres onglets de l'expéditeur via le
  hub. Le socket ne porte rien que `GET /chat/...` ne sache reconstruire.
  Rate limit par id d'expéditeur (`message_limiter`, 20 puis 1/s).
- **Blocage** : `friendships.status = blocked`, `requester_id` = le bloqueur.
  `chat.py` refuse la livraison entre une paire bloquée ; la paire disparaît
  des deux listes d'amis.
- **Profil public** : `GET /users/{id}` → `ProfileOut` (pas d'email : réservé à
  `/auth/me`), plus `online` et `friendship` (`none|pending_in|pending_out|friends|blocked`).
- **`services/social.py`** : `friend_ids`, `blocked_between`, `friendship_view`
  — mutualisés entre `friends.py`, `chat.py` et `ws.py`.

## Incohérences d'API — corrigées

- ~~`POST /friends/requests?addressee_id=N` en query string~~ → corps Pydantic `FriendRequestCreate`.
- ~~Les routes de `friends.py` sans `response_model`~~ → toutes typées (`schemas/friends.py`), présentes dans OpenAPI.
- `GET /users/{id}/avatar` renvoie toujours un identicon (nginx sert les vrais via `/media/`) — inchangé, c'est le fallback voulu.
- `users.py` renvoie encore `"name already taken"` là où `auth.py` dit `"display name already used"` — à uniformiser.
