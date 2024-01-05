import { Client, Config } from '@bugsnag/core'

interface KeplerConfig extends Config {}

interface KeplerBugsnagStatic extends Client {
  start(apiKeyOrOpts: string | KeplerConfig): Client
  isStarted(): boolean
}

declare const Bugsnag: KeplerBugsnagStatic

export default Bugsnag
export * from '@bugsnag/core'
export { KeplerConfig }
