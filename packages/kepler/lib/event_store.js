import { BugsnagFileIO, isErrorResult } from '@bugsnag/kepler-native'

const loadQueuedEventFiles = (dir) => {
  const entries = BugsnagFileIO.listDirectory(dir) || []
  // should be sorted by name property, otherwise the order is not guaranteed
  return entries.sort((a, b) => { return (a.name > b.name) ? 1 : ((b.name > a.name) ? -1 : 0) })
}

const getApiKeyFromFilename = (name) => {
  const match = name.match(/^[0-9]+_([^.]+)\.json$/)
  return match ? match[1] : null
}

const dirEntryEquals = (a, b) => a && b && a.name === b.name && a.isFile === b.isFile && a.isDirectory === b.isDirectory

function createFilename (apiKey) {
  const date = new Date()
  const formattedDate = [
    date.getFullYear().toString().padStart(4, '0'),
    (date.getMonth() + 1).toString().padStart(2, '0'),
    date.getDate().toString().padStart(2, '0'),
    date.getHours().toString().padStart(2, '0'),
    date.getMinutes().toString().padStart(2, '0'),
    date.getSeconds().toString().padStart(2, '0'),
    date.getMilliseconds().toString().padStart(4, '0')
  ].join('')

  return `${formattedDate}_${apiKey}.json`
}

export default (dir) => {
  // try to make sure the directory exists (if it doesn't already)
  BugsnagFileIO.mkdir(dir)

  return {
    queue: loadQueuedEventFiles(dir),
    deleteOldEventsIfNeeded: function (maxPersistedEvents) {
      const entries = loadQueuedEventFiles(dir)
      if (entries.length >= maxPersistedEvents) {
        const endIdx = entries.length === maxPersistedEvents ? 1 : entries.length - maxPersistedEvents
        const toDelete = entries.slice(0, endIdx)
        toDelete.forEach(file => this.deleteEvent(file))
      }
    },
    writeEvent: function (eventString, apiKey, maxPersistedEvents) {
      const fileName = createFilename(apiKey)
      if (maxPersistedEvents !== 0) {
        this.deleteOldEventsIfNeeded(maxPersistedEvents)
        BugsnagFileIO.writeTextFile(`${dir}/${fileName}`, eventString)
      }
      this.queue.push({ name: fileName, isFile: true, isDirectory: false })
    },
    nextEvent: function () {
      let entries = this.queue
      if (!entries || entries.length === 0) {
        // reload the queue to make sure it's up-to-date
        entries = loadQueuedEventFiles(dir)
        this.queue = entries
      }

      const file = entries.shift()
      if (!file) {
        return null
      }

      const path = `${dir}/${file.name}`
      try {
        const result = BugsnagFileIO.readTextFile(path)
        const failed = isErrorResult(result)
        if (failed) {
          throw new Error(result.message)
        }

        return {
          file,
          apiKey: getApiKeyFromFilename(file.name),
          eventString: result.content
        }
      } catch (e) {
        BugsnagFileIO.deleteFile(path)
        return null
      }
    },
    deleteEvent: function (file) {
      // remove the file from storage
      BugsnagFileIO.deleteFile(`${dir}/${file.name}`)

      // remove the file from the queue (if it's there)
      const queue = this.queue
      if (queue) {
        const fileIndex = queue.findIndex(f => dirEntryEquals(f, file))
        if (fileIndex !== -1) {
          queue.splice(fileIndex, 1)
        }
      }
    }
  }
}
