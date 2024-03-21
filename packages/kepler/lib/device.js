import { Platform } from 'react-native'

const getEngine = () => global.HermesInternal ? 'hermes' : 'unknown'

const getReactNativeVersion = () => {
  const { major, minor, patch } = Platform.constants.reactNativeVersion
  return `${major}.${minor}.${patch}`
}

const pluginDevice = {
  load: (client) => {
    const device = {
      osName: 'kepler',
      runtimeVersions: {
        reactNative: getReactNativeVersion(),
        reactNativeJsEngine: getEngine()
      }
    }

    client.addOnSession(session => {
      session.device = {
        ...session.device,
        ...device
      }
    })

    client.addOnError((event) => {
      event.device = {
        ...event.device,
        ...device,
        time: new Date()
      }
    }, true)
  }
}

export default pluginDevice
