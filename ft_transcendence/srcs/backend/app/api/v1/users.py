"""Profile and avatar"""

from __future__ import annotations

from fastapi import APIRouter, File, HTTPException, Query, Response, UploadFile, status
from sqlalchemy import func, select
from sqlalchemy.exc import IntegrityError

from app.deps import CurrentUser, DbSession
from app.models.user import User
from app.schemas.user import ProfileOut, UserPublic, UserSummary, UserUpdate
from app.schemas.user import UserPublic, UserUpdate
from app.services import media
from app.services.realtime import hub
from app.services.social import friendship_state

from app.models.gamification import Achievement, UserAchievement
from app.schemas.gamification import AchievementOut, UserStats
from app.models.attempt import Attempt

router = APIRouter(prefix="/users", tags=["users"])


@router.patch("/me", response_model=UserPublic)
async def update_me(payload: UserUpdate, user: CurrentUser, db: DbSession):
    """Edits your own display name bio or language

    Only the fields listed in `UserUpdate` can be written
    so a client cannot promote itself by sending `role`
    """
    # exclude_unset : partial PATCH would erase bio with None
    for field, value in payload.model_dump(exclude_unset=True).items():
        setattr(user, field, value)
    try:
        await db.commit()
    except IntegrityError as exc:
        await db.rollback()
        raise HTTPException(
            status.HTTP_409_CONFLICT, detail="name already taken"
        ) from exc
    await db.refresh(user)
    return user

@router.post("/me/avatar", response_model=UserPublic)
async def upload_avatar(
    user: CurrentUser, db: DbSession, file: UploadFile = File(...)
):
    """Replaces your avatar with an uploaded image

    Caps the size then re encodes to WebP
    Re encoding is what strips metadata and any payload hidden in the file
    """
    raw = await media.read_limited(file)          # size
    processed = await media.process_avatar(raw)   # format then re encode
    user.avatar_path = await media.save_avatar(processed)
    await db.commit()
    await db.refresh(user)
    return user

@router.get("/me/achievements", response_model=list[AchievementOut])
async def my_achievements(user: CurrentUser, db: DbSession):
    """Every achievement of the catalog, with your unlock date or None"""
    result = await db.execute(
        select(Achievement.code, Achievement.icon, UserAchievement.earned_at)
        .outerjoin(
            UserAchievement,
            (UserAchievement.achievement_id == Achievement.id)
            & (UserAchievement.user_id == user.id),
        )
        .order_by(Achievement.id)
    )
    return result.all()

@router.get("", response_model=list[UserSummary])
async def search_users(
    user: CurrentUser,
    db: DbSession,
    q: str = Query(min_length=1, max_length=32),
    limit: int = Query(default=10, ge=1, le=25),
):
    """Finds users by a display-name fragment, to add a friend or start a chat

    Case-insensitive substring match. Yourself is excluded. Authentication is
    required so this cannot be scraped anonymously.
    """
    rows = await db.scalars(
        select(User)
        .where(
            User.id != user.id,
            User.is_active.is_(True),
            func.lower(User.display_name).contains(q.lower()),
        )
        .order_by(User.display_name)
        .limit(limit)
    )
    return list(rows)


@router.get("/{user_id}", response_model=ProfileOut)
async def get_profile(user_id: int, viewer: CurrentUser, db: DbSession):
    """Public profile of one user, plus where the viewer stands with them

    `friendship` drives the profile's action button (add / accept / pending /
    unblock); `online` drives the presence dot.
    """
    target = await db.get(User, user_id)
    if target is None or not target.is_active:
        raise HTTPException(status.HTTP_404_NOT_FOUND, detail="user not found")
    view, friendship_id = await friendship_state(db, viewer.id, target.id)
    return ProfileOut(
        id=target.id,
        display_name=target.display_name,
        avatar_path=target.avatar_path,
        bio=target.bio,
        role=target.role,
        created_at=target.created_at,
        online=hub.is_online(target.id),
        friendship=view,
        friendship_id=friendship_id,
    )


@router.get("/{user_id}/avatar")
async def get_avatar(user_id: int, db: DbSession):
    """Serves a generated fallback avatar

    If no avatar
    Real avatars provided by nginx (`location /media/`) :
    this route handles by default
    """
    return Response(
        media.identicon(user_id),
        media_type="image/webp",
        headers={"Cache-Control": "public, max-age=86400"},
    )


@router.get("/me/stats", response_model=UserStats)
async def my_stats(user: CurrentUser, db: DbSession) -> UserStats:
    """Questions solved, and questions tried but never solved

    Counted per question: three misses then a success make one solved exercise
    """
    per_question = (
        select(func.bool_or(Attempt.is_correct).label("solved"))
        .where(Attempt.user_id == user.id)
        .group_by(Attempt.question_id)
        .subquery()
    )
    solved, to_review = (
        await db.execute(
            select(
                func.count().filter(per_question.c.solved.is_(True)),
                # IS NOT TRUE, not == False: bool_or skips NULLs, so a question
                # whose only attempts await self-assessment comes out NULL
                func.count().filter(per_question.c.solved.is_not(True)),
            )
        )
    ).one()
    return UserStats(solved=solved, to_review=to_review)