module.exports = {
  parserOptions: { ecmaVersion: 2018 },
  overrides: [
    // linting for js files
    {
      files: ['*.js'],
      extends: ['standard']
    },
    // Linting for tests
    {
      files: ['*.test.js'],
      env: { jest: true, browser: true },
      plugins: ['eslint-plugin-jest'],
      extends: ['standard', 'plugin:jest/recommended'],
      rules: {
        // Disable preferring Promise-based async tests
        'jest/no-test-callback': 'off'
      }
    }
  ]
}
