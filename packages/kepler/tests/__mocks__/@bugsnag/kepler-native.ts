export * from '@bugsnag/kepler-native/src/type_guards'

export const BugsnagFileIO = {
  readTextFile: jest.fn().mockImplementation((filename: string) => ({content: JSON.stringify({filename})})),
  writeTextFile: jest.fn(),
  listDirectory: jest.fn(),
  mkdir: jest.fn(),
  deleteFile: jest.fn(),
  sha1: jest.fn().mockImplementation((payload: string) => payload),
}

export const BugsnagKeplerNative = {
  configure: jest.fn().mockReturnValue({ app: {} }),
  getDeviceID: jest.fn(),
  setDeviceID: jest.fn(),
  generateUUID: jest.fn(),
  leaveBreadcrumb: jest.fn(),
  setMetadata: jest.fn(),
  clearMetadata: jest.fn(),
  setFeatures: jest.fn(),
  clearFeatures: jest.fn(),
  setUser: jest.fn(),
  clearUser: jest.fn(),
  setApp: jest.fn(),
  setDevice: jest.fn(),
  markLaunchCompleted: jest.fn(),
}
