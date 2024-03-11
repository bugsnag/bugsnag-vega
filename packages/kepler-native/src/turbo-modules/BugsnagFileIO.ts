import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api'
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api'

export interface DirEntry {
  name: string
  isFile: boolean
  isDirectory: boolean
}

export interface ErrorResult {
  error: true
  message: string
  code: number
}

export type ReadFileResult = ErrorResult | { content: string }
export type WriteFileResult = ErrorResult | Record<string, never>

export interface BugsnagFileIO extends KeplerTurboModule {
  readTextFile: (path: string) => ReadFileResult
  writeTextFile: (path: string, content: string) => WriteFileResult
  listDirectory: (dir: string) => DirEntry[]
  mkdir: (dir: string) => boolean
  deleteFile: (path: string) => boolean
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagFileIO>(
  'BugsnagFileIO',
) as BugsnagFileIO
