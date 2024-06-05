import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import { Client } from '@bugsnag/core'

const nativeAppInfo = {
  register: () => {
    const origConfigure = Client.prototype._configure
    Client.prototype._configure = function (...args) {
      origConfigure.apply(this, args)
      const config = this._config

      BugsnagKeplerNative.setApp({
        releaseStage: config.releaseStage,
        type: config.appType,
        version: config.appVersion
      })
    }
  }
}

export default nativeAppInfo
