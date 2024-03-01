import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

interface BugsnagConfiguration {
  apiKey: string
  appVersion?: string
}

interface DirEntry {
  name: string
  isFile: boolean
  isDirectory: boolean
}

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  configure: (configuration: BugsnagConfiguration) => void
  readTextFile: (path: string) => string | null
  writeTextFile: (path: string, content: string) => boolean
  listDirectory: (dir: string) => DirEntry[]
  deleteFile: (path: string) => boolean
  markLaunchCompleted: () => void
  nativeCrash:() => void
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative
