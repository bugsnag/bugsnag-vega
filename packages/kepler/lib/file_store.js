import { BugsnagFileIO, isErrorResult } from '@bugsnag/kepler-native'

const loadPersistedFiles = (dir) => {
  const entries = BugsnagFileIO.listDirectory(dir) || []
  // should be sorted by name property, otherwise the order is not guaranteed
  return entries.sort((a, b) => { return (a.name > b.name) ? 1 : ((b.name > a.name) ? -1 : 0) })
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
  const year = date.substring(0, 4)
  const month = date.substring(4, 6)
  const day = date.substring(6, 8)
  const hour = date.substring(8, 10)
  const minute = date.substring(10, 12)
  const second = date.substring(12, 14)
  const formatStr = `${year}-${month}-${day}T${hour}:${minute}:${second}`
  return new Date(formatStr)
}

const MAX_RETENTION_MILLISECONDS = 60 * 24 * 60 * 60 * 1000 // 60 days
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
    queue: loadPersistedFiles(dir),
    discardIfPastRetention: function (queueEntry) {
      const fileCreationDate = getDateFromFilename(queueEntry.file.name)
      if (fileCreationDate && fileCreationDate.getTime() + MAX_RETENTION_MILLISECONDS < Date.now()) {
        this.deleteItem(queueEntry.file)
      }
    },
    deleteOldItemsIfNeeded: function (maxPersistedItems) {
      const entries = loadPersistedFiles(dir)
      if (entries.length >= maxPersistedItems) {
        const endIdx = entries.length === maxPersistedItems ? 1 : entries.length - maxPersistedItems
        const toDelete = entries.slice(0, endIdx)
        toDelete.forEach(file => this.deleteItem(file))
      }
    },
    writeItem: function (eventString, apiKey, maxPersistedItems) {
      const fileName = createFilename(apiKey)
      if (maxPersistedItems !== 0) {
        this.deleteOldItemsIfNeeded(maxPersistedItems)
        BugsnagFileIO.writeTextFile(`${dir}/${fileName}`, eventString)
      }
      this.queue.push({ name: fileName, isFile: true, isDirectory: false })
    },
    nextItem: function () {
      let entries = this.queue
      if (!entries || entries.length === 0) {
        // reload the queue to make sure it's up-to-date
        entries = loadPersistedFiles(dir)
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
          valueString: result.content
        }
      } catch (e) {
        BugsnagFileIO.deleteFile(path)
        return null
      }
    },
    deleteItem: function (file) {
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
