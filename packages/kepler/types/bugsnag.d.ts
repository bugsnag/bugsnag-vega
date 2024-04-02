import { type Client, type Config } from '@bugsnag/core'

// eslint-disable-next-line @typescript-eslint/no-empty-interface
interface KeplerConfig extends Config {
  persistenceDirectory?: string
  maxPersistedEvents?: number
  maxPersistedSessions?: number
  persistUser?: boolean
}

declare class KeplerClient extends Client {
  markLaunchComplete: () => void
  readonly lastRunInfo: LastRunInfo | null
}

interface KeplerBugsnagStatic extends Client {
  start: (apiKeyOrOpts: string | KeplerConfig) => Client
  isStarted: () => boolean
}

declare const Bugsnag: KeplerBugsnagStatic

export default Bugsnag
export * from '@bugsnag/core'
export type { KeplerConfig }
