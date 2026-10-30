"""Loads versioned content files (TOML) into the database

Content files are reviewed in pull requests and then seeded via this module.
The process is idempotent: re-running yields the same database state,
never creating duplicates.

Idempotence stems from `ON CONFLICT DO UPDATE` on the `slug` natural key.
"""

from __future__ import annotations

import tomllib
from dataclasses import dataclass
from decimal import Decimal
from pathlib import Path
from typing import Annotated, Literal

from fastapi import HTTPException
from pydantic import BaseModel, ConfigDict, Field, ValidationError
from sqlalchemy import delete, func, select, update
from sqlalchemy.dialects.postgresql import insert as pg_insert
from sqlalchemy.ext.asyncio import AsyncSession

from app.i18n import SUPPORTED
from app.models.content import (
    Choice, ChoiceTranslation, Question, QuestionTag, QuestionTranslation, Tag,
    Topic, TopicTranslation
)
from app.models.enums import QuestionKind, QuestionStatus
from app.services.validation import assert_publishable


# kebab-case, the same shape as every other slug in the schema
SLUG = r"^[a-z0-9]+(?:-[a-z0-9]+)*$"


class ContentError(Exception):
    """Raised when a content file cannot be loaded

    The message names the file and the problematic entry for debugging.
    """

    pass


@dataclass
class LoadSummary:
    """Counts of loaded resources"""

    files: int
    topics: int
    questions: int


# --- Pydantic schema: shape of a TOML file ---


class TopicTranslationIn(BaseModel):
    """Translations of one topic into one language"""

    model_config = ConfigDict(extra="forbid")

    name: str = Field(min_length=1, max_length=120)
    description: str | None = None


class TopicIn(BaseModel):
    """A topic that organizes questions hierarchically"""

    model_config = ConfigDict(extra="forbid")

    slug: str = Field(pattern=SLUG, max_length=64)
    parent: str | None = None
    position: int = 0
    translations: dict[Literal["en", "fr", "es"], TopicTranslationIn]


class QuestionTranslationIn(BaseModel):
    """Translations of one question into one language"""

    model_config = ConfigDict(extra="forbid")

    title: str = Field(min_length=1, max_length=120)
    stem: str = Field(min_length=1)
    explanation: str | None = None


class ChoiceIn(BaseModel):
    """One possible answer of a choice question, in every language at once

    The order in the file is the order shown to players
    """

    model_config = ConfigDict(extra="forbid")

    correct: bool = False
    text: dict[Literal["en", "fr", "es"], Annotated[str, Field(min_length=1)]]


class QuestionIn(BaseModel):
    """A question: written (`free_text`) or with one right choice (`single_choice`)

    `free_text` questions are graded by the model against their key points,
    `single_choice` ones feed the multiplayer quiz. The counts of choices and
    right answers are checked by `assert_publishable`, like any other rule
    that spans several rows
    """

    model_config = ConfigDict(extra="forbid")

    slug: str = Field(pattern=SLUG, max_length=96)
    topic: str
    kind: Literal["free_text", "single_choice"]
    difficulty: Decimal = Decimal("1200")
    # The file owns the status: `draft` keeps a question hidden while it is being
    # written, `archived` retires it. `in_review` and `rejected` are left over
    # from an in-app moderation workflow that no longer exists, and are refused
    status: Literal["draft", "published", "archived"] = "published"
    # Annotated per item: a `pattern` on `list[str]` itself never reaches the
    # elements, so a bad tag is rejected here at parse time, where the
    # resulting ContentError still names the file
    tags: list[Annotated[str, Field(pattern=SLUG, max_length=64)]] = Field(default_factory=list)
    # One grid per question, written in English whatever the student's
    # language, so every student is graded on the same criteria
    key_points: list[str] = Field(default_factory=list)
    translations: dict[Literal["en", "fr", "es"], QuestionTranslationIn]
    choices: list[ChoiceIn] = Field(default_factory=list)


class ContentFile(BaseModel):
    """The schema of one TOML file in `seeds/content/`"""

    model_config = ConfigDict(extra="forbid")

    topics: list[TopicIn] = Field(default_factory=list)
    questions: list[QuestionIn] = Field(default_factory=list)


# --- Main loader ---


async def load_content(db: AsyncSession, directory: Path) -> LoadSummary:
    """Loads all TOML files from `directory` into the database.

    Does NOT commit the transaction: the caller decides to commit (persisting
    the changes) or rollback (discarding them). This separation lets the caller
    handle errors and transaction logic at a higher level.

    Raises `ContentError` on any validation or constraint violation, naming the
    file and entry. Rows may already sit in the open transaction when it
    raises: the caller must roll back, as `seeds/__main__.py` does.

    The process is idempotent: re-running against the same files produces the
    same outcome, never creating duplicates. Questions' `difficulty_rating` is
    fixed at insertion and never updated by a reload — it evolves only through
    student results. Topics and translations are upserted by slug, and tags are
    inserted with `ON CONFLICT DO NOTHING`. The status comes from each file, and
    only published questions are validated.
    """
    files = sorted(directory.glob("*.toml")) if directory.is_dir() else []

    # --- Parse all files, validating shape early ---
    all_topics: list[tuple[Path, TopicIn]] = []
    all_questions: list[tuple[Path, QuestionIn]] = []

    for path in files:
        try:
            raw = tomllib.loads(path.read_text(encoding="utf-8"))
        except tomllib.TOMLDecodeError as exc:
            raise ContentError(f"{path.name}: invalid TOML — {exc}") from exc

        try:
            content_file = ContentFile.model_validate(raw)
        except ValidationError as exc:
            raise ContentError(f"{path.name}: {exc}") from exc

        all_topics.extend((path, t) for t in content_file.topics)
        all_questions.extend((path, q) for q in content_file.questions)

    # --- Detect duplicate slugs (same slug in two places is an error) ---
    topic_slugs: dict[str, Path] = {}
    for path, topic in all_topics:
        if topic.slug in topic_slugs:
            raise ContentError(
                f"duplicate topic slug '{topic.slug}' in {topic_slugs[topic.slug].name} "
                f"and {path.name}"
            )
        topic_slugs[topic.slug] = path

    question_slugs: dict[str, Path] = {}
    for path, question in all_questions:
        if question.slug in question_slugs:
            raise ContentError(
                f"duplicate question slug '{question.slug}' in {question_slugs[question.slug].name} "
                f"and {path.name}"
            )
        question_slugs[question.slug] = path

    # --- Topics, pass 1: upsert all topics and collect IDs ---
    topic_id_by_slug: dict[str, int] = {}

    for path, topic in all_topics:
        insert_topic = pg_insert(Topic).values(slug=topic.slug, position=topic.position)
        upsert_topic = insert_topic.on_conflict_do_update(
            index_elements=["slug"],
            set_={"position": insert_topic.excluded.position},
        ).returning(Topic.id)
        topic_id = (await db.execute(upsert_topic)).scalar_one()
        topic_id_by_slug[topic.slug] = topic_id

        # Replace translations: delete old ones, insert new ones
        await db.execute(
            delete(TopicTranslation).where(TopicTranslation.topic_id == topic_id)
        )
        for locale in SUPPORTED:
            if locale in topic.translations:
                topic_tr = topic.translations[locale]
                db.add(
                    TopicTranslation(
                        topic_id=topic_id,
                        locale=locale,
                        name=topic_tr.name,
                        description=topic_tr.description,
                    )
                )

    # --- Topics, pass 2: resolve parent relationships ---
    for path, topic in all_topics:
        topic_id = topic_id_by_slug[topic.slug]

        if topic.parent is None:
            parent_id = None
        elif topic.parent == topic.slug:
            raise ContentError(
                f"{path.name}: topic '{topic.slug}' cannot be its own parent"
            )
        else:
            # Parent slug given: resolve to ID
            if topic.parent in topic_id_by_slug:
                parent_id = topic_id_by_slug[topic.parent]
            else:
                # Check database for an existing parent topic
                parent_id = await db.scalar(
                    select(Topic.id).where(Topic.slug == topic.parent)
                )
                if parent_id is None:
                    raise ContentError(
                        f"{path.name}: topic '{topic.slug}' references unknown parent "
                        f"'{topic.parent}'"
                    )

        await db.execute(
            update(Topic).where(Topic.id == topic_id).values(parent_id=parent_id)
        )

    # --- Tags: upsert all unique tag slugs ---
    all_tag_slugs: set[str] = set()
    for path, question in all_questions:
        all_tag_slugs.update(question.tags)

    if all_tag_slugs:
        await db.execute(
            pg_insert(Tag).values([{"slug": s} for s in all_tag_slugs]).on_conflict_do_nothing(
                index_elements=["slug"]
            )
        )

    # Fetch tag IDs by slug
    tag_id_by_slug: dict[str, int] = {}
    if all_tag_slugs:
        rows = await db.execute(
            select(Tag.slug, Tag.id).where(Tag.slug.in_(all_tag_slugs))
        )
        # The lookup below goes from slug to id, so key on the slug explicitly:
        # `dict(rows)` keys on whichever column comes first and silently inverts
        tag_id_by_slug = {slug: tag_id for slug, tag_id in rows}

    # --- Questions ---
    for path, question in all_questions:
        # Resolve topic
        if question.topic in topic_id_by_slug:
            topic_id = topic_id_by_slug[question.topic]
        else:
            existing_topic_id = await db.scalar(
                select(Topic.id).where(Topic.slug == question.topic)
            )
            if existing_topic_id is None:
                raise ContentError(
                    f"{path.name}: question '{question.slug}' references unknown topic "
                    f"'{question.topic}'"
                )
            topic_id = existing_topic_id

        # Upsert question
        insert_question = pg_insert(Question).values(
            slug=question.slug,
            topic_id=topic_id,
            kind=QuestionKind(question.kind),
            difficulty_rating=question.difficulty,
            status=QuestionStatus(question.status),
            key_points=question.key_points,
        )
        upsert_question = insert_question.on_conflict_do_update(
            index_elements=["slug"],
            set_={
                "topic_id": insert_question.excluded.topic_id,
                "kind": insert_question.excluded.kind,
                # The file owns the status, so a reload applies it: setting
                # `archived` in the file retires the question. Deleting an entry
                # from its file leaves its row as last loaded — the loader never
                # removes rows — so retire a question by archiving it
                "status": insert_question.excluded.status,
                "key_points": insert_question.excluded.key_points,
                # CRITICAL: `difficulty_rating` is NOT updated here.
                # It is fixed at insertion and evolves only via student results.
                # Re-loading a file with a different difficulty never overwrites
                # the rating already built up from usage.
                "updated_at": func.now(),
            },
        ).returning(Question.id)
        question_id = (await db.execute(upsert_question)).scalar_one()

        # Replace translations
        await db.execute(
            delete(QuestionTranslation).where(QuestionTranslation.question_id == question_id)
        )
        for locale in SUPPORTED:
            if locale in question.translations:
                question_tr = question.translations[locale]
                db.add(
                    QuestionTranslation(
                        question_id=question_id,
                        locale=locale,
                        title=question_tr.title,
                        stem=question_tr.stem,
                        explanation=question_tr.explanation,
                    )
                )

        # Replace tags
        await db.execute(
            delete(QuestionTag).where(QuestionTag.question_id == question_id)
        )
        for tag_slug in question.tags:
            tag_id = tag_id_by_slug[tag_slug]
            db.add(QuestionTag(question_id=question_id, tag_id=tag_id))

        await _replace_choices(db, question_id, question.choices)

        # Only published questions are validated: a draft may be incomplete,
        # and an archived one is no longer shown
        if question.status == "published":
            # Validate the question: does it satisfy publication rules?
            # `populate_existing=True` is CRITICAL here. The upserts above use raw SQL
            # INSERT/UPDATE statements which do not update ORM objects already in the
            # session. Without this flag, loading the question again would return the
            # stale cached version, not the one just written to the database.
            question_entity = await db.get(Question, question_id, populate_existing=True)
            if question_entity is None:  # the row was upserted a few lines above
                raise ContentError(f"{path.name}: question '{question.slug}' vanished after upsert")
            try:
                await assert_publishable(db, question_entity)
            except HTTPException as exc:
                raise ContentError(
                    f"{path.name}: question '{question.slug}': {exc.detail}"
                ) from exc

    return LoadSummary(files=len(files), topics=len(all_topics), questions=len(all_questions))


async def _replace_choices(
    db: AsyncSession, question_id: int, choices: list[ChoiceIn]
) -> None:
    """Makes the question's choices match the file, position by position

    Rows are updated in place rather than deleted and recreated: a finished
    match keeps pointing at the choice each player picked, so a reload must not
    change their ids. Positions the file no longer has are removed
    """
    existing = {
        choice.position: choice
        for choice in await db.scalars(select(Choice).where(Choice.question_id == question_id))
    }

    for position, choice_in in enumerate(choices):
        choice = existing.pop(position, None)
        if choice is None:
            choice = Choice(question_id=question_id, position=position)
            db.add(choice)
        choice.is_correct = choice_in.correct
        await db.flush()

        await db.execute(
            delete(ChoiceTranslation).where(ChoiceTranslation.choice_id == choice.id)
        )
        for locale in SUPPORTED:
            if locale in choice_in.text:
                db.add(
                    ChoiceTranslation(
                        choice_id=choice.id, locale=locale, text=choice_in.text[locale]
                    )
                )

    for choice in existing.values():
        await db.delete(choice)
    await db.flush()
