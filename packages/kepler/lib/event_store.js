import { BugsnagFileIO, isErrorResult } from '@bugsnag/kepler-native'

const loadQueuedEventFiles = (dir) => {
  const entries = BugsnagFileIO.listDirectory(dir) || []
  return entries.sort()
}

const getApiKeyFromFilename = (name) => {
  const match = name.match(/^[0-9]+_([^.]+)\.json$/)
  return match ? match[1] : null
}

const getDateFromFilename = (name) => {
  const match = name.match(/^([0-9]+)_[^.]+\.json$/)
  return match ? parseDateFromFilename(match[1]) : null
}

const parseDateFromFilename = (date) => {
  const fileDate = new Date()
  fileDate.setFullYear(date.substring(0, 4))
  fileDate.setMonth(date.substring(4, 6))
  fileDate.setDate(date.substring(6, 8))
  fileDate.setHours(date.substring(8, 10))
  fileDate.setMinutes(date.substring(10, 12))
  fileDate.setSeconds(date.substring(12, 14))
  fileDate.setMilliseconds(date.substring(14))
  return fileDate
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
    checkMaxEvents: function (apiKey, maxPersistedEvents) {
      if (maxPersistedEvents === undefined) return

      const entries = BugsnagFileIO.listDirectory(dir)
      const apiKeyEntries = entries.filter(e => getApiKeyFromFilename(e.name) === apiKey)
      if (apiKeyEntries.length > maxPersistedEvents) {
        apiKeyEntries.sort((a, b) => getDateFromFilename(a.name) - getDateFromFilename(b.name))
        const toDelete = apiKeyEntries.slice(0, apiKeyEntries.length - maxPersistedEvents)
        toDelete.forEach(file => BugsnagFileIO.deleteFile(`${dir}/${file.name}`))
      }
    },
    writeEvent: function (eventString, apiKey) {
      const fileName = createFilename(apiKey)
      BugsnagFileIO.writeTextFile(`${dir}/${fileName}`, eventString)
      this.queue.push({ name: fileName, isFile: true, isDirectory: false })
    },
    nextEvent: function () {
      let entries = this.queue
      if (!entries || entries.length === 0) {
        // reload the queue to make sure it's up-to-date
        entries = loadQueuedEventFiles(dir)
        this.queue = entries
      }

      const file = entries.pop()
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
