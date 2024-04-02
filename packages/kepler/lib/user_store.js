import { BugsnagFileIO, isErrorResult } from '@bugsnag/kepler-native'

export default (dir, persistUserConfig) => {
  const persistUser = persistUserConfig
  const userFile = `${dir}/user-info`
  let previousUser = null

  // try to make sure the directory exists (if it doesn't already)
  BugsnagFileIO.mkdir(dir)

  return {
    load: function (initialUser) {
      // try to set user from the config
      if (this.validUser(initialUser)) {
        this.save(initialUser)
        return initialUser
      }

      // user from config is invalid, try to load from disk
      const result = BugsnagFileIO.readTextFile(userFile)
      if (!isErrorResult(result)) {
        try {
          const savedUser = JSON.parse(result.content)
          if (this.validUser(savedUser)) {
            previousUser = savedUser
            return savedUser
          }
        } catch (e) {
          // ignore errors reading the file, set default user
        }
      }

      // no user on disk, create a default user
      // TODO getDeviceId
      const defaultUser = { id: 'deviceId', name: undefined, email: undefined }
      this.save(defaultUser)
      return defaultUser
    },
    validUser: function (user) {
      return user && user.id && user.name && user.email
    },
    save: function (user) {
      if (persistUser && user !== previousUser) {
        BugsnagFileIO.writeTextFile(userFile, JSON.stringify(user))
        previousUser = user
      }
    }
  }
}
