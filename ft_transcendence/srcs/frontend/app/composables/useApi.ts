/* Same-origin fetch with the CSRF header and silent session refresh.
 *
 * GETs still work with plain `$fetch` (the httpOnly `access_token` cookie rides
 * along on its own), but anything that must survive a 15-minute access token
 * should go through `api()`:
 *
 *  - writes get the double-submit `X-CSRF-Token` header, read fresh from
 *    `document.cookie` on every call (`_issue_session` rotates it);
 *  - on a 401 (outside `/auth/*`) it replays `POST /auth/refresh` once,
 *    single-flighted across all callers, then retries the original request.
 */

type ApiOptions = {
  method?: 'GET' | 'POST' | 'PATCH' | 'DELETE'
  body?: Record<string, unknown> | FormData
  query?: Record<string, unknown>
  headers?: Record<string, string>
}

export function csrfToken(): string {
  return (
    document.cookie
      .split('; ')
      .find((c) => c.startsWith('csrf_token='))
      ?.slice('csrf_token='.length) ?? ''
  )
}

// one refresh at a time, shared by every concurrent 401
let refreshing: Promise<boolean> | null = null

export function attemptRefresh(): Promise<boolean> {
  if (!refreshing) {
    refreshing = $fetch('/api/v1/auth/refresh', {
      method: 'POST',
      headers: { 'X-CSRF-Token': csrfToken() },
    })
      .then(() => true)
      .catch(() => false)
      .finally(() => {
        refreshing = null
      })
  }
  return refreshing
}

export function useApi() {
  async function api<T>(url: string, opts: ApiOptions = {}): Promise<T> {
    const run = () => {
      const method = opts.method ?? 'GET'
      const headers: Record<string, string> = { ...opts.headers }
      if (method !== 'GET') headers['X-CSRF-Token'] = csrfToken()
      return $fetch<T>(url, { ...opts, headers })
    }
    try {
      return await run()
    } catch (err) {
      if (httpStatus(err) === 401 && !url.includes('/api/v1/auth/')) {
        if (await attemptRefresh()) return await run()
      }
      throw err
    }
  }

  return { api }
}
