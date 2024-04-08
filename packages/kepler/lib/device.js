import { Platform } from 'react-native'

const getEngine = () => global.HermesInternal ? 'hermes' : 'unknown'

const getReactNativeVersion = () => {
  const { major, minor, patch } = Platform.constants.reactNativeVersion
  return `${major}.${minor}.${patch}`
}

let deviceID

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
        ...device,
        id: deviceID
      }
    })

    client.addOnError((event) => {
      event.device = {
        ...event.device,
        ...device,
        id: deviceID,
        time: new Date()
      }
    }, true)
  },
  setDeviceID: (id) => {
    deviceID = id
  }
}

export default pluginDevice
