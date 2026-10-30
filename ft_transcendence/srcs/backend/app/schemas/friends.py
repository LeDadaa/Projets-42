"""Friends request and response contracts

Kept in one place so every `friends.py` route carries a `response_model` and
therefore shows up in the OpenAPI schema — the "API publique avec
documentation" module counts undocumented routes against us.
"""

from __future__ import annotations

from datetime import datetime

from pydantic import BaseModel

from app.schemas.user import UserSummary


class FriendRequestCreate(BaseModel):
    """Body of `POST /friends/requests`

    A Pydantic body, not a query string parameter: every other write endpoint
    takes its input this way and the inconsistency was noted in BACKEND.md.
    """

    addressee_id: int


class BlockCreate(BaseModel):
    """Body of `POST /friends/block`"""

    user_id: int


class FriendActionOut(BaseModel):
    """What a request/accept returns: the resulting status and the row id"""

    status: str
    friendship_id: int


class FriendOut(BaseModel):
    """One accepted friend, with everything the list view needs to render

    `online` comes from the in-memory hub, `last_seen_at` from the DB: a friend
    can be online without ever having disconnected, so the hub is checked first.
    """

    friendship_id: int
    user: UserSummary
    online: bool
    last_seen_at: datetime | None


class FriendListOut(BaseModel):
    items: list[FriendOut]


class PendingRequestOut(BaseModel):
    """A friend request still awaiting an answer, either direction"""

    friendship_id: int
    user: UserSummary
    created_at: datetime


class PendingRequestsOut(BaseModel):
    """Split so the UI can show "asked you" and "you asked" separately"""

    incoming: list[PendingRequestOut]
    outgoing: list[PendingRequestOut]
