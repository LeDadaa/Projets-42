"""Topics: the exam hierarchy, read only

Topics come from the versioned content files, like questions, so nothing here
writes
"""

from __future__ import annotations

from fastapi import APIRouter

from app.deps import DbSession, Locale
from app.schemas.topic import TopicOut
from app.services.content import topic_tree

router = APIRouter(prefix="/topics", tags=["topics"])


@router.get("", response_model=list[TopicOut])
async def list_topics(db: DbSession, locale: Locale):
    """Returns the whole topic hierarchy in the caller's language

    Open to anonymous callers, like the question search it is used to filter
    `?locale=` is honoured by the `Locale` dependency itself
    """
    return await topic_tree(db, locale)
