//@ts-ignore
import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
//@ts-ignore
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  type: string
  appVersion?: string
  launchDurationMillis?: number
}

interface BugsnagNativeConfiguration {

}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => BugsnagNativeConfiguration
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
