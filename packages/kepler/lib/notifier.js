import { version } from '../package.json'
import { Client, Event } from '@bugsnag/core'
import { schema } from './config'
import delivery from './delivery'
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
import nativeBreadcrumbs from './breadcrumb_native'
import nativeMetadata from './metadata_native'
import nativeFeatureFlags from './features_native'
import nativeUser from './user_native'
import nativeApp from './app_info_native'
import nativeDevice from './device_info_native'

const name = 'Bugsnag Kepler'
const url = 'https://github.com/bugsnag/bugsnag-kepler'

Event.__type = 'reactnativejs'

export const Bugsnag = {
  _client: null,
  createClient: (opts) => {
    console.log('[Bugsnag] create client started with options:', opts)
    // handle very simple use case where user supplies just the api key as a string
    if (typeof opts === 'string') opts = { apiKey: opts }
    if (!opts) opts = {}

    console.log('[Bugsnag] starting plugins')
    const internalPlugins = [
      createBugsnagGlobalErrorHandlerPlugin(),
      unhandledRejectionPlugin,
      pluginConsoleBreadcrumbs,
      createPluginNetworkBreadcrumbs(),
      pluginSession,
      pluginAppDuration,
      new BugsnagPluginReact(React)
    ]
    console.log('[Bugsnag] finished starting plugins')

    // configure a client with user supplied options
    const bugsnag = new Client(opts, schema, internalPlugins, { name, version, url })
    console.log('[Bugsnag] client created with options:', opts)
    const nativeStaticInfo = BugsnagKeplerNative.configure({
      dummyStrValue: opts.apiKey,
      apikey: opts.apiKey,
      persistenceDirectory: bugsnag._config.persistenceDirectory,
      enabledErrorTypes: bugsnag._config.enabledErrorTypes
    })
    console.log('[Bugsnag] native static info:', nativeStaticInfo)

    const deviceStore = createDeviceStore(bugsnag._config.persistenceDirectory)
    const { id: deviceId } = deviceStore.load()

    console.log('[Bugsnag] device id:', deviceId)

    nativeBreadcrumbs.register(bugsnag)
    nativeMetadata.register()
    nativeFeatureFlags.register()
    nativeUser.register(bugsnag, deviceId)
    nativeApp.register(bugsnag, nativeStaticInfo.app)
    nativeDevice.register(bugsnag, deviceId)

    console.log('[Bugsnag] native modules registered')

    bugsnag._setDelivery(delivery)
    bugsnag.markLaunchComplete = () => {
      BugsnagKeplerNative.markLaunchCompleted()
    }

    console.log('[Bugsnag] client configured with delivery and launch complete handler')

    bugsnag._logger.debug('Loaded!')
    bugsnag.leaveBreadcrumb('Bugsnag loaded', {}, 'state')

    console.log('[Bugsnag] loaded')

    return bugsnag._config.autoTrackSessions
      ? bugsnag.startSession()
      : bugsnag
  },
  start: (opts) => {
    console.log('[Bugsnag] start() called with options:', opts)
    if (Bugsnag._client) {
      console.log('[Bugsnag] start() was called more than once. Ignoring.')
      Bugsnag._client._logger.warn('Bugsnag.start() was called more than once. Ignoring.')
      return Bugsnag._client
    }
    console.log('[Bugsnag] starting client')
    Bugsnag._client = Bugsnag.createClient(opts)
    console.log('[Bugsnag] client created')
    Bugsnag._client.markLaunchComplete()
    console.log('[Bugsnag] launch complete marked')
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
