export function useNotions() {
  const { t, te } = useI18n()

  function notionLabel(slug: string) {
    const key = `notions.${slug}`
    return te(key) ? t(key) : slug
  }

  return { notionLabel }
}
