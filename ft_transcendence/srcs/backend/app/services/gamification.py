"""Gamification — XP and daily streak

`record_activity` is the single entry point: exercises and games call it,
never touch `xp` / `current_streak` directly
Days are counted in UTC, the same for every user
"""

from __future__ import annotations

from collections.abc import Callable
from datetime import UTC, date, datetime, timedelta

from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.gamification import Achievement, UserAchievement
from app.models.user import User

# code -> "has the user earned it?"
# Codes must match the rows seeded by the "seed achievements" migration
ACHIEVEMENT_RULES: dict[str, Callable[[User], bool]] = {
    "first_steps": lambda user: user.xp > 0,
    "level_5": lambda user: user.level >= 5,
    "level_10": lambda user: user.level >= 10,
    "streak_3": lambda user: user.current_streak >= 3,
    "streak_7": lambda user: user.current_streak >= 7,
}

XP_CORRECT_LLM = 50
XP_CORRECT_SELF = 20
XP_MATCH_PLAYED = 20
XP_MATCH_WON = 50

def utc_today() -> date:
    """Today's date in UTC"""
    return datetime.now(UTC).date()


def update_streak(user: User, today: date) -> None:
    """Counts `today` in the user's daily streak

    Same day: nothing, already counted
    Day after the last activity: +1
    Anything else (first activity, or a gap): starts over at 1
    """
    if user.last_activity_date == today - timedelta(days=1):
        user.last_activity_date = today
        user.current_streak = user.current_streak + 1
    elif user.last_activity_date == today:
        pass
    else:
        user.last_activity_date = today
        user.current_streak = 1



def effective_streak(user: User, today: date) -> int:
    """The streak to display

    `current_streak` is only written when the user acts: after a gap it still
    holds the old value, so reading it raw would show a dead streak
    """
    if user.last_activity_date == today or user.last_activity_date == today - timedelta(days=1):
        return user.current_streak
    return 0

async def check_achievements(db: AsyncSession, user: User) -> list[str]:
    """Grants every achievement the user now qualifies for

    Returns the codes newly granted, [] if none
    No commit: the caller commits
    """
    # a. codes the user already has
    result = await db.execute(
        select(Achievement.code)
        .join(UserAchievement, UserAchievement.achievement_id == Achievement.id)
        .where(UserAchievement.user_id == user.id)
    )
    owned = set(result.scalars().all())

    # b. earned now and not owned yet
    new_codes = [
        code
        for code, rule in ACHIEVEMENT_RULES.items()
        if code not in owned and rule(user)
    ]
    if not new_codes:
        return []

    # c. their ids
    achievements = (
        await db.scalars(select(Achievement).where(Achievement.code.in_(new_codes)))
    ).all()

    # d. grant them
    for achievement in achievements:
        db.add(UserAchievement(user_id=user.id, achievement_id=achievement.id))
    return [achievement.code for achievement in achievements]

async def record_activity(
    db: AsyncSession, user: User, xp_gained: int, today: date | None = None
) -> list[str]:
    """Credits XP, counts today in the streak, grants new achievements

    Returns the codes of the achievements unlocked by this activity
    No commit: the caller commits, so the answer and its XP land in the same
    transaction — or neither does
    """
    # Sessions keep objects as first loaded (`expire_on_commit=False`), and the
    # websocket's lives as long as the socket: `user` may hold an XP from long
    # ago. Reload the row, locked until the caller's commit, so two activities
    # at once add up instead of the last one overwriting the other
    await db.refresh(user, with_for_update=True)
    if today is None:
        today = utc_today()
    user.xp += xp_gained
    update_streak(user, today)
    return await check_achievements(db, user)