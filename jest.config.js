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
      displayName: 'vega',
      roots: ['<rootDir>/packages/vega', '<rootDir>/packages/vega-plugin-react-navigation'],
      testMatch: ['<rootDir>/packages/vega/**/*.test.ts', '<rootDir>/packages/vega-plugin-react-navigation/**/*.test.tsx'],
      preset: 'react-native',
      transformIgnorePatterns: [
        'node_modules/(?!(jest-)?react-native|@bugsnag/delivery-fetch)/'
      ]
    }
  ]
}
