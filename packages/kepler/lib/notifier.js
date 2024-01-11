import { version } from '../package.json'
import { Client } from '@bugsnag/core'
import { schema } from './config'
import delivery from '@bugsnag/delivery-fetch'

const name = 'Bugsnag Kepler'
const url = 'https://github.com/bugsnag/bugsnag-kepler'

export const Bugsnag = {
  _client: null,
  createClient: (opts) => {
    // handle very simple use case where user supplies just the api key as a string
    if (typeof opts === 'string') opts = { apiKey: opts }
    if (!opts) opts = {}

    // TODO: Add plugin-react
    const internalPlugins = []

    // configure a client with user supplied options
    const bugsnag = new Client(opts, schema, internalPlugins, { name, version, url })

    bugsnag._setDelivery(delivery)

    bugsnag._logger.debug('Loaded!')
    bugsnag.leaveBreadcrumb('Bugsnag loaded', {}, 'state')

    return bugsnag
  },
  start: (opts) => {
    if (Bugsnag._client) {
      Bugsnag._client._logger.warn('Bugsnag.start() was called more than once. Ignoring.')
      return Bugsnag._client
    }
    Bugsnag._client = Bugsnag.createClient(opts)
    return Bugsnag._client
  },
  isStarted: () => {
    return Bugsnag._client != null
  }
}

Object.getOwnPropertyNames(Client.prototype).forEach((m) => {
  if (/^_/.test(m)) return
  Bugsnag[m] = function () {
    if (!Bugsnag._client) return console.error(`Bugsnag.${m}() was called before Bugsnag.start()`)
    Bugsnag._client._depth += 1
    try {
      return Bugsnag._client[m].apply(Bugsnag._client, arguments)
    } finally {
      Bugsnag._client._depth -= 1
    }
  }
})

export default Bugsnag
