from __future__ import annotations

from collections.abc import AsyncGenerator, Awaitable, Callable

import pytest
from httpx import ASGITransport, AsyncClient
from sqlalchemy.ext.asyncio import AsyncSession

from app.db import engine, get_db
from app.main import app
from app.models.user import User
from app.services.ratelimit import grading_limiter, login_limiter, register_limiter, totp_limiter
from app.services.attempts import llm_budget
from app.services.security import hash_password


@pytest.fixture(autouse=True)
def reset_rate_limiters() -> None:
    """Empties the token buckets before every test.

    The limiters in `app.services.ratelimit` are module-level singletons, so
    their `_buckets` outlive a test, and every request here reaches the app
    through the same ASGI client — hence the same `client_ip()` key. Without
    this reset the buckets are shared by the whole session: `register_limiter`
    holds 3 tokens while the suite sends 5 `POST /register`, so the last two
    would answer 429 instead of the status under test, and which tests fail
    would depend on collection order. Also clears the grading limiter and token budget.
    """
    for limiter in (login_limiter, register_limiter, totp_limiter, grading_limiter):
        limiter._buckets.clear()
    llm_budget.clear()


@pytest.fixture
async def db() -> AsyncGenerator[AsyncSession, None]:
    """One test, one Postgres transaction, rolled back at teardown.

    `join_transaction_mode="create_savepoint"` is what makes this safe even
    though route code calls `db.commit()` (see `/auth/register`,
    `/auth/login`): each app-level commit only releases a SAVEPOINT, the
    outer transaction started here stays open until the explicit rollback
    below, so nothing a test does is ever actually persisted. Without it, a
    route's own `commit()` would end the outer transaction and the
    rollback below would have nothing left to undo.
    """
    async with engine.connect() as conn:
        trans = await conn.begin()
        session = AsyncSession(
            bind=conn, join_transaction_mode="create_savepoint", expire_on_commit=False
        )
        try:
            yield session
        finally:
            await session.close()
            await trans.rollback()


@pytest.fixture
async def client(db: AsyncSession) -> AsyncGenerator[AsyncClient, None]:
    """httpx client hitting the FastAPI app in-process (`ASGITransport` —
    no real socket), with `get_db` swapped for the test's transactional
    session.

    `base_url="https://test"`, not `http://`: the login route's cookies
    carry `Secure`, and httpx's cookie jar (like a real browser) refuses to
    resend a `Secure` cookie on a plain `http://` request. `ASGITransport`
    never touches the network either way, so the scheme only matters for
    this cookie check.
    """
    app.dependency_overrides[get_db] = lambda: db
    transport = ASGITransport(app=app)
    async with AsyncClient(transport=transport, base_url="https://test") as c:
        yield c
    app.dependency_overrides.clear()


@pytest.fixture
def user_factory(
    db: AsyncSession,
) -> Callable[..., Awaitable[tuple[User, str]]]:
    """Inserts a `User` row directly (bypassing `/auth/register`).

    Returns `(user, plaintext_password)` — the plaintext is needed for a
    subsequent login test and can't be recovered from the hash.
    """

    async def _make(
        *,
        display_name: str = "testuser",
        email: str = "test@example.com",
        password: str = "correcthorsebatterystaple",
        **extra: object,
    ) -> tuple[User, str]:
        user = User(
            display_name=display_name,
            email=email,
            password_hash=await hash_password(password),
            **extra,
        )
        db.add(user)
        await db.flush()
        await db.refresh(user)
        return user, password

    return _make


@pytest.fixture
def as_user(
    client: AsyncClient,
    user_factory: Callable[..., Awaitable[tuple[User, str]]],
) -> Callable[..., Awaitable[User]]:
    """Creates a user and logs the shared client in as them.

    The one httpx client has one cookie jar, so calling this again switches the
    session to the new user — fine for the sequential "A acts, then B acts"
    shape the chat and friends tests use. It also copies the freshly set
    `csrf_token` cookie into the `X-CSRF-Token` header so the client can make
    non-safe requests, exactly as the real frontend does.
    """

    async def _login(**kwargs: object) -> User:
        user, password = await user_factory(**kwargs)
        r = await client.post(
            "/api/v1/auth/login",
            json={"email": user.email, "password": password},
        )
        assert r.status_code == 200, r.text
        client.headers["x-csrf-token"] = client.cookies["csrf_token"]
        return user

    return _login


@pytest.fixture
def login(client: AsyncClient) -> Callable[..., Awaitable[None]]:
    """Switches the shared client to an already-created user.

    Use with `user_factory` when a test needs two accounts and has to hand the
    session back and forth between them.
    """

    async def _login(
        email: str, password: str = "correcthorsebatterystaple"
    ) -> None:
        r = await client.post(
            "/api/v1/auth/login", json={"email": email, "password": password}
        )
        assert r.status_code == 200, r.text
        client.headers["x-csrf-token"] = client.cookies["csrf_token"]

    return _login
