import { BugsnagVegaNative } from '@bugsnag/vega-native'
import createUserStore, { isValidUser } from './user_store'

const nativeUser = {
  register: (client, deviceId) => {
    const userStore = createUserStore(client._config.persistenceDirectory, client._config.persistUser)
    const initialUser = userStore.load(client._config.user, deviceId)

    const origSetUser = client.setUser
    client.setUser = function (...args) {
      origSetUser.apply(this, args)
      const userInfo = this._user
      userStore.save(userInfo)

      if (isValidUser(userInfo)) {
        BugsnagVegaNative.setUser(userInfo)
      } else {
        BugsnagVegaNative.clearUser()
      }
    }

    // set the initial user loaded from the store
    client.setUser(initialUser.id, initialUser.email, initialUser.name)
  }
}

export default nativeUser
