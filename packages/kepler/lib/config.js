const { schema } = require('@bugsnag/core/config')
const iserror = require('iserror')

module.exports.schema = {
  ...schema,
  logger: {
    ...schema.logger,
    defaultValue: () => getPrefixedConsole()
  }
}

const getPrefixedConsole = () => {
  return ['debug', 'info', 'warn', 'error'].reduce((accum, method) => {
    // keep references to initial console methods, since the
    // console breadcrumb plugin wraps them later
    const { debug, info, warn, error } = console
    const originalConsole = { debug, info, warn, error }
    if (method !== 'warn') {
      accum[method] = (...args) => originalConsole[method]('[bugsnag]', ...args)
    } else {
      accum[method] = (...args) => {
        if (!iserror(args[0])) {
          originalConsole[method]('[bugsnag]', ...args)
        } else {
          // a raw error doesn't display nicely in react native's yellow box,
          // so this takes the message from the error and displays that instead
          originalConsole[method]('[bugsnag]', `${args[0].message}`)
        }
      }
    }
    return accum
  }, {})
}
