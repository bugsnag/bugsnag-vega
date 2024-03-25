import { schema as coreSchema } from '@bugsnag/core/config'
import intRange from '@bugsnag/core/lib/validators/int-range'

const iserror = require('iserror')

export const schema = {
  ...coreSchema,
  logger: {
    ...coreSchema.logger,
    defaultValue: () => getPrefixedConsole()
  },
  persistenceDirectory: {
    defaultValue: () => '/data/bugsnag',
    message: 'should be a path string',
    validate: (value) => typeof value === 'string' && value !== null
  },
  maxPersistedEvents: {
    defaultValue: () => 32,
    message: 'should be a positive number',
    validate: (value) => intRange()(value)
  }
}

const getPrefixedConsole = () => {
  return ['debug', 'info', 'warn', 'error'].reduce((accum, method) => {
    const consoleMethod = console[method] || console.log
    if (method !== 'warn') {
      accum[method] = (...args) => consoleMethod('[bugsnag]', ...args)
    } else {
      accum[method] = (...args) => {
        if (!iserror(args[0])) {
          consoleMethod('[bugsnag]', ...args)
        } else {
          // a raw error doesn't display nicely in react native's yellow box,
          // so this takes the message from the error an displays that instead
          consoleMethod('[bugsnag]', `${args[0].message}`)
        }
      }
    }
    return accum
  }, {})
}

export default schema
