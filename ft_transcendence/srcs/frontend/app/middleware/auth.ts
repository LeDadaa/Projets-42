/* Route guard for the pages that need a real session.
   Applied per-page via `definePageMeta({ middleware: 'auth' })`.

   `fetchMe(true)` re-checks the server on every navigation (and refreshes the
   token if it just expired) instead of trusting a stale cached user. */
export default defineNuxtRouteMiddleware(async () => {
  const { fetchMe } = useAuth()
  const me = await fetchMe(true)
  if (!me) return navigateTo('/login')
})
