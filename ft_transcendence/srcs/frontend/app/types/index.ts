export type Difficulty = 'easy' | 'medium' | 'hard'

export type QuestionKind = 'single_choice' | 'multi_choice' | 'free_text'

export interface PlayableTopic {
  slug: string
  name: string
  description: string | null
  parent_slug: string | null
  parent_name: string | null
  question_count: number
}

export interface Topic {
  id: number
  parent_id: number | null
  slug: string
  position: number
  depth: number
  name: string | null
  description: string | null
}

export interface QuestionSummary {
  id: number
  slug: string
  title: string | null
  topic_id: number
  kind: QuestionKind
  status: string
  difficulty_rating: string
  stem: string | null
  is_fallback: boolean
  tags: string[]
  times_asked: number
  times_correct: number
  created_at: string
  updated_at: string
}

export interface QuestionPage {
  items: QuestionSummary[]
  page: number
  per_page: number
  total: number
  total_pages: number
}

/* --- user interaction module: profiles, friends, chat --- */

export interface UserSummary {
  id: number
  display_name: string
  avatar_path: string | null
}

export type FriendshipView = 'none' | 'pending_in' | 'pending_out' | 'friends' | 'blocked'

export interface Profile extends UserSummary {
  bio: string | null
  role: string
  created_at: string
  online: boolean
  friendship: FriendshipView
  friendship_id: number | null
}

export interface Friend {
  friendship_id: number
  user: UserSummary
  online: boolean
  last_seen_at: string | null
}

export interface PendingRequest {
  friendship_id: number
  user: UserSummary
  created_at: string
}

export interface DirectMessage {
  id: number
  sender_id: number
  recipient_id: number
  body: string
  created_at: string
  read_at: string | null
}

export interface Conversation {
  user: UserSummary
  last_message: DirectMessage
  unread: number
}

export type GraderOutcome = 'correct' | 'imprecise' | 'off_target'

export type FallbackReason = 'disabled' | 'budget' | 'unavailable'

export interface AttemptQuota {
  limit: number
  left: number
}

export type GraderEvent =
  | {
      type: 'verdict'
      attempt_id: number
      outcome: GraderOutcome
      score: number
      flagged: boolean
      attempts_left: number
      achievements: string[]
    }
  | { type: 'delta'; text: string }
  | { type: 'error'; code: string }
  | { type: 'done'; closed: boolean; explanation: string | null }
  | {
      type: 'self_assessment'
      attempt_id: number
      reason: FallbackReason
      explanation: string | null
      attempts_left: number
    }

export interface MatchSummaryChoice {
  id: number
  text: string
  correct: boolean
}

export interface MatchSummaryAttempt {
  user_id: number
  choice_id: number | null
  is_correct: boolean
  response_ms: number | null
  score: number
}

export interface MatchSummaryQuestion {
  position: number
  question_id: number
  slug: string
  stem: string
  is_fallback: boolean
  choices: MatchSummaryChoice[]
  attempts: MatchSummaryAttempt[]
}

export interface MatchSummaryPlayer {
  user_id: number
  display_name: string
  avatar_path: string | null
  score: number
  final_rank: number | null
}

export interface MatchSummary {
  public_id: string
  mode: string
  question_count: number
  started_at: string | null
  finished_at: string | null
  players: MatchSummaryPlayer[]
  questions: MatchSummaryQuestion[]
}
