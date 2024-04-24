import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  appVersion?: string
}

interface NativeStaticApp {
  binaryArch: string
}

interface BugsnagNativeStaticInfo {
  app: NativeStaticApp
}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => BugsnagNativeStaticInfo
  markLaunchCompleted: () => void
  getDeviceID: () => string
  setDeviceID: (id: string) => void
  generateUUID: () => string
  leaveBreadcrumb: (type: number, message: string, metadata: string, timestamp: number) => void
  nativeCrash: () => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
