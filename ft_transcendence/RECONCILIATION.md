# Réconciliation des deux historiques

Le dépôt `alyssiagalleze/Transcendence` et le `main` local n'ont **aucun ancêtre commun**. Aucun des deux n'est un sur-ensemble de l'autre. Ce document dit quoi prendre de quel côté, dans quel ordre, et où sont les deux vrais conflits.

Remote local : `aly` → `git@github.com:alyssiagalleze/Transcendence.git`.

## L'état des lieux

| | Fichiers |
|---|---|
| Sur `main` | 126 |
| Sur `aly/main` | 139 |
| **Identiques des deux côtés** | **89** |
| Divergents | 28 |
| Seulement `main` | 9 |
| Seulement `aly/main` | 22 |

Sur les 28 divergences, **10 sont purement cosmétiques** : `aly` a réécrit des docstrings, sans toucher une ligne de code. Vérifié fichier par fichier — le contenu technique des commentaires est préservé, y compris le piège `X-Forwarded-For` de `deps.py`.

---

## La direction : partir de `aly/main`

`aly/main` a 22 fichiers que nous n'avons pas, dont tout le domaine contenu. Notre avance à nous est **concentrée en six endroits identifiables**. Il est donc bien moins coûteux de partir de leur arbre et d'y rejouer nos six deltas que l'inverse.

### Les six deltas à rejouer

| # | Delta | Fichiers | Effort |
|---|---|---|---|
| 1 | **La 2FA branchée** | `api/v1/auth.py` (124 lignes de code), `models/backup_code.py`, `schemas/auth.py`, sa migration | le gros morceau — voir le conflit ci-dessous |
| 2 | **`assert_production_ready()`** appelée | `app/main.py`, 1 ligne dans `lifespan` | trivial |
| 3 | **La purge des secrets** | `.gitignore` décommenté, `git rm --cached` sur `srcs/.env` + `secrets/*`, `srcs/.env.example` | trivial mais obligatoire |
| 4 | **L'URL OAuth relative** | `srcs/frontend/app/components/auth/AuthProviders.vue` | trivial |
| 5 | **Les 4 documents** | `MODULES.md`, `PLAN.md`, `srcs/backend/BACKEND.md`, `srcs/frontend/FRONTEND.md` | déjà poussés pour 2 d'entre eux |
| 6 | **La suite de tests auth** | `tests/conftest.py`, `tests/__init__.py`, `tests/test_auth.py` | complémentaire de leurs `test_i18n` / `test_roles` |

Tout le reste des divergences se résout en **prenant la version `aly`** : elle est soit identique fonctionnellement, soit en avance.

---

## Conflit n°1 — `backup_code` contre `recovery_code`

**La même fonctionnalité, écrite deux fois sous deux noms.** Ce sont les codes de secours de la 2FA.

| | `main` | `aly/main` |
|---|---|---|
| Modèle | `models/backup_code.py` | `models/recovery_code.py` |
| Migration | `bbd3c8683790_backup_codes` | `d53216e55eef_recovery_code` |
| Fonctions | `generate_backup_code()`, `hash_backup_code()` | `generate_recovery_code()`, `hash_recovery_code()` |
| Routes branchées | **oui** — `/2fa/setup`, `/2fa/enable`, `/2fa/verify`, `DELETE /2fa` | **non** — `auth.py` n'a que 5 routes |

**Recommandation : garder la nôtre.** Elle est câblée de bout en bout et testable immédiatement ; la leur est un service et un modèle sans point d'entrée. Concrètement : supprimer `models/recovery_code.py`, sa migration `d53216e55eef`, et les deux fonctions `*_recovery_code` de `security.py`.

⚠️ Leur `security.py` apporte aussi le **chiffrement Fernet des secrets TOTP** (`_cipher`, `encrypt_totp_secret`, `decrypt_totp_secret`) avec un message d'erreur explicite si `TOTP_ENCRYPTION_KEY` manque. **Ça, il faut le garder** — c'est complémentaire, pas concurrent. Ne pas jeter tout leur `security.py` avec le bébé.

## Conflit n°2 — deux têtes alembic

Les deux chaînes partagent un tronc, puis forkent :

```
12e6d8f50850 → cefb74bd94bc → eee92670a8c1
                                   ├── bbd3c8683790  (backup_codes)      ← main
                                   └── d53216e55eef  (recovery_code)     ← aly
                                            └── 1b222c630292  (content)  ← aly
```

En l'état, `alembic upgrade head` échouerait sur **deux têtes**.

La résolution découle du conflit n°1 : si `recovery_code` disparaît, il faut aussi retirer `d53216e55eef` de la chaîne et faire pointer `1b222c630292` (le contenu, qu'on garde) directement sur `eee92670a8c1`, puis rebaser notre `bbd3c8683790` par-dessus :

```
12e6d8f50850 → cefb74bd94bc → eee92670a8c1 → 1b222c630292 → bbd3c8683790
```

Deux `down_revision` à éditer. **Aucune base n'ayant encore tourné en production, réécrire ces chaînes est sans risque** — c'est le bon moment, ça ne le sera plus après.

---

## Ce que `aly/main` apporte, et qu'on prend tel quel

**Le domaine contenu — 11 tables** dans `models/content.py` : `topics`, `topic_translations`, `learning_paths`, `learning_path_translations`, `path_modules`, `questions`, `question_translations`, `choices`, `choice_translations`, `tags`, `question_tags`. C'est exactement le schéma que `BACKEND.md` décrivait comme à construire.

**La rédaction et la modération** — `api/v1/questions.py`, 7 routes : liste paginée, création, mise à jour, soumission, approbation, rejet, file de relecture. Avec les rôles `Author` et `Moderator` déjà câblés, et un `audit_log`.

**La recherche** — `services/search.py`, 197 lignes : full-text avec index GIN, correspondance par trigrammes `pg_trgm` pour tolérer les fautes de frappe, facettes par difficulté, pagination. **Le module « recherche avancée » est acquis côté serveur.**

**L'i18n backend** — `app/i18n.py` (124 lignes) et la dépendance `Locale` dans `deps.py`, avec repli sur `en`.

**Plus** : `services/workflow.py`, `services/validation.py`, `services/content.py`, `services/permissions.py`, `models/audit_log.py`, `schemas/question.py`, `schemas/search.py`, un paquet `seeds/`, et les tests `test_i18n.py` / `test_roles.py`.

⚠️ **`services/permissions.py` revient.** Je l'avais supprimé de `main` en le jugeant mort — il l'était chez nous, parce qu'il importait un `class Question: pass` vide. Chez eux, `models/content.py` fournit le vrai modèle : le fichier est fonctionnel. La suppression était juste sur notre arbre et fausse sur le projet.

## Ce qu'il faut refuser de `aly/main`

- `srcs/.env`, `secrets/POSTGRES_PASSWORD`, `secrets/POSTGRES_USER` — versionnés en violation de `subject.md:186`. Valeurs déjà régénérées donc inertes, mais la purge est à refaire de ce côté.
- `index(1).html` — le mock du produit socratique, supprimé de `main` à dessein.

---

## Ordre d'exécution

1. **Purger les secrets sur `aly/main`** — avant tout le reste, pour ne pas les propager dans les commits de fusion.
2. Créer une branche de réconciliation depuis `aly/main`.
3. Rejouer les six deltas, la 2FA en dernier (c'est le plus gros).
4. Trancher `backup_code` / `recovery_code`, puis recoudre les deux `down_revision`.
5. `alembic upgrade head` sur une base vide → doit passer d'un bloc, sans « multiple heads ».
6. `mypy app` et la suite pytest complète — les nôtres **et** les leurs.
7. Faire de cette branche le nouveau `main` du dépôt.

## Vérification

```bash
# une seule tête alembic
docker compose exec api alembic heads          # doit renvoyer une ligne

# la chaîne se déroule sur une base vierge
make clean && make dev                          # pgdata recréé, entrypoint migre

# les deux suites passent
docker compose exec api pytest -q               # test_auth + test_i18n + test_roles

# aucun secret suivi
git ls-files | grep -E "^srcs/\.env$|^secrets/" # doit ne rien renvoyer

# les 8 routes d'auth sont là, pas 5
curl -sk https://localhost/api/openapi.json | python3 -c "
import json,sys
p=json.load(sys.stdin)['paths']
print([k for k in p if '/auth/' in k])"
```
