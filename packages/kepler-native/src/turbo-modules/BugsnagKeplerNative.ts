import type { KeplerTurboModule } from '@amzn/keplerscript-turbomodule-api'
import { TurboModuleRegistry } from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  type: string
  appVersion?: string
  launchDurationMillis?: number
}

// eslint-disable-next-line @typescript-eslint/no-empty-interface
interface BugsnagNativeConfiguration {

}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => BugsnagNativeConfiguration
}

// eslint-disable-next-line @typescript-eslint/no-unnecessary-type-assertion
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative'
) as BugsnagKeplerNative
