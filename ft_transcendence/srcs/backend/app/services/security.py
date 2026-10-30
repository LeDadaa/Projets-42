"""Hash, JWT, opaque tokens

1. Argon2 100–300 ms pure CPU
called in `async def`, blocks events loop, no other request, websocket included,
all hash go through `asyncio.to_thread`
2. failure must take as much tinme as success, if unknown email, fake hash still checked
to avoid giving a time value
3. never `==` on a secret, `secrets.compare_digest` is time constant
"""

from __future__ import annotations

import asyncio
import hashlib
import secrets
from datetime import UTC, datetime, timedelta
from typing import Any

from functools import lru_cache
from cryptography.fernet import Fernet

import jwt
from pwdlib import PasswordHash
from pwdlib.hashers.argon2 import Argon2Hasher

from app.config import get_settings

settings = get_settings()

ALGORITHM = "HS256"

_hasher = PasswordHash((Argon2Hasher(),))

# Hash once when loading module
# dummy_verify : cost as much as a real verif
_DUMMY_HASH = _hasher.hash("timing-equalisation-dummy-password")


# PASSWORD

def _hash_sync(password: str) -> str:
    """Argon2 hashing the blocking way

    Never call this straight from async code
    """
    return _hasher.hash(password)


def _verify_sync(password: str, password_hash: str) -> bool:
    """Argon2 checking the blocking way"""
    return _hasher.verify(password, password_hash)


async def hash_password(password: str) -> str:
    """Hashes a password for storage

    Runs in a thread so 100 to 300 ms of Argon2 does not freeze the server
    """
    return await asyncio.to_thread(_hash_sync, password)


async def verify_password(password: str, password_hash: str | None) -> bool:
    """Checks a password against its stored hash

    Falls back to the dummy hash so a NULL hash still costs the same time
    """
    return await asyncio.to_thread(_verify_sync, password, _DUMMY_HASH if not password_hash else password_hash)


async def dummy_verify(password: str) -> bool:
    """Burns as much CPU as a real `verify_password` then fails anyway

    Called when unknown email or `password_hash` is NULL
    (OAuth pure)
    Otherwise unknown email in 1ms and wrong password in 200 ms
    gap proves email exists
    """
    await asyncio.to_thread(_verify_sync, password, _DUMMY_HASH)
    return False


# JWT

def _now() -> datetime:
    """Current time as an aware UTC datetime"""
    return datetime.now(UTC)


def _encode(payload: dict[str, Any], ttl: int, typ: str) -> str:
    """Signs a payload into a JWT

    Stamps the type the issue time and the expiry every token must carry
    """
    now = _now()
    payload |= {
        "typ": typ,
        "iat": int(now.timestamp()),
        "exp": int((now + timedelta(seconds=ttl)).timestamp()),
    }
    return jwt.encode(payload, settings.SECRET_KEY, algorithm=ALGORITHM)


def create_access_token(user_id: int, role: str) -> str:
    """Makes the short lived token that proves who you are

    Carries the role so most checks need no database read
    """
    # `sub` is a chain (RFC 7519) : PyJWT rejets an int.
    return _encode({"sub": str(user_id), "role": role}, settings.ACCESS_TOKEN_TTL, "access")


def create_partial_token(user_id: int) -> str:
    """Makes the half way token issued once the password is right

    Proves the password step passed but opens no session until the code is given
    """
    return _encode({"sub": str(user_id)}, settings.MFA_TOKEN_TTL, "mfa")


def decode_token(token: str, expected_typ: str = "access") -> dict[str, Any]:
    """Decodes a token and checks it is the type expected

    Raises `jwt.InvalidTokenError` on anything wrong

    2 security points :

    - `algorithms=[ALGORITHM]` explicit list, never read the algo from the token head
    algorithm confusion attack (`alg: none`, or HS256 signed with public RSA).
    - without `expected_typ`, partial token emitted before the next would be acceptedas complete

    """
    payload = jwt.decode(
        token,
        settings.SECRET_KEY,
        algorithms=[ALGORITHM],
        options={"require": ["exp", "iat", "sub", "typ"]},
    )
    if payload.get("typ") != expected_typ:
        raise jwt.InvalidTokenError(f"expected token type : {expected_typ!r}")
    return payload


# opaque token

def generate_opaque_token() -> str:
    """Draws a refresh token as pure randomness never a JWT

    A refresh token must be revocable immediatly
    A JWT isn't,remains valid until expired, whatever happens on server side
    Thus opaque token
    """
    return secrets.token_urlsafe(32)


def hash_opaque_token(token: str) -> str:
    """Hashes a refresh token for storage with SHA-256 not Argon2

    Argon2 meant to be slow, to resist a password attack with weak entropy
    `token_urlsafe(32)` already has 256 bits entropy
    cannot brute force, SHA-256 costs ~1 µs is enough
    Argon2 would add 200 ms to each refresh for nothing
    """
    return hashlib.sha256(token.encode()).hexdigest()


def generate_csrf_token() -> str:
    """Draws a CSRF token which is random and carries no meaning

    only compared to itself (cookie vs header)
    Skill : unpredictable
    """
    return secrets.token_urlsafe(32)


@lru_cache
def _fernet() -> Fernet:
    """Builds the cipher used for TOTP secrets

    Cached so the key is read once
    Refuses to run without a configured key rather than storing secrets in clear
    """
    if not settings.TOTP_ENCRYPTION_KEY:
        raise RuntimeError(
            "TOTP_ENCRYPTION_KEY missing: make one with Fernet.generate_key()"
        )
    return Fernet(settings.TOTP_ENCRYPTION_KEY.encode())


def encrypt_totp_secret(secret: str) -> str:
    """Encrypts a TOTP secret before it goes to the database

    Encrypted not hashed because the server must read it back to check codes
    """
    return _fernet().encrypt(secret.encode()).decode()


def decrypt_totp_secret(blob: str) -> str:
    """Reads a stored TOTP secret back"""
    return _fernet().decrypt(blob.encode()).decode()

def generate_backup_code() -> str:
    """Human-typable recovery code.

    Low entropy (10 hex chars) is acceptable only because it's checked
    against an Argon2 hash (see hash_password) — same reasoning as a
    password, not as a refresh token (cf. hash_opaque_token).
    """
    return secrets.token_hex(5)
