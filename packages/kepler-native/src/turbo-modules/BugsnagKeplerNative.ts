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

interface BugsnagBreadcrumb {
  type: number
  message: string
  metadata: string
  timestamp: number
}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => BugsnagNativeStaticInfo
  markLaunchCompleted: () => void
  getDeviceID: () => string
  setDeviceID: (id: string) => void
  generateUUID: () => string
  leaveBreadcrumb: (crumb: BugsnagBreadcrumb) => void
  setMetadata(): (metadataStr: string) => void
  clearMetadata(): () => void
  setFeatures(): (featuresStr: string) => void
  clearFeatures(): () => void
  nativeCrash: () => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
