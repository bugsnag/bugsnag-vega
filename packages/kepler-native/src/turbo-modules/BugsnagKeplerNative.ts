import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  appVersion?: string
}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => void
  markLaunchCompleted: () => void
  nativeCrash: () => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
