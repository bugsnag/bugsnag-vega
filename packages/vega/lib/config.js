/* global __DEV__ */

import { schema as coreSchema } from '@bugsnag/core/config'
import intRange from '@bugsnag/core/lib/validators/int-range'

const iserror = require('iserror')

const IS_PRODUCTION = typeof __DEV__ === 'undefined' || __DEV__ !== true
const defaultErrorTypes = () => ({ unhandledExceptions: true, unhandledRejections: true, nativeCrashes: true })

export const schema = {
  ...coreSchema,
  logger: {
    ...coreSchema.logger,
    defaultValue: () => getPrefixedConsole()
  },
  releaseStage: {
    ...coreSchema.releaseStage,
    defaultValue: () => {
      return IS_PRODUCTION ? 'production' : 'development'
    }
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
  },
  maxPersistedSessions: {
    defaultValue: () => 128,
    message: 'should be a positive number',
    validate: (value) => intRange()(value)
  },
  persistUser: {
    defaultValue: () => true,
    message: 'should be a boolean',
    validate: (value) => typeof value === 'boolean'
  },
  enabledErrorTypes: {
    defaultValue: () => defaultErrorTypes(),
    message: 'should be an object containing the flags { unhandledExceptions:true|false, unhandledRejections:true|false, nativeCrashes: true|false }',
    allowPartialObject: true,
    validate: value => {
      // ensure we have an object
      if (typeof value !== 'object' || !value) return false
      const providedKeys = Object.keys(value)
      const defaultKeys = Object.keys(defaultErrorTypes())
      // ensure it only has a subset of the allowed keys
      if (providedKeys.filter(k => defaultKeys.includes(k)).length < providedKeys.length) return false
      // ensure all of the values are boolean
      if (Object.keys(value).filter(k => typeof value[k] !== 'boolean').length > 0) return false
      return true
    }
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
