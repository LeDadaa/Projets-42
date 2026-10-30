import pytest

from app.i18n import DEFAULT, parse_accept_language, resolve_locale


@pytest.mark.parametrize(
    "header,expected",
    [
        ("fr", ["fr"]),
        ("fr-CA,fr;q=0.9,en;q=0.8", ["fr-ca", "fr", "en"]),
        ("en;q=0.5,fr;q=0.9", ["fr", "en"]),           # sorted by q, not by order
        ("de,fr;q=0", ["de"]),                         # q=0 removes the language
        ("", []),
        ("fr;q=notanumber", []),                       # unreadable q -> discarded
        ("en,fr", ["en", "fr"]),                       # equal q -> order of appearance
    ],
)
def test_parse(header: str, expected: list[str]) -> None:
    assert parse_accept_language(header) == expected


def test_resolution_priority() -> None:
    # the URL parameter wins over everything
    assert resolve_locale(query="es", user_locale="fr", accept_language="en") == "es"
    # then the user preference
    assert resolve_locale(query=None, user_locale="fr", accept_language="en") == "fr"
    # then the header
    assert resolve_locale(accept_language="es;q=0.9,en;q=0.1") == "es"
    # then the default
    assert resolve_locale() == DEFAULT


def test_region_fallback() -> None:
    assert resolve_locale(accept_language="fr-CA") == "fr"


def test_unsupported_language_ignored() -> None:
    assert resolve_locale(query="de", accept_language="de,fr;q=0.5") == "fr"
