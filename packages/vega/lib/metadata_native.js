import { BugsnagVegaNative } from '@bugsnag/vega-native'
import { Client } from '@bugsnag/core'

const nativeMetadata = {
  register: () => {
    // wrap add single and multiple
    const origAddMetadata = Client.prototype.addMetadata
    Client.prototype.addMetadata = function (...args) {
      origAddMetadata.apply(this, args)
      const metadataAll = this._metadata
      const metadataStr = JSON.stringify(metadataAll)
      BugsnagVegaNative.setMetadata(metadataStr)
    }

    // wrap clear
    const origClearMetadata = Client.prototype.clearMetadata
    Client.prototype.clearMetadata = function (...args) {
      origClearMetadata.apply(this, args)
      const metadataAll = this._metadata
      const metadataStr = JSON.stringify(metadataAll)
      BugsnagVegaNative.setMetadata(metadataStr)
    }
  }
}

export default nativeMetadata
