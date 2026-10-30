"""Gamification rules, public: the same for every user"""

from __future__ import annotations

from fastapi import APIRouter

from app.models.user import XP_PER_LEVEL
from app.schemas.gamification import GamificationRules, XpRewards
from app.services.gamification import (
    XP_CORRECT_LLM,
    XP_CORRECT_SELF,
    XP_MATCH_PLAYED,
    XP_MATCH_WON,
)

router = APIRouter(prefix="/gamification", tags=["gamification"])


@router.get("/rules", response_model=GamificationRules)
async def rules() -> GamificationRules:
    return GamificationRules(
        xp_per_level=XP_PER_LEVEL,
        xp=XpRewards(
            correct_llm=XP_CORRECT_LLM,
            correct_self=XP_CORRECT_SELF,
            match_played=XP_MATCH_PLAYED,
            match_won=XP_MATCH_WON,
        ),
    )