import { BugsnagKeplerNative } from '@bugsnag/kepler-native'
import createUserStore, { isValidUser } from './user_store'

const nativeUser = {
  register: (client, deviceId) => {
    const userStore = createUserStore(client._config.persistenceDirectory, client._config.persistUser)
    const initialUser = userStore.load(client._config.user, deviceId)

    const origSetUser = client.setUser
    client.setUser = function (...args) {
      origSetUser.apply(this, args)
      const userInfo = this._user

      if (isValidUser(userInfo)) {
        BugsnagKeplerNative.setUser(userInfo)
      } else {
        BugsnagKeplerNative.clearUser()
      }
    }

    // set the initial user loaded from the store
    client.setUser(initialUser.id, initialUser.email, initialUser.name)
  }
}

export default nativeUser
