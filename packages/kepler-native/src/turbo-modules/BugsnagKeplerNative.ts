import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  appVersion?: string
  persistenceDirectory: string
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

interface BugsnagUserInfo {
  id: string
  name: string
  email: string
}

interface BugsnagAppInfo {
  bundleId: string
  releaseStage: string
  type: string
  version: string
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
  setUser(): (userInfo: BugsnagUserInfo) => void
  clearUser(): () => void
  setApp(): (appInfo: BugsnagAppInfo) => void
  nativeCrash: () => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
