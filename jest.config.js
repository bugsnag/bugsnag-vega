/** @type {import('ts-jest').JestConfigWithTsJest} */

module.exports = {
  collectCoverageFrom: [
    '**/packages/*/**/*.js',
    '!**/*.test.ts',
    '!**/*.test-*.ts'
  ],
  coverageReporters: [
    'json-summary', 'json', 'lcov', 'text', 'clover'
  ],
  projects: [
    {
      displayName: 'kepler',
      roots: ['<rootDir>/packages/kepler'],
      testMatch: ['<rootDir>/packages/kepler/**/*.test.ts'],
      preset: 'ts-jest'
    }
  ]
}
