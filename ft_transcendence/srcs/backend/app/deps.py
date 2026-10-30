from __future__ import annotations

from typing import Annotated

import jwt
from fastapi import Cookie, Depends, HTTPException, WebSocket, WebSocketException, status, Request
from sqlalchemy.ext.asyncio import AsyncSession

from collections.abc import Awaitable, Callable

from app.db import get_db
from app.models.user import User
from app.services.security import decode_token
from app.services.ratelimit import TokenBucket
from app.i18n import resolve_locale
from app.config import get_settings
from app.services.llm.client import OllamaClient
from app.services.llm.grading import AnswerGrader


DbSession = Annotated[AsyncSession, Depends(get_db)]


def _unauthenticated() -> HTTPException:
    """Builds the single 401 used whenever a caller is not identified"""
    return HTTPException(status.HTTP_401_UNAUTHORIZED, detail="unauthenticated")


async def get_current_user(
    db: DbSession,
    access_token: Annotated[str | None, Cookie()] = None,
) -> User:
    """Identifies the caller from the access cookie or refuses the request

    Re reads the account each time because a token proves the past not the present
    """
    if access_token is None:
        raise _unauthenticated()
    try:
        payload = decode_token(access_token, expected_typ="access")
    except jwt.InvalidTokenError as exc:
        raise _unauthenticated() from exc

    user = await db.get(User, int(payload["sub"]))
    # Token can be cryptographicly valid although account is deactivated or deleted
    # so revalid against base
    # A JWT says about the past, not the present
    if user is None or not user.is_active:
        raise _unauthenticated()
    return user


CurrentUser = Annotated[User, Depends(get_current_user)]


def client_ip(request: Request) -> str:
    """Finds the real client IP behind nginx

    DO NOT read `X-Forwarded-For[0]`
    nginx gives `X-Forwarded-For $proxy_add_x_forwarded_for` which add REAL IP
    to the end of what the client sends :
    a client sends `X-Forwarded-For: 1.2.3.4` the gets `1.2.3.4, <real_ip>`
    want to avoid IP usupation to avoid limit rate

    `X-Real-IP` use `proxy_set_header` which replace the client value
    """
    return request.headers.get("x-real-ip") or (
        request.client.host if request.client else "unknown"
    )


def rate_limited(limiter: TokenBucket) -> Callable[[Request], Awaitable[None]]:
    """Builds a dependency that caps how often one IP may call a route

    Made as a factory so each route can pass its own bucket
    """

    async def _dep(request: Request) -> None:
        """Rejects the call with 429 once this IP has spent its budget"""
        key = client_ip(request)
        if not limiter.consume(key):
            raise HTTPException(
                status.HTTP_429_TOO_MANY_REQUESTS,
                detail="too many tries",
                headers={"Retry-After": str(limiter.retry_after(key))},
            )

    return _dep


async def get_optional_user(
    db: DbSession,
    access_token: Annotated[str | None, Cookie()] = None,
) -> User | None:
    """Identifies the caller if possible and returns None otherwise

    For endpoints with a public and non public versions :
    for instance a field `answered` for questions will only be filled for authenticated users
    """
    if access_token is None:
        return None
    try:
        payload = decode_token(access_token, expected_typ="access")
    except jwt.InvalidTokenError:
        return None
    user = await db.get(User, int(payload["sub"]))
    return user if user is not None and user.is_active else None


OptionalUser = Annotated[User | None, Depends(get_optional_user)]


async def get_ws_user(db: DbSession, websocket: WebSocket) -> User:
    """Identifies a websocket's owner from its handshake cookie

    Same rules as `get_current_user`, but a plain `HTTPException` cannot cross
    a websocket handshake cleanly: `WebSocketException` closes the socket
    with code 1008 instead of failing the connection with a bare 500
    """
    access_token = websocket.cookies.get("access_token")
    if access_token is None:
        raise WebSocketException(code=status.WS_1008_POLICY_VIOLATION)
    try:
        payload = decode_token(access_token, expected_typ="access")
    except jwt.InvalidTokenError as exc:
        raise WebSocketException(code=status.WS_1008_POLICY_VIOLATION) from exc

    user = await db.get(User, int(payload["sub"]))
    if user is None or not user.is_active:
        raise WebSocketException(code=status.WS_1008_POLICY_VIOLATION)
    return user


WsUser = Annotated[User, Depends(get_ws_user)]


async def get_locale(
    request: Request,
    user: OptionalUser,
    locale: str | None = None,
) -> str:
    """Picks the language for this request

    `?locale=` then the account preference then `Accept-Language` then English
    """
    return resolve_locale(
        query=locale,
        user_locale=user.locale if user else None,
        accept_language=request.headers.get("accept-language"),
    )


Locale = Annotated[str, Depends(get_locale)]


def get_grader() -> AnswerGrader | None:
    """The configured answer grader, or None when the model is switched off

    The one object tests replace: the route only knows `AnswerGrader`, which only
    knows the `LLMClient` protocol, so changing engine only touches this function
    Built per request because it is two plain objects; the HTTP connection itself
    is opened per call by the client
    """
    settings = get_settings()
    if not settings.LLM_ENABLED:
        return None
    return AnswerGrader(
        OllamaClient(
            settings.LLM_BASE_URL,
            settings.LLM_MODEL,
            proxy=settings.LLM_PROXY_URL or None,
            timeout=settings.LLM_TIMEOUT,
        )
    )


Grader = Annotated[AnswerGrader | None, Depends(get_grader)]
