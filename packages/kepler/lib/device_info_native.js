import { Platform } from 'react-native'
import DeviceInfo from '@amzn/react-native-device-info'
import createDeviceStore from './device_store'

const getEngine = () => global.HermesInternal ? 'hermes' : 'unknown'

const getReactNativeVersion = () => {
  const { major, minor, patch } = Platform.constants.reactNativeVersion
  return `${major}.${minor}.${patch}`
}

const nativeDeviceInfo = {
  register: (client) => {
    const deviceStore = createDeviceStore(client._config.persistenceDirectory)
    const { id } = deviceStore.load()

    const device = {
      id,
      manufacturer: DeviceInfo.getManufacturerSync(),
      model: DeviceInfo.getModel(),
      osName: DeviceInfo.getSystemName(),
      osVersion: DeviceInfo.getSystemVersion(),
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

    // TODO: sync with native
  }
}

export default nativeDeviceInfo
