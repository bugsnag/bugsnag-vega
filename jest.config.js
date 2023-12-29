/** @type {import('ts-jest').JestConfigWithTsJest} */

// these paths must be specified because otherwise typescript relies on the
// "main" field in each package.json file, which points to the compiled JS and
// we want to run Jest against the TS source
const paths = {
  '@bugsnag/kepler': ['./packages/kepler/lib/kepler.ts']
}

// convert the tsconfig "paths" option into Jest's "moduleNameMapper" option
// e.g.: "{ 'path': ['./a/b'] }" -> "{ '^path$': ['<rootDir>/a/b'] }"
const moduleNameMapper = Object.fromEntries(
  Object.entries(paths)
    .map(([name, directories]) => [
      `^${name}$`,
      directories.map(directory => directory.replace('./', '<rootDir>/'))
    ])
)

const defaultModuleConfig = {
  preset: 'ts-jest/presets/js-with-ts',
  moduleNameMapper,
  transform: {
    '^.+\\.m?[tj]sx?$': [
      'ts-jest',
      { tsconfig: { paths, allowJs: true } }
    ]
  }
}

module.exports = {
  projects: [
    {
      displayName: 'kepler',
      testMatch: ['<rootDir>/packages/kepler/**/*.test.ts'],
      ...defaultModuleConfig
    }
  ]
}
