## Verify users and password authentication

```bash
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml restart api && docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml logs --tail=20 api      # clean startup
```

### Registration

```bash
curl -k -i -X POST https://localhost/api/v1/auth/register \
  -H 'Content-Type: application/json' \
  -d '{"email":"joe@joe.com","password":"Joejoejoe-42","display_name":"joe","accept_terms":true}'
#   201 + UserPublic, WITHOUT password_hash
```

### Duplicate -> 409, and validation -> 422

```bash
# replay the command above  -> 409 {"detail":"email already in use"}

curl -k -s -X POST https://localhost/api/v1/auth/register \
  -H 'Content-Type: application/json' \
  -d '{"email":"notanemail","password":"short","display_name":"a!"}' | python3 -m json.tool
#   422 + one message per offending field
```

### Login + cookie

```bash
curl -k -i -c /tmp/cookies.txt -X POST https://localhost/api/v1/auth/login \
  -H 'Content-Type: application/json' \
  -d '{"email":"joe@joe.com","password":"Joejoejoe-42"}'
#   200 + Set-Cookie: access_token=...; HttpOnly; Secure; SameSite=Lax; Path=/

curl -k -s -b /tmp/cookies.txt https://localhost/api/v1/auth/me | python3 -m json.tool
curl -k -s -o /dev/null -w '%{http_code}\n' https://localhost/api/v1/auth/me   # 401 without a cookie
```

### Equal response times

```bash
for i in $(seq 1 8); do
  curl -k -s -o /dev/null -w "unknown %{time_total}\n" -X POST https://localhost/api/v1/auth/login \
    -H 'Content-Type: application/json' -d '{"email":"nobody@joe.com","password":"whatever1234"}'
  curl -k -s -o /dev/null -w "wrongpass %{time_total}\n" -X POST https://localhost/api/v1/auth/login \
    -H 'Content-Type: application/json' -d '{"email":"joe@joe.com","password":"wrongpassword1"}'
done
```
The two series must overlap. An "unknown" that is consistently 100 ms faster means `dummy_verify` is not being called on that path, which leaks which emails exist.

### `password_hash` leaks nowhere

```bash
curl -k -s https://localhost/api/openapi.json | grep -c password_hash
#   expected: 0

curl -k -s -b /tmp/cookies.txt https://localhost/api/v1/auth/me | grep -c password_hash
#   expected: 0
```

### The event loop does not freeze (`to_thread`)

```bash
# 5 concurrent registrations while /health is being polled
for i in $(seq 1 5); do
  curl -k -s -o /dev/null -X POST https://localhost/api/v1/auth/register \
    -H 'Content-Type: application/json' \
    -d "{\"email\":\"u$i@joe.com\",\"password\":\"Joejoejoe-42\",\"display_name\":\"user$i\",\"accept_terms\":true}" &
done
curl -k -s -w '\nhealth: %{time_total}s\n' https://localhost/api/v1/health
wait
```

`/health` must answer within a few milliseconds. Without `asyncio.to_thread` it would wait for all five Argon2 hashes to finish, roughly 1 s.

---




## Verify sessions, refresh, CSRF

```bash
# the table exists
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db sh -c 'psql -U "$(cat /run/secrets/POSTGRES_USER)" -d "$POSTGRES_DB" \
  -c "\d refresh_tokens" -c "\d+ users"'
#   refresh_tokens present, with ix_refresh_tokens_family_id and the CASCADE FK
#   users carries the ck_users_role constraint


# register then login
curl -k -s -X POST https://localhost/api/v1/auth/register -H 'Content-Type: application/json' \
  -d '{"email":"joe@joe.com","password":"Joejoejoe-42","display_name":"joe","accept_terms":true}'
```


### All three cookies are set, with the right attributes

```bash
curl -k -i -c /tmp/c.txt -X POST https://localhost/api/v1/auth/login \
	-H "Content-Type: application/json" \
	-d '{"email":"joe@joe.com","password":"Joejoejoe-42"}' | \
	grep -i set-cookie
#   access_token  ... HttpOnly; Secure; SameSite=Lax; Path=/
#   refresh_token ... HttpOnly; Secure; SameSite=Lax; Path=/api/v1/auth
#   csrf_token    ... Secure; SameSite=Lax; Path=/      <- NO HttpOnly
#   200 + all three cookies; csrf_token must have Max-Age=1209600, NOT 900
```


### Rotation: refresh returns a different token

> `/api/v1/auth/refresh` is NOT in `EXEMPT_PREFIXES`: it is CSRF-protected just
> like `logout`. Without `X-CSRF-Token` you get 403, not 200. And `$CSRF` must be
> re-read **after every successful call**, since `_issue_session` sets a fresh
> CSRF cookie on each rotation.

```bash
CSRF=$(grep csrf_token /tmp/c.txt | awk '{print $7}')
grep refresh_token /tmp/c.txt | awk '{print $7}'      # token A
curl -k -s -b /tmp/c.txt -c /tmp/c.txt -H "X-CSRF-Token: $CSRF" \
  -X POST https://localhost/api/v1/auth/refresh > /dev/null
grep refresh_token /tmp/c.txt | awk '{print $7}'      # token B, different from A
```


### Replay detection

```bash
cp /tmp/c.txt /tmp/stolen.txt       # we "steal" the session at this instant

CSRF=$(grep csrf_token /tmp/c.txt | awk '{print $7}')   # re-read: it changed in test 2
curl -k -s -b /tmp/c.txt -c /tmp/c.txt -H "X-CSRF-Token: $CSRF" \
  -X POST https://localhost/api/v1/auth/refresh >/dev/null
#   the victim keeps going: the token in /tmp/stolen.txt is now revoked

VOL=$(grep csrf_token /tmp/stolen.txt | awk '{print $7}')  # the CSRF of the stolen session
curl -k -s -o /dev/null -w 'replay: %{http_code}\n' \
  -b /tmp/stolen.txt -H "X-CSRF-Token: $VOL" -X POST https://localhost/api/v1/auth/refresh
#   expected: 401, and it must carry the deletion Set-Cookie headers

CSRF=$(grep csrf_token /tmp/c.txt | awk '{print $7}')
curl -k -s -o /dev/null -w 'victim: %{http_code}\n' \
  -b /tmp/c.txt -H "X-CSRF-Token: $CSRF" -X POST https://localhost/api/v1/auth/refresh
#   expected: 401 AS WELL - the whole family is revoked, that is the point
```

The 401 from the replay must additionally carry all three deletion `Set-Cookie` headers:

```bash
curl -k -s -D - -o /dev/null -b /tmp/stolen.txt -H "X-CSRF-Token: $VOL" \
  -X POST https://localhost/api/v1/auth/refresh | grep -i set-cookie

#   access_token / refresh_token / csrf_token with Max-Age=0
#   if this is empty: the branch raises an HTTPException instead of RETURNING a
#   JSONResponse - FastAPI only copies the headers of the injected `Response` on
#   the normal return path (routing.py:750), never from an exception handler.
```

And the log must contain the security line:

```bash
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml logs api | grep security.refresh_reuse
```

In the database:

```bash
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db sh -c 'psql -U "$(cat /run/secrets/POSTGRES_USER)" -d "$POSTGRES_DB" -c \
  "SELECT id, family_id, revoked_at IS NOT NULL AS revoked FROM refresh_tokens ORDER BY id;"'
#   every row of the family at revoked = t
```


### CSRF

```bash
# without the header => 403
curl -k -s -o /dev/null -w 'without: %{http_code}\n' -b /tmp/c.txt \
  -X POST https://localhost/api/v1/auth/logout

# with the header copied from the cookie => 204
CSRF=$(grep csrf_token /tmp/c.txt | awk '{print $7}')
curl -k -s -o /dev/null -w 'with: %{http_code}\n' -b /tmp/c.txt \
  -H "X-CSRF-Token: $CSRF" -X POST https://localhost/api/v1/auth/logout
```

### Rate-limit

> The bucket is keyed on the client IP and is **shared with every previous
> login** - test 1 included, and every manual attempt. It refills at 1 token
> every 12 s (`login_limiter = TokenBucket(capacity=5, refill_per_second=1/12)`),
> so you must **wait ~60 s after the last login** to start from a full bucket.
> Without that wait you will see fewer 401s than expected: that is not a bug, it
> is the limit having already bitten.
>
> The second bucket, `acct:joe@joe.com`, drains in parallel at the same rate;
> the IP one is what trips first here, since both start at 5.

```bash
sleep 60
for i in $(seq 1 10); do
  curl -k -s -o /dev/null -w "%{http_code} " -X POST https://localhost/api/v1/auth/login \
    -H "Content-Type: application/json" -d '{"email":"joe@joe.com","password":"wrongpass"}'
done; echo
#   expected: 401 401 401 401 401 429 429 429 429 429
```

Both sources of 429 (IP bucket and account bucket) must set `Retry-After`:

```bash
for i in $(seq 1 7); do
  curl -k -s -o /dev/null -D - -X POST https://localhost/api/v1/auth/login \
    -H 'Content-Type: application/json' \
    -d '{"email":"joe@joe.com","password":"wrongpass"}' | grep -iE '^HTTP|^retry-after'
done
```

And `/register` has its own, stricter limit (`capacity=3`, 1 token/min - a
registration costs a full Argon2):

```bash
for i in $(seq 1 5); do
  curl -k -s -o /dev/null -w "%{http_code} " -X POST https://localhost/api/v1/auth/register \
    -H 'Content-Type: application/json' \
    -d "{\"email\":\"r$i@joe.com\",\"password\":\"Joejoejoe-42\",\"display_name\":\"reg$i\"}"
done; echo
#   expected: 201 201 201 429 429
```

### Logout actually works

```bash
CSRF=$(grep csrf_token /tmp/c.txt | awk '{print $7}')
curl -k -s -o /dev/null -w 'after logout: %{http_code}\n' \
  -b /tmp/c.txt -H "X-CSRF-Token: $CSRF" -X POST https://localhost/api/v1/auth/refresh
#   401 - if you get 200, `_clear_auth_cookies` has the wrong `path`
```

### Race on rotation

Two simultaneous `/refresh` calls carrying the same token must never both
succeed - that is what the `.with_for_update()` on the SELECT guarantees:

```bash
curl -k -s -c /tmp/r.txt -X POST https://localhost/api/v1/auth/login \
  -H 'Content-Type: application/json' \
  -d '{"email":"joe@joe.com","password":"Joejoejoe-42"}' > /dev/null
CSRF=$(grep csrf_token /tmp/r.txt | awk '{print $7}')
for i in 1 2; do
  curl -k -s -o /dev/null -w "%{http_code} " -b /tmp/r.txt \
    -H "X-CSRF-Token: $CSRF" -X POST https://localhost/api/v1/auth/refresh &
done; wait; echo
#   expected: 200 401 (in any order) - never 200 200
```



## Verify roles and permissions



## Verify profile, avatars, friends


> **Open a FRESH session - this login is not optional.**
> `/tmp/c.txt` is the cookie jar from 4.3/4.4, and these tests kill it on purpose: replay
> detection revokes the whole family, then "logout actually works" finishes off the rest.
> Even intact, the access token only lives 15 min. Reusing the `$CSRF`/`$AUTH` of an
> earlier block gives `{"detail":"unauthenticated"}` on EVERY call below - and the mass
> assignment test then prints nothing at all: its `grep` does not find `"role"` in
> `{"detail":"unauthenticated"}`. A silent false negative, not to be read as a pass.
>
> Watch out for the `login_limiter` too (capacity 5, refill 1/12 s): chaining re-runs of
> this block ends in 429, which is not a test failure.

```bash
# fresh session, in ITS OWN jar - note the -c, not just the -b
curl -k -s -c /tmp/c45.txt -X POST https://localhost/api/v1/auth/login \
  -H 'Content-Type: application/json' \
  -d '{"email":"joe@joe.com","password":"Joejoejoe-42"}' > /dev/null

CSRF=$(grep csrf_token /tmp/c45.txt | awk '{print $7}')
AUTH=(-b /tmp/c45.txt -H "X-CSRF-Token: $CSRF")

#  no point going further if the session is not alive
curl -k -s -o /dev/null -w 'session: %{http_code}\n' -b /tmp/c45.txt \
  https://localhost/api/v1/auth/me

#   200 expected. 401 => the login above failed, re-read it before continuing
```

### Profile

```bash
curl -k -s "${AUTH[@]}" -X PATCH https://localhost/api/v1/users/me \
  -H 'Content-Type: application/json' -d '{"bio":"hello","locale":"fr"}' | python3 -m json.tool

# mass assignment: `role` must be IGNORED (extra field not declared in UserUpdate)
curl -k -s "${AUTH[@]}" -X PATCH https://localhost/api/v1/users/me \
  -H 'Content-Type: application/json' -d '{"role":"admin"}' | grep -o '"role":"[a-z]*"'

#   expected: "role":"student"
#   EMPTY output is not a pass: it means the session is dead
```

### Avatar

```bash
python3 -c "
from PIL import Image; Image.new('RGB',(1200,900),(30,90,160)).save('/tmp/a.png')"
AVATAR_PATH=$(curl -k -s "${AUTH[@]}" -F file=@/tmp/a.png \
  https://localhost/api/v1/users/me/avatar \
  | python3 -c 'import json,sys; print(json.load(sys.stdin)["avatar_path"])')
echo "$AVATAR_PATH"        # /media/avatars/<32 hex>.webp

# nginx does serve the file (location /media/, volume mounted :ro)
curl -k -s -o /dev/null -w 'avatar served: %{http_code}\n' "https://localhost$AVATAR_PATH"
#   200

# the file written is really WebP, not the original PNG.
# `file` is not installed in the api image: Pillow is what answers here.
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec -T api python -c "
from PIL import Image; import glob
for p in glob.glob('/srv/media/avatars/*'):
    with Image.open(p) as im: print(p, im.format, im.size)
"
#   ... WEBP (512, 384)  for the 1200x900 input.
#   512 is the LONG side, not a square: `thumbnail()` preserves the ratio.


# polyglot file: a valid GIF containing HTML+JS
printf 'GIF89a<html><script>alert(1)</script>' > /tmp/poly.gif
curl -k -s -o /dev/null -w 'polyglot: %{http_code}\n' "${AUTH[@]}" \
  -F file=@/tmp/poly.gif https://localhost/api/v1/users/me/avatar
#   415 - and even if it got through, re-encoding would keep nothing but pixels.
#   The 415 comes from the `except UnidentifiedImageError` in `_process_sync`, NOT
#   from the whitelist: Pillow never identifies this file, so `fmt not in
#   ALLOWED_FORMATS` is never even reached. A 500 here means that guard broke.


# too large
dd if=/dev/zero of=/tmp/big.bin bs=1M count=5 2>/dev/null
curl -k -s -o /dev/null -w 'large: %{http_code}\n' "${AUTH[@]}" \
  -F file=@/tmp/big.bin https://localhost/api/v1/users/me/avatar
#   413 (from read_limited) or 413 from nginx (client_max_body_size 2m) - either is fine
```


The three branches of `_process_sync` must stay distinguishable - a badly ordered
`except` would make them all land in the same place:

```bash
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec -T api python -c "
import io
from PIL import Image
from fastapi import HTTPException
from app.services.media import _process_sync

def probe(name, raw):
    try:
        _process_sync(raw); print(name, '-> OK')
    except HTTPException as e:
        print(name, '->', e.status_code, e.detail)

buf = io.BytesIO(); Image.new('RGB',(20,20)).save(buf, format='PNG')
probe('valid png   ', buf.getvalue())
buf = io.BytesIO(); Image.new('RGB',(20,20)).save(buf, format='BMP')
probe('bmp rejected', buf.getvalue())
probe('polyglot    ', b'GIF89a<html><script>alert(1)</script>')
"
#   valid png    -> OK
#   bmp rejected -> 415 format not allowed: BMP        (whitelist)
#   polyglot     -> 415 unreadable image               (decode guard)
# none of these probes may raise anything other than an HTTPException
```

### Race friends A<->B

```bash
# two distinct sessions: /tmp/c1.txt and /tmp/c2.txt do not exist yet,
# and neither do $C1/$C2. Create them, otherwise both curls run without a cookie
# and return 401 - which looks deceptively like "no duplicate was created".
# The register is here because a `docker compose down -v` between two runs empties
# `users`: the accounts from 4.3/4.4 may be gone. If they already exist, the 409
# is harmless, the login that follows is what counts


for u in joe bob; do
  curl -k -s -o /dev/null -X POST https://localhost/api/v1/auth/register \
    -H 'Content-Type: application/json' \
    -d "{\"email\":\"$u@joe.com\",\"password\":\"Joejoejoe-42\",\"display_name\":\"$u\"}"
done

login() {   # login <jar> <email>  -> writes the jar, prints the CSRF
  curl -k -s -c "$1" -X POST https://localhost/api/v1/auth/login \
    -H 'Content-Type: application/json' \
    -d "{\"email\":\"$2\",\"password\":\"Joejoejoe-42\"}" > /dev/null
  grep csrf_token "$1" | awk '{print $7}'
}
C1=$(login /tmp/c1.txt joe@joe.com)
C2=$(login /tmp/c2.txt bob@joe.com)

# the targeted ids must EXIST: otherwise the users FK would reject the insert, and
# the IntegrityError would be misread as the collision we are trying to provoke
A=$(curl -k -s -b /tmp/c1.txt https://localhost/api/v1/auth/me \
  | python3 -c 'import json,sys; print(json.load(sys.stdin)["id"])')
B=$(curl -k -s -b /tmp/c2.txt https://localhost/api/v1/auth/me \
  | python3 -c 'import json,sys; print(json.load(sys.stdin)["id"])')
echo "A=$A B=$B"

# the race itself: crossed requests, simultaneous.
# `-w` on the HTTP code: without it, a 500 reads as "Internal Server Error" in the
# middle of the other request's JSON and passes for noise.
curl -k -s -w ' <- %{http_code}\n' -b /tmp/c1.txt -H "X-CSRF-Token: $C1" -X POST \
  https://localhost/api/v1/friends/requests -H 'Content-Type: application/json' -d "{\"addressee_id\":$B}" &
curl -k -s -w ' <- %{http_code}\n' -b /tmp/c2.txt -H "X-CSRF-Token: $C2" -X POST \
  https://localhost/api/v1/friends/requests -H 'Content-Type: application/json' -d "{\"addressee_id\":$A}" &
wait
#   expected: one {"status":"pending"} and one {"status":"accepted"}, both 201.
#   The loser of the INSERT is the one who accepts: it finds the reciprocal
#   request and flips it. A 500 here = the rollback trap described below.

docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db sh -c 'psql -U "$(cat /run/secrets/POSTGRES_USER)" -d "$POSTGRES_DB" -c \
  "SELECT id, requester_id, addressee_id, status FROM friendships;"'
#   EXACTLY ONE row, status = accepted

# the two other paths that go through the SAME rollback, to check as well:

curl -k -s -w ' <- %{http_code}\n' -b /tmp/c1.txt -H "X-CSRF-Token: $C1" -X POST \
  https://localhost/api/v1/friends/requests -H 'Content-Type: application/json' -d "{\"addressee_id\":$B}"

# 409 request already exists


curl -k -s -w ' <- %{http_code}\n' -b /tmp/c1.txt -H "X-CSRF-Token: $C1" -X POST \
  https://localhost/api/v1/friends/requests -H 'Content-Type: application/json' -d "{\"addressee_id\":$A}"
  # 400 invalid request (not_self)
```

## Interaction utilisateur — profil, chat, blocage

```bash
# deux sessions joe/bob comme ci-dessus -> $C1/$C2, $A/$B

# profil public (pas d'email), relation vue par l'appelant
curl -k -s -b /tmp/c1.txt "https://localhost/api/v1/users/$B" | python3 -m json.tool
#   {... "online": false, "friendship": "none"}   pas de champ "email"

# recherche pour trouver quelqu'un
curl -k -s -b /tmp/c1.txt "https://localhost/api/v1/users?q=bob"

# chat : envoyer, lister, lire
curl -k -s -b /tmp/c1.txt -H "X-CSRF-Token: $C1" -X POST \
  https://localhost/api/v1/chat/messages -H 'Content-Type: application/json' \
  -d "{\"recipient_id\":$B,\"body\":\"salut\"}"
curl -k -s -b /tmp/c2.txt https://localhost/api/v1/chat/conversations | python3 -m json.tool
#   [{... "unread": 1, "last_message": {"body": "salut", ...}}]
curl -k -s -b /tmp/c2.txt "https://localhost/api/v1/chat/with/$A" | python3 -m json.tool
#   items oldest-first ; ouvre le fil => marque lu, unread repasse à 0

# blocage : coupe le chat et les demandes, seul le bloqueur lève
curl -k -s -b /tmp/c2.txt -H "X-CSRF-Token: $C2" -X POST \
  https://localhost/api/v1/friends/block -H 'Content-Type: application/json' -d "{\"user_id\":$A}"
curl -k -s -o /dev/null -w 'blocked send: %{http_code}\n' -b /tmp/c1.txt -H "X-CSRF-Token: $C1" \
  -X POST https://localhost/api/v1/chat/messages -H 'Content-Type: application/json' \
  -d "{\"recipient_id\":$B,\"body\":\"?\"}"
#   403
curl -k -s -o /dev/null -w 'non-blocker unblock: %{http_code}\n' -b /tmp/c1.txt -H "X-CSRF-Token: $C1" \
  -X DELETE "https://localhost/api/v1/friends/block/$B"
#   404
curl -k -s -o /dev/null -w 'blocker unblock: %{http_code}\n' -b /tmp/c2.txt -H "X-CSRF-Token: $C2" \
  -X DELETE "https://localhost/api/v1/friends/block/$A"
#   204
```
