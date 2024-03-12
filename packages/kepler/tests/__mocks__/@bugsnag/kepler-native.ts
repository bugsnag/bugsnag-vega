export * from '@bugsnag/kepler-native/src/type_guards'

export const BugsnagFileIO = {
  readTextFile: jest.fn().mockImplementation((filename: string) => ({content: JSON.stringify({filename})})),
  writeTextFile: jest.fn(),
  listDirectory: jest.fn(),
  mkdir: jest.fn(),
  deleteFile: jest.fn(),
}


export const BugsnagKeplerNative = {
  configure: jest.fn().mockReturnValue({app: {}}),
}
