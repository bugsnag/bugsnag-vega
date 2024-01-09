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
