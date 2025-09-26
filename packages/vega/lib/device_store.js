import { BugsnagVegaNative, BugsnagFileIO, isErrorResult } from '@bugsnag/vega-native'

export default (dir) => {
  const deviceFile = `${dir}/device.json`

  // try to make sure the directory exists (if it doesn't already)
  BugsnagFileIO.mkdir(dir)

  return {
    load: function () {
      // try to read device info from disk
      const result = BugsnagFileIO.readTextFile(deviceFile)
      if (!isErrorResult(result)) {
        try {
          const savedDeviceID = JSON.parse(result.content)
          if (savedDeviceID.id) {
            BugsnagVegaNative.setDeviceID(savedDeviceID.id)
            return savedDeviceID
          }
        } catch (e) {
          // ignore errors reading the file
        }
      }

      // no deviceInfo on disk, save with generated deviceId
      const deviceID = BugsnagVegaNative.getDeviceID()
      const newDeviceInfo = { id: deviceID }
      this.save(newDeviceInfo)
      return newDeviceInfo
    },
    save: function (deviceInfo) {
      BugsnagFileIO.writeTextFile(deviceFile, JSON.stringify(deviceInfo))
    }
  }
}
