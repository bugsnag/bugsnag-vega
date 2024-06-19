import DeviceInfo from '@amzn/react-native-device-info'
import { Event } from '@bugsnag/core'
import { BugsnagKeplerNative } from '@bugsnag/kepler-native'

const keplerEventFactory = (createCoreEvent, app) => (...createEventArgs) => {
  const event = createCoreEvent(...createEventArgs)
  Object.assign(event.app, app)
  return event
}

const nativeAppInfo = {
  register: (client, nativeStaticApp) => {
    const app = {
      id: DeviceInfo.getBundleId(),
      releaseStage: client._config.releaseStage,
      type: client._config.appType,
      version: client._config.appVersion,
      ...nativeStaticApp
    }

    BugsnagKeplerNative.setApp(app)
    Event.create = keplerEventFactory(Event.create, app)
  }
}

export default nativeAppInfo
