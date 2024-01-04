module.exports = {
  collectCoverageFrom: [
    '**/packages/*/**/*.js',
    '!**/*.test.js',
    '!**/*.test-*.js'
  ],
  coverageReporters: [
    'json-summary', 'json', 'lcov', 'text', 'clover'
  ],
  projects: [
    {
      displayName: 'kepler',
      roots: ['<rootDir>/packages/kepler'],
      testMatch: ['<rootDir>/packages/kepler/**/*.test.js']
    }
  ]
}
