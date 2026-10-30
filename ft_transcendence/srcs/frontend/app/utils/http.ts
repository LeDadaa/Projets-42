/* Reading errors from `$fetch`.
 *
 * ofetch's `FetchError` class is not a resolvable import under pnpm's strict
 * node_modules layout (it lives nested under Nuxt, not hoisted), so importing
 * it breaks the page at runtime. We read what we need off the error by shape
 * instead of `instanceof`. ofetch attaches `status` / `statusCode` and the
 * parsed body as `data`.
 */

export function httpStatus(err: unknown): number | undefined {
  const e = err as {
    status?: number
    statusCode?: number
    response?: { status?: number }
  }
  return e?.status ?? e?.statusCode ?? e?.response?.status
}

export function httpDetail(err: unknown): string | undefined {
  const d = (err as { data?: { detail?: unknown } })?.data?.detail
  return typeof d === 'string' ? d : undefined
}

/* FastAPI/pydantic 422: `detail` is a list of { loc, msg, ... }. Return the
   first human message, stripped of pydantic's "Value error, " prefix. */
export function firstValidationMessage(err: unknown): string | undefined {
  const d = (err as { data?: { detail?: unknown } })?.data?.detail
  if (!Array.isArray(d) || d.length === 0) return undefined
  const msg = (d[0] as { msg?: unknown })?.msg
  return typeof msg === 'string' ? msg.replace(/^Value error,\s*/, '') : undefined
}
