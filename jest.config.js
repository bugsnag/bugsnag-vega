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
      roots: ['<rootDir>/packages/kepler', '<rootDir>/packages/kepler-plugin-react-navigation'],
      testMatch: ['<rootDir>/packages/kepler/**/*.test.ts', '<rootDir>/packages/kepler-plugin-react-navigation/**/*.test.tsx'],
      preset: 'react-native',
      transformIgnorePatterns: [
        'node_modules/(?!(jest-)?react-native|@bugsnag/delivery-fetch)/'
      ]
    }
  ]
}
