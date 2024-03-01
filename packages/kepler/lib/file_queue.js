import { BugsnagKeplerNative } from '@bugsnag/kepler-native'

const derecursify = require('@bugsnag/core/lib/derecursify')

export default (apiKey, dir) => ({
  writeEvent: (event) => {
    const date = new Date()
    const filename = [date.toUTCString(), event.apiKey || apiKey].join('_')
    BugsnagKeplerNative.writeTextFile(`${dir}/${filename}.json`, JSON.stringify(derecursify(event)))
  },
  nextEvent: () => {
    const entries = BugsnagKeplerNative.listDirectory(dir)
    if (entries.length === 0) return null

    entries.sort((a, b) => a.name < b.name ? -1 : a.name > b.name ? 1 : 0)

    const file = entries[0]
    const path = `${dir}/${file.name}`
    try {
      return {
        file,
        event: JSON.parse(BugsnagKeplerNative.readTextFile(path))
      }
    } catch (e) {
      BugsnagKeplerNative.deleteFile(path)
      return null
    }
  },
  deleteEvent: (file) => {
    BugsnagKeplerNative.deleteFile(`${dir}/${file.name}`)
  }
})
