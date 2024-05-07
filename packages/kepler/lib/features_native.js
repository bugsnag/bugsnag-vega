import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import { Client } from '@bugsnag/core'

const nativeFeatures = {
  register: () => {
    // wrap add single
    const origAddOneFeature = Client.prototype.addFeatureFlag
    Client.prototype.addFeatureFlag = function (...args) {
      origAddOneFeature.apply(this, args)
      const featuresAll = this._features
      const featuresStr = JSON.stringify(featuresAll)
      BugsnagKeplerNative.setFeatures(featuresStr)
    }

    // wrap add multiple
    const origAddMultipleFeatures = Client.prototype.addFeatureFlags
    Client.prototype.addFeatureFlags = function (...args) {
      origAddMultipleFeatures.apply(this, args)
      const featuresAll = this._features
      const featuresStr = JSON.stringify(featuresAll)
      BugsnagKeplerNative.setFeatures(featuresStr)
    }

    // wrap clear
    const origClearOneFeature = Client.prototype.clearFeatureFlag
    Client.prototype.clearFeatureFlag = function (...args) {
      origClearOneFeature.apply(this, args)
      const metadataAll = this._metadata
      const metadataStr = JSON.stringify(metadataAll)
      BugsnagKeplerNative.setFeatures(metadataStr)
    }

    // wrap clear all
    const origClearAllFeatures = Client.prototype.clearFeatureFlags
    Client.prototype.clearFeatureFlags = function (...args) {
      origClearAllFeatures.apply(this, args)
      BugsnagKeplerNative.clearFeatures()
    }
  }
}

export default nativeFeatures
