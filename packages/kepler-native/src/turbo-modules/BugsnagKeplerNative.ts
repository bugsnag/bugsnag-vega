import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  type: string
  appVersion?: string
  launchDurationMillis?: number
}

interface BugsnagNativeConfiguration {
  appVersion?: string
}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => BugsnagNativeConfiguration
  readTextFile: (path: string) => string | null
  writeTextFile: (path: string, content: string) => boolean
  nativeCrash:() => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
