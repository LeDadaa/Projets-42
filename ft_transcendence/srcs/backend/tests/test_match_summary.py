"""Reading a finished duel back

The summary reveals the right answers, so who may read it and when is the whole
point of these tests: a player of a finished match, and nobody else
"""

from __future__ import annotations

import uuid
from datetime import UTC, datetime

from httpx import AsyncClient
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.content import Choice, ChoiceTranslation, Question, QuestionTranslation, Topic
from app.models.enums import MatchMode, MatchStatus, QuestionKind, QuestionStatus
from app.models.match import Match, MatchAttempt, MatchPlayer, MatchQuestion
from app.models.user import User


async def _question(db: AsyncSession, suffix: str) -> tuple[int, list[int]]:
    """A published written question with two choices, the first one right"""
    topic = Topic(slug=f"t-{suffix}", position=0)
    db.add(topic)
    await db.flush()

    question = Question(
        slug=f"q-{suffix}",
        topic_id=topic.id,
        kind=QuestionKind.free_text,
        status=QuestionStatus.published,
        key_points=["KP something"],
    )
    db.add(question)
    await db.flush()
    db.add(
        QuestionTranslation(
            question_id=question.id, locale="en", title="T", stem=f"stem {suffix}"
        )
    )

    choice_ids = []
    for position, correct in ((0, True), (1, False)):
        choice = Choice(question_id=question.id, position=position, is_correct=correct)
        db.add(choice)
        await db.flush()
        db.add(
            ChoiceTranslation(
                choice_id=choice.id, locale="en", text=f"choice {position} {suffix}"
            )
        )
        choice_ids.append(choice.id)
    await db.flush()
    return question.id, choice_ids


async def _finished_match(
    db: AsyncSession, winner: User, loser: User, *, status: MatchStatus = MatchStatus.finished
) -> str:
    public_id = uuid.uuid4().hex[:12]
    match = Match(
        public_id=public_id,
        mode=MatchMode.duel,
        status=status,
        max_players=2,
        question_count=1,
        started_at=datetime.now(UTC),
        finished_at=datetime.now(UTC) if status == MatchStatus.finished else None,
    )
    db.add(match)
    await db.flush()

    db.add(MatchPlayer(match_id=match.id, user_id=winner.id, score=120, final_rank=1))
    db.add(MatchPlayer(match_id=match.id, user_id=loser.id, score=40, final_rank=2))

    question_id, choice_ids = await _question(db, public_id)
    round_one = MatchQuestion(match_id=match.id, question_id=question_id, position=1)
    db.add(round_one)
    await db.flush()

    db.add(
        MatchAttempt(
            match_id=match.id,
            match_question_id=round_one.id,
            user_id=winner.id,
            choice_id=choice_ids[0],
            is_correct=True,
            response_ms=1200,
            score=120,
        )
    )
    db.add(
        MatchAttempt(
            match_id=match.id,
            match_question_id=round_one.id,
            user_id=loser.id,
            choice_id=choice_ids[1],
            is_correct=False,
            response_ms=3400,
            score=40,
        )
    )
    await db.flush()
    return public_id


class TestMatchSummary:
    async def test_a_player_gets_the_match_round_by_round(
        self, client: AsyncClient, as_user, db: AsyncSession
    ) -> None:
        # The client keeps the session of the last login, so the winner logs in last
        loser = await as_user(display_name="loser", email="loser@example.com")
        winner = await as_user()
        public_id = await _finished_match(db, winner, loser)

        r = await client.get(f"/api/v1/matches/{public_id}")

        assert r.status_code == 200, r.text
        body = r.json()
        assert body["public_id"] == public_id
        assert [p["final_rank"] for p in body["players"]] == [1, 2]
        assert [p["display_name"] for p in body["players"]] == [winner.display_name, "loser"]
        round_one = body["questions"][0]
        assert round_one["position"] == 1
        assert round_one["stem"].startswith("stem ")
        assert [c["correct"] for c in round_one["choices"]] == [True, False]
        assert {a["user_id"] for a in round_one["attempts"]} == {winner.id, loser.id}
        fastest = min(round_one["attempts"], key=lambda a: a["response_ms"])
        assert fastest["is_correct"] is True
        assert fastest["score"] == 120

    async def test_someone_who_did_not_play_gets_404(
        self, client: AsyncClient, as_user, db: AsyncSession
    ) -> None:
        loser = await as_user(display_name="loser2", email="loser2@example.com")
        winner = await as_user(display_name="winner2", email="winner2@example.com")
        public_id = await _finished_match(db, winner, loser)

        await as_user(display_name="stranger", email="stranger@example.com")
        r = await client.get(f"/api/v1/matches/{public_id}")

        assert r.status_code == 404

    async def test_a_match_still_running_is_not_readable(
        self, client: AsyncClient, as_user, db: AsyncSession
    ) -> None:
        """Served mid-game the summary would hand a player the right answers"""
        loser = await as_user(display_name="loser3", email="loser3@example.com")
        winner = await as_user(display_name="winner3", email="winner3@example.com")
        public_id = await _finished_match(db, winner, loser, status=MatchStatus.running)

        r = await client.get(f"/api/v1/matches/{public_id}")

        assert r.status_code == 404

    async def test_anonymous_is_refused(self, client: AsyncClient) -> None:
        r = await client.get("/api/v1/matches/whatever")

        assert r.status_code == 401

    async def test_an_unknown_match_is_404(self, client: AsyncClient, as_user) -> None:
        await as_user()

        r = await client.get("/api/v1/matches/does-not-exist")

        assert r.status_code == 404
