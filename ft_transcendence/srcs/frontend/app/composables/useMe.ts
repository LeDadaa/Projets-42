/* Thin alias over `useAuth` for the chat code, which only needs the current
   user's id (sent vs received) and a way to load it. */

export function useMe() {
  const { user, fetchMe } = useAuth()
  return { me: user, load: () => fetchMe() }
}
