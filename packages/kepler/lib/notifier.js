import { version } from '../package.json'
import { Client, Event } from '@bugsnag/core'
import { schema } from './config'
import delivery from './delivery'
import createUserStore from './user_store'
import createDeviceStore from './device_store'
import BugsnagPluginReact from '@bugsnag/plugin-react'
import createBugsnagGlobalErrorHandlerPlugin from '@bugsnag/plugin-react-native-global-error-handler'
import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import unhandledRejectionPlugin from '@bugsnag/plugin-react-native-unhandled-rejection'
import pluginConsoleBreadcrumbs from '@bugsnag/plugin-console-breadcrumbs'
import pluginAppDuration from '@bugsnag/plugin-app-duration'
import createPluginNetworkBreadcrumbs from '@bugsnag/plugin-network-breadcrumbs'
import pluginSession from '@bugsnag/plugin-browser-session'
import React from 'react'
import pluginDevice from './device'
import nativeBreadcrumbs from './breadcrumb_native'

const name = 'Bugsnag Kepler'
const url = 'https://github.com/bugsnag/bugsnag-kepler'

Event.__type = 'reactnativejs'

const keplerEventFactory = (createCoreEvent, nativeStateInfo) => (...createEventArgs) => {
  const event = createCoreEvent(...createEventArgs)
  Object.assign(event.app, nativeStateInfo.app)
  return event
}

export const Bugsnag = {
  _client: null,
  createClient: (opts) => {
    // handle very simple use case where user supplies just the api key as a string
    if (typeof opts === 'string') opts = { apiKey: opts }
    if (!opts) opts = {}

    const internalPlugins = [
      createBugsnagGlobalErrorHandlerPlugin(),
      unhandledRejectionPlugin,
      pluginConsoleBreadcrumbs,
      createPluginNetworkBreadcrumbs(),
      pluginSession,
      pluginDevice,
      pluginAppDuration,
      new BugsnagPluginReact(React)
    ]

    // configure a client with user supplied options
    const bugsnag = new Client(opts, schema, internalPlugins, { name, version, url })
    const nativeStaticInfo = BugsnagKeplerNative.configure({
      apiKey: opts.apiKey
    })

    nativeBreadcrumbs.register(bugsnag)

    Event.create = keplerEventFactory(Event.create, nativeStaticInfo)

    const userStore = createUserStore(bugsnag._config.persistenceDirectory, bugsnag._config.persistUser)
    const user = userStore.load(bugsnag._config.user)
    bugsnag.setUser(user.id, user.email, user.name)

    const deviceStore = createDeviceStore(bugsnag._config.persistenceDirectory)
    const deviceInfo = deviceStore.load()
    pluginDevice.setDeviceID(deviceInfo.id)

    bugsnag._setDelivery(delivery)
    bugsnag.markLaunchComplete = () => {
      BugsnagKeplerNative.markLaunchCompleted()
    }

    bugsnag._logger.debug('Loaded!')
    bugsnag.leaveBreadcrumb('Bugsnag loaded', {}, 'state')

    return bugsnag._config.autoTrackSessions
      ? bugsnag.startSession()
      : bugsnag
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
