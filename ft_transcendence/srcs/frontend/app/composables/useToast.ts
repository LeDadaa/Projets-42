/* Short-lived notices stacked in a corner, e.g. an achievement just unlocked.
 *
 * State is module-level like `usePresence`: any component can push, the one
 * `UiToaster` mounted by the dashboard layout renders them. Each toast leaves
 * on its own after TOAST_DURATION_MS, the close button removes it sooner.
 */

export interface Toast {
  id: number
  title: string
  body?: string
  emoji?: string
}

const TOAST_DURATION_MS = 6000

const toasts = ref<Toast[]>([])
let nextId = 0

function dismiss(id: number) {
  toasts.value = toasts.value.filter((toast) => toast.id !== id)
}

function push(toast: Omit<Toast, 'id'>) {
  const id = nextId++
  toasts.value = [...toasts.value, { ...toast, id }]
  setTimeout(() => dismiss(id), TOAST_DURATION_MS)
}

export function useToast() {
  return { toasts: readonly(toasts), push, dismiss }
}