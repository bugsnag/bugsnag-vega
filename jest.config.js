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
      preset: 'react-native',
      transformIgnorePatterns: [
        'node_modules/(?!(jest-)?react-native|@bugsnag/delivery-fetch)/'
      ]
    }
  ]
}
