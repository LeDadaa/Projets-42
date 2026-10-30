## Testing models

### Procedure for full testing

```bash
echo '"""THROWAWAY testmodel, to be delete"""

from __future__ import annotations

from sqlalchemy import Index, String
from sqlalchemy.orm import Mapped, mapped_column

from app.models.base import Base, TimestampMixin
from app.models.enums import Role, enum_column


class Testmodel(Base, TimestampMixin):
    __tablename__ = "testmodel"

    id: Mapped[int] = mapped_column(primary_key=True)
    label: Mapped[str] = mapped_column(String(50), unique=True, nullable=False)
    role: Mapped[Role] = mapped_column(enum_column(Role, "role"), nullable=False)

    __table_args__ = (Index("ix_testmodel_role", "role"),)
' > srcs/backend/app/mdoels/testmodel.py
```

And add Testmodel in import and in __all__ in models/__init__.py

```bash
make dev
# docker-compose.dev.yml bind-mounts ./backend/migrations, so files that alembic
# generates inside the container appear on host and survive `down`
```

```bash
# at first
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic current     # empty = no migration applied yet
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic history     # empty = versions/ has no scripts

# autogenerate
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic revision --autogenerate -m "throwaway testmodel"
```

If this errors with `Target database is not up to date`, run `alembic upgrade head` first —
autogenerate refuses to diff against a database that has pending migrations

```bash
cat srcs/backend/migrations/versions/*_throwaway_testmodel.py
```

Need `op.create_table("testmodel", ...)` with `id`, `label`, `role`,
`created_at`, `updated_at`; a `CheckConstraint` named `ck_testmodel_role`; a unique constraint
`uq_testmodel_label`; an index `ix_testmodel_role`. And a `downgrade()` that drops them.
**If `op.drop_table()` on anything, stop** — metadata isn't loading.

```bash
# apply
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic upgrade head
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic current      # now shows the revision id

# verify in psql
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db psql -U POSTGRES_USER -d pgdb
```

(`POSTGRES_USER` secret file literally contains the string `POSTGRES_USER`, and
`POSTGRES_DB=pgdb`. To avoid hardcoding, `docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db sh -c 'psql -U "$(cat /run/secrets/POSTGRES_USER)" -d "$POSTGRES_DB"'`.)

Inside psql:

```sql
\dt                          -- testmodel and alembic_version are listed
\d testmodel                     -- columns, PK, index, and the CHECK constraint
SELECT * FROM alembic_version;  -- one row: the revision id
```

The line you specifically want to see in `\d testmodel`:

```
Check constraints:
    "ck_testmodel_role" CHECK (role::text = ANY (ARRAY['guest', 'student', 'author', 'moderator', 'admin']))
```

Its presence is the proof that `create_constraint=True` did its job. Without A2, this ection is absent and the column is an unvalidated `character varying(32)`.

Prove the constraint actually bites:

```sql
INSERT INTO testmodel (label, role) VALUES ('x', 'not_a_role');
-- ERROR:  new row for relation "testmodel" violates check constraint "ck_testmodel_role"
```

```bash
# downgrade
\q
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic downgrade -1

# verify disappearance
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db psql -U POSTGRES_USER -d pgdb -c '\dt'
#   testmodel is gone; alembic_version remains (it is alembic's own bookkeeping)
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec db psql -U POSTGRES_USER -d pgdb -c 'SELECT * FROM alembic_version;'
#   zero rows — back at base
```

Zero rows rather than a previous id is expected: this was the first migration, so `downgrade -1` returns to base.

```bash
# delete the dummy
rm srcs/backend/migrations/versions/*_throwaway_testmodel.py
rm srcs/backend/app/models/testmodel.py
# then remove the testmodel import and __all__ entry from app/models/__init__.py

docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic history    # empty again
docker compose -f srcs/docker-compose.yml -f srcs/docker-compose.dev.yml exec api alembic current    # empty again
```

### Commands

```bash
alembic current                 # revision the DB is on
alembic history --verbose       # full chain with down_revision links
alembic heads                   # >1 head means branched migrations, must be merged
alembic upgrade head --sql      # print SQL without executing (offline mode)
alembic downgrade base          # unwind everything
```
