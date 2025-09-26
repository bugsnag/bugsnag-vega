import DeviceInfo from '@amzn/react-native-device-info'
import { Event } from '@bugsnag/core'
import { BugsnagVegaNative } from '@bugsnag/vega-native'

const vegaEventFactory = (createCoreEvent, app) => (...createEventArgs) => {
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

    BugsnagVegaNative.setApp(app)
    Event.create = vegaEventFactory(Event.create, app)
  }
}

export default nativeAppInfo
