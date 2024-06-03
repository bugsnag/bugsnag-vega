import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import { Client } from '@bugsnag/core'

const nativeUser = {
  register: () => {
    const origSetUser = Client.prototype.setUser
    Client.prototype.setUser = function (...args) {
      origSetUser.apply(this, args)
      const userInfo = this._user
      if (userInfo.id === undefined && userInfo.email === undefined && userInfo.name === undefined) {
        BugsnagKeplerNative.clearUser()
      } else {
        BugsnagKeplerNative.setUser(userInfo)
      }
    }
  }
}

export default nativeUser
