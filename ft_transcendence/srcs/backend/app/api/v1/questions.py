"""Questions: the public read side, search and detail

Every question comes from the versioned content files in `seeds/content/`, so
nothing here writes: there is no authoring or moderation route left to guard
"""

from __future__ import annotations

from typing import Annotated

from fastapi import APIRouter, HTTPException, Query

from app.deps import DbSession, Locale, OptionalUser
from app.i18n import translated_questions
from app.models.content import Question
from app.models.enums import QuestionStatus, Role
from app.schemas.question import QuestionDetail
from app.schemas.search import QuestionFilters, QuestionPage
from app.services.search import search_questions

router = APIRouter(prefix="/questions", tags=["questions"])


@router.get("", response_model=QuestionPage)
async def list_questions(
    filters: Annotated[QuestionFilters, Query()],
    user: OptionalUser,
    db: DbSession,
    locale: Locale,
):
    """Searches filters sorts and paginates questions

    Open to anonymous callers who only ever see published questions
    Asking for a status you cannot see returns an empty page not a substitute

    `?locale=` wins over the negotiated language
    so a caller can preview another language without changing their account
    """
    return await search_questions(db, filters, user, filters.locale or locale)


@router.get("/{question_id}", response_model=QuestionDetail)
async def get_question(
    question_id: int, user: OptionalUser, db: DbSession, locale: Locale
):
    """Returns one question in the caller's language

    Open to anonymous callers, who only ever see published questions
    Anything else answers 404, never 403, so the existence of an unpublished
    question is not confirmed to someone who may not read it
    `?locale=` is honoured by the `Locale` dependency itself
    """
    row = (await db.execute(
        translated_questions(locale)
        .add_columns(
            Question.slug, Question.topic_id, Question.kind,
            Question.status, Question.difficulty_rating,
        )
        .where(Question.id == question_id)
    )).one_or_none()
    if row is None:
        raise HTTPException(404, detail="not found")

    is_moderator = user is not None and user.role >= Role.moderator
    if row.status != QuestionStatus.published and not is_moderator:
        raise HTTPException(404, detail="not found")

    return QuestionDetail(
        id=row.id,
        slug=row.slug,
        title=row.title,
        topic_id=row.topic_id,
        kind=row.kind,
        status=row.status,
        difficulty_rating=row.difficulty_rating,
        stem=row.stem,
        is_fallback=row.is_fallback,
    )
