import { BugsnagKeplerNative } from '@bugsnag/kepler-native'

const breadcrumbType = {
  error: 0,
  log: 1,
  manual: 2,
  navigation: 3,
  process: 4,
  request: 5,
  state: 6,
  user: 7
}

const nativeBreadcrumbs = {
  register: (client) => {
    client.addOnBreadcrumb(crumb => {
      const nativeType = breadcrumbType[crumb.type]
      let metadataStr = JSON.stringify(crumb.metadata)
      metadataStr = metadataStr === undefined ? '{}' : metadataStr
      const timeSeconds = Math.floor(Date.now() / 1000)
      BugsnagKeplerNative.leaveBreadcrumb({
        type: nativeType,
        message: crumb.message,
        metadata: metadataStr,
        timestamp: timeSeconds
      })
    }, true)
  }
}

export default nativeBreadcrumbs
