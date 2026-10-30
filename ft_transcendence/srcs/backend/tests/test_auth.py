from __future__ import annotations

import pytest
from httpx import AsyncClient


class TestRegister:
    async def test_success(self, client: AsyncClient) -> None:
        r = await client.post(
            "/api/v1/auth/register",
            json={
                "display_name": "alice",
                "email": "alice@example.com",
                "password": "Correct-horse-battery-9",
                "accept_terms": True,
            },
        )
        assert r.status_code == 201
        body = r.json()
        assert body["display_name"] == "alice"
        assert body["email"] == "alice@example.com"
        assert body["role"] == "student"
        assert "password_hash" not in body

    async def test_duplicate_email_is_case_insensitive(self, client: AsyncClient, user_factory) -> None:
        await user_factory(email="bob@example.com", display_name="bob")

        r = await client.post(
            "/api/v1/auth/register",
            json={
                "display_name": "someone_else",
                "email": "BOB@EXAMPLE.com",
                "password": "Correct-horse-battery-9",
                "accept_terms": True,
            },
        )
        assert r.status_code == 409
        assert r.json()["detail"] == "email already used"

    async def test_duplicate_display_name(self, client: AsyncClient, user_factory) -> None:
        await user_factory(display_name="carol", email="carol@example.com")

        r = await client.post(
            "/api/v1/auth/register",
            json={
                "display_name": "carol",
                "email": "other@example.com",
                "password": "Correct-horse-battery-9",
                "accept_terms": True,
            },
        )
        assert r.status_code == 409
        assert r.json()["detail"] == "display name already used"

    @pytest.mark.parametrize(
        "password,expected_status",
        [
            pytest.param("Trombone9", 422, id="9-chars-rejected"),
            pytest.param("Trombone-9", 201, id="10-chars-accepted"),
        ],
    )
    async def test_password_length_boundary(
        self, client: AsyncClient, password: str, expected_status: int
    ) -> None:
        r = await client.post(
            "/api/v1/auth/register",
            json={"display_name": "boundary", "email": "boundary@example.com", "password": password, "accept_terms": True},
        )
        assert r.status_code == expected_status

    @pytest.mark.parametrize(
        "password",
        [
            pytest.param("TROMBONE-99", id="no-lowercase"),
            pytest.param("trombone-99", id="no-uppercase"),
            pytest.param("Trombone-xx", id="no-digit"),
            pytest.param("Trombone999", id="no-special"),
        ],
    )
    async def test_password_needs_every_class(self, client: AsyncClient, password: str) -> None:
        r = await client.post(
            "/api/v1/auth/register",
            json={
                "display_name": "weakling",
                "email": "weakling@example.com",
                "password": password,
                "accept_terms": True,
            },
        )
        assert r.status_code == 422

    @pytest.mark.parametrize(
        "extra",
        [pytest.param({}, id="missing"), pytest.param({"accept_terms": False}, id="refused")],
    )
    async def test_terms_must_be_accepted(self, client: AsyncClient, extra: dict) -> None:
        r = await client.post(
            "/api/v1/auth/register",
            json={
                "display_name": "noterms",
                "email": "noterms@example.com",
                "password": "Correct-horse-battery-9",
                **extra,
            },
        )
        assert r.status_code == 422


class TestLogin:
    async def test_success_sets_cookies(self, client: AsyncClient, user_factory) -> None:
        _, password = await user_factory(email="dave@example.com", display_name="dave")

        r = await client.post("/api/v1/auth/login", json={"email": "dave@example.com", "password": password})

        assert r.status_code == 200
        assert "access_token" in client.cookies
        assert "refresh_token" in client.cookies

    async def test_wrong_password(self, client: AsyncClient, user_factory) -> None:
        await user_factory(email="erin@example.com", display_name="erin")

        r = await client.post(
            "/api/v1/auth/login", json={"email": "erin@example.com", "password": "definitely wrong"}
        )
        assert r.status_code == 401
        assert r.json()["detail"] == "Invalid login"

    async def test_unknown_email_gives_same_message_as_wrong_password(self, client: AsyncClient) -> None:
        r = await client.post(
            "/api/v1/auth/login", json={"email": "ghost@example.com", "password": "whatever123"}
        )
        assert r.status_code == 401
        assert r.json()["detail"] == "Invalid login"

    async def test_disabled_account(self, client: AsyncClient, user_factory) -> None:
        _, password = await user_factory(email="frank@example.com", display_name="frank", is_active=False)

        r = await client.post("/api/v1/auth/login", json={"email": "frank@example.com", "password": password})
        assert r.status_code == 401


class TestMe:
    async def test_without_cookie(self, client: AsyncClient) -> None:
        r = await client.get("/api/v1/auth/me")
        assert r.status_code == 401

    async def test_with_garbage_cookie(self, client: AsyncClient) -> None:
        client.cookies.set("access_token", "not-a-real-jwt")
        r = await client.get("/api/v1/auth/me")
        assert r.status_code == 401

    async def test_with_valid_session(self, client: AsyncClient, user_factory) -> None:
        _, password = await user_factory(email="grace@example.com", display_name="grace")
        await client.post("/api/v1/auth/login", json={"email": "grace@example.com", "password": password})

        r = await client.get("/api/v1/auth/me")

        assert r.status_code == 200
        body = r.json()
        assert body["email"] == "grace@example.com"
        assert "password_hash" not in body
