import type { QuestionPage, QuestionSummary, Topic } from '~/types'

const PAGE_SIZE = 100

const questions = ref<QuestionSummary[]>([])
const topics = ref<Topic[]>([])
const loadedLocale = ref<string | null>(null)
const failed = ref(false)
let pending: Promise<void> | null = null

export function useQuestions() {
  const { api } = useApi()
  const { locale } = useI18n()

  const loading = computed(() => !failed.value && loadedLocale.value !== locale.value)
  const topicsById = computed(() => new Map(topics.value.map((topic) => [topic.id, topic])))
  const exams = computed(() => topics.value.filter((topic) => topic.parent_id === null))

  function fetchPage(page: number, lang: string) {
    return api<QuestionPage>('/api/v1/questions', {
      query: { locale: lang, sort: 'difficulty', per_page: PAGE_SIZE, page },
    })
  }

  async function fetchAll(lang: string) {
    try {
      const [first, tree] = await Promise.all([
        fetchPage(1, lang),
        api<Topic[]>('/api/v1/topics', { query: { locale: lang } }),
      ])
      const others = await Promise.all(
        Array.from({ length: Math.max(0, first.total_pages - 1) }, (_, index) =>
          fetchPage(index + 2, lang),
        ),
      )
      // choice questions belong to the multiplayer quiz: the grader only
      // takes written answers, so exercises and training never show them
      questions.value = [first, ...others]
        .flatMap((page) => page.items)
        .filter((question) => question.kind === 'free_text')
      topics.value = tree
      loadedLocale.value = lang
    } catch {
      failed.value = true
    } finally {
      pending = null
    }
  }

  async function ensureLoaded() {
    failed.value = false
    while (!failed.value && loadedLocale.value !== locale.value) {
      pending ??= fetchAll(locale.value)
      await pending
    }
  }

  function load() {
    loadedLocale.value = null
    return ensureLoaded()
  }

  function topicBySlug(slug: string) {
    return topics.value.find((topic) => topic.slug === slug)
  }

  function childrenOf(topicId: number) {
    return topics.value.filter((topic) => topic.parent_id === topicId)
  }

  function questionsOf(topicId: number) {
    return questions.value.filter((question) => question.topic_id === topicId)
  }

  function lineage(topicId: number) {
    const chain: Topic[] = []
    let topic = topicsById.value.get(topicId)
    while (topic) {
      chain.unshift(topic)
      topic = topic.parent_id === null ? undefined : topicsById.value.get(topic.parent_id)
    }
    return chain
  }

  function topicLabel(topicId: number) {
    return lineage(topicId)
      .flatMap((topic) => (topic.name ? [topic.name] : []))
      .join(' · ')
  }

  function questionCount(topicId: number): number {
    return childrenOf(topicId).reduce(
      (sum, child) => sum + questionCount(child.id),
      questionsOf(topicId).length,
    )
  }

  onMounted(ensureLoaded)
  watch(locale, ensureLoaded)

  return {
    questions,
    topics,
    exams,
    loading,
    failed,
    load,
    topicBySlug,
    childrenOf,
    questionsOf,
    lineage,
    topicLabel,
    questionCount,
  }
}
