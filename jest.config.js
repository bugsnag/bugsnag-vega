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
      roots: [
        '<rootDir>/packages/vega',
        '<rootDir>/packages/vega-plugin-react-navigation'
      ],
      testMatch: [
        '<rootDir>/packages/vega/**/*.test.ts',
        '<rootDir>/packages/vega-plugin-react-navigation/**/*.test.tsx'
      ],
      testEnvironment: 'jsdom',
      transform: {
        '^.+\\.[jt]sx?$': ['babel-jest', { configFile: './babel.config.js' }]
      },
      transformIgnorePatterns: [
        'node_modules/(?!(react-native|@react-native|@amazon-devices|@bugsnag/delivery-fetch)/)'
      ],
      moduleNameMapper: {
        '^react-native$': '<rootDir>/node_modules/react-native/index.js'
      }
    }
  ]
}
