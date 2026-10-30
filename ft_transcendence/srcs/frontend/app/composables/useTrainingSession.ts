export interface TrainingSession {
  topicId: number
  questionIds: number[]
  index: number
  startedAt: number
  endsAt: number | null
  finishedAt: number | null
  answers: Record<number, string>
  results: Record<number, boolean>
}

const STORAGE_KEY = 'maia_training_session'

function isSession(value: unknown): value is TrainingSession {
  const session = value as Partial<TrainingSession> | null
  return (
    typeof session === 'object' &&
    session !== null &&
    Array.isArray(session.questionIds) &&
    typeof session.startedAt === 'number' &&
    (typeof session.endsAt === 'number' || session.endsAt === null) &&
    typeof session.index === 'number' &&
    typeof session.answers === 'object' &&
    session.answers !== null &&
    typeof session.results === 'object' &&
    session.results !== null
  )
}

function readStored(): TrainingSession | null {
  try {
    const parsed: unknown = JSON.parse(sessionStorage.getItem(STORAGE_KEY) ?? 'null')
    return isSession(parsed) ? parsed : null
  } catch {
    return null
  }
}

function writeStored(session: TrainingSession | null) {
  try {
    if (session) sessionStorage.setItem(STORAGE_KEY, JSON.stringify(session))
    else sessionStorage.removeItem(STORAGE_KEY)
  } catch {
    return
  }
}

function shuffle<T>(items: T[]): T[] {
  const arr = [...items]
  for (let i = arr.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1))
    ;[arr[i], arr[j]] = [arr[j]!, arr[i]!]
  }
  return arr
}

export function formatClock(totalSeconds: number): string {
  const hours = Math.floor(totalSeconds / 3600)
  const minutes = Math.floor((totalSeconds % 3600) / 60)
  const seconds = totalSeconds % 60

  if (hours > 0) {
    return `${hours}:${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`
  }
  return `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`
}

export function useTrainingSession() {
  const session = ref<TrainingSession | null>(readStored())
  const now = ref(Date.now())

  function tick() {
    now.value = Date.now()
    if (
      session.value &&
      session.value.finishedAt === null &&
      session.value.endsAt !== null &&
      now.value >= session.value.endsAt
    ) {
      session.value.finishedAt = session.value.endsAt
    }
  }

  const remaining = computed(() =>
    session.value && session.value.endsAt !== null
      ? Math.max(0, session.value.endsAt - now.value)
      : 0,
  )
  const elapsed = computed(() =>
    session.value ? Math.max(0, now.value - session.value.startedAt) : 0,
  )
  const ended = computed(() => session.value !== null && session.value.finishedAt !== null)
  const unlimited = computed(() => session.value !== null && session.value.endsAt === null)
  const timedOut = computed(
    () =>
      session.value?.finishedAt != null &&
      session.value.endsAt !== null &&
      session.value.finishedAt >= session.value.endsAt,
  )

  function start(topicId: number, pool: number[], count: number, minutes: number | null) {
    const questionIds = shuffle(pool).slice(0, count)
    const startedAt = Date.now()
    session.value = {
      topicId,
      questionIds,
      index: 0,
      startedAt,
      endsAt: minutes === null ? null : startedAt + minutes * 60_000,
      finishedAt: null,
      answers: {},
      results: {},
    }
  }

  function finish() {
    if (session.value && session.value.finishedAt === null) {
      session.value.finishedAt = Date.now()
    }
  }

  function reset() {
    session.value = null
  }

  function record(questionId: number, accepted: boolean) {
    if (!session.value || session.value.finishedAt !== null) return
    session.value.results[questionId] = (session.value.results[questionId] ?? false) || accepted
  }

  tick()
  const interval = setInterval(tick, 1000)
  onUnmounted(() => clearInterval(interval))
  watch(session, writeStored, { deep: true })

  return { session, remaining, elapsed, ended, unlimited, timedOut, start, finish, reset, record }
}
