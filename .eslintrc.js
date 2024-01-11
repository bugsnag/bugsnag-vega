const ruleOverrides = {
  // Disable preferring Promise-based async tests
  'jest/no-test-callback': 'off',

  // Let TypeScript inference work without being verbose
  '@typescript-eslint/explicit-function-return-type': 'off',

  // (Explicit) any has its valid use cases
  '@typescript-eslint/no-explicit-any': 'off',

  // We use noop functions liberally (() => {})
  '@typescript-eslint/no-empty-function': 'off',

  // This incorrectly fails on TypeScript method override signatures
  'no-dupe-class-members': 'off',

  // Disable all rules that require parserServices (for now)
  '@typescript-eslint/no-floating-promises': 'off',
  '@typescript-eslint/no-misused-promises': 'off',
  '@typescript-eslint/no-unnecessary-type-assertion': 'off',
  '@typescript-eslint/prefer-nullish-coalescing': 'off',
  '@typescript-eslint/prefer-readonly': 'off',
  '@typescript-eslint/promise-function-async': 'off',
  '@typescript-eslint/require-array-sort-compare': 'off',
  '@typescript-eslint/require-await': 'off',
  '@typescript-eslint/restrict-plus-operands': 'off',
  '@typescript-eslint/restrict-template-expressions': 'off',
  '@typescript-eslint/strict-boolean-expressions': 'off',
  '@typescript-eslint/no-throw-literal': 'off',
  '@typescript-eslint/no-implied-eval': 'off',
  '@typescript-eslint/no-unnecessary-boolean-literal-compare': 'off',
  '@typescript-eslint/prefer-includes': 'off',
  '@typescript-eslint/no-for-in-array': 'off'
}

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
      files: ['**/*.test.ts'],
      env: { jest: true, browser: true },
      plugins: ['eslint-plugin-jest'],
      extends: ['standard-with-typescript', 'plugin:jest/recommended'],
      rules: {
        ...ruleOverrides
      }
    }
  ]
}
