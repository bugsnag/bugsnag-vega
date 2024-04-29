import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import { Client } from '@bugsnag/core'

const nativeMetadata = {
  register: () => {
    // wrap add single and multiple
    const origAddMetadata = Client.prototype.addMetadata
    Client.prototype.addMetadata = function (...args) {
      origAddMetadata.apply(this, args)
      const metadataAll = this._metadata
      const metadataStr = JSON.stringify(metadataAll)
      BugsnagKeplerNative.addMetadata(metadataStr)
    }

    // wrap clear
    const origClearMetadata = Client.prototype.clearMetadata
    Client.prototype.clearMetadata = function (...args) {
      origClearMetadata.apply(this, args)
      BugsnagKeplerNative.clearMetadata()
    }
  }
}

export default nativeMetadata
