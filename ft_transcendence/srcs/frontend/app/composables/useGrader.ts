import type { GraderEvent } from '~/types'
import { attemptRefresh, csrfToken } from './useApi'

export class GraderError extends Error {
  constructor(
    readonly status: number,
    readonly detail: string | null,
    readonly retryAfter: number | null,
  ) {
    super(`grading failed with ${status}`)
  }
}

async function failure(response: Response): Promise<GraderError> {
  const body = (await response.json().catch(() => null)) as { detail?: unknown } | null
  const detail = typeof body?.detail === 'string' ? body.detail : null
  const retryAfter = Number.parseInt(response.headers.get('Retry-After') ?? '', 10)
  return new GraderError(response.status, detail, Number.isNaN(retryAfter) ? null : retryAfter)
}

async function readEvents(
  body: ReadableStream<Uint8Array>,
  onEvent: (event: GraderEvent) => void,
): Promise<void> {
  const reader = body.getReader()
  const decoder = new TextDecoder()
  let buffer = ''
  for (;;) {
    const { done, value } = await reader.read()
    buffer += done ? decoder.decode() : decoder.decode(value, { stream: true })
    const lines = buffer.split('\n')
    buffer = done ? '' : (lines.pop() ?? '')
    for (const line of lines) {
      if (line.trim()) onEvent(JSON.parse(line) as GraderEvent)
    }
    if (done) return
  }
}

export function useGrader() {
  const { api } = useApi()
  const { locale } = useI18n()

  async function submit(
    questionId: number,
    answer: string,
    onEvent: (event: GraderEvent) => void,
  ): Promise<void> {
    const post = () =>
      fetch(`/api/v1/questions/${questionId}/attempts?locale=${locale.value}`, {
        method: 'POST',
        credentials: 'same-origin',
        headers: {
          'Content-Type': 'application/json',
          Accept: 'application/x-ndjson',
          'X-CSRF-Token': csrfToken(),
        },
        body: JSON.stringify({ answer: answer.trim() }),
      })

    let response = await post()
    if (response.status === 401 && (await attemptRefresh())) response = await post()
    if (!response.ok || !response.body) throw await failure(response)
    await readEvents(response.body, onEvent)
  }

  async function selfAssess(attemptId: number, correct: boolean): Promise<string[]> {
    const { achievements } = await api<{ achievements: string[] }>
    (`/api/v1/attempts/${attemptId}/self-assessment`, {
      method: 'POST',
      body: { correct },
    })
    return achievements
  }

  return { submit, selfAssess }
}
