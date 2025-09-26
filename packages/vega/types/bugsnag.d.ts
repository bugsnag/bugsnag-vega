import { type Client, type Config } from '@bugsnag/core'

// eslint-disable-next-line @typescript-eslint/no-empty-interface
interface VegaConfig extends Config {
  persistenceDirectory?: string
  maxPersistedEvents?: number
  maxPersistedSessions?: number
  persistUser?: boolean
  enabledErrorTypes?: {
    unhandledExceptions?: boolean
    unhandledRejections?: boolean
    nativeCrashes?: boolean
  }
}

declare class VegaClient extends Client {
  markLaunchComplete: () => void
  readonly lastRunInfo: LastRunInfo | null
}

interface VegaBugsnagStatic extends Client {
  start: (apiKeyOrOpts: string | VegaConfig) => Client
  isStarted: () => boolean
}

declare const Bugsnag: VegaBugsnagStatic

export default Bugsnag
export * from '@bugsnag/core'
export type { VegaConfig }
