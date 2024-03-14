import createEventQueue from './event_store'
import payload from '@bugsnag/core/lib/json-payload'
import { BugsnagFileIO } from '@bugsnag/kepler-native'

const isHttpStatusFatal = (responseCode) =>
  responseCode >= 400 && responseCode <= 499 &&
  // these codes are allowed to retry:
  responseCode !== 402 &&
  responseCode !== 407 &&
  responseCode !== 408 &&
  responseCode !== 429

const isHttpStatusSuccess = (responseCode) => responseCode >= 200 && responseCode <= 299

const delivery = (client, fetch = global.fetch) => {
  const fileQueue = createEventQueue(client._config.persistenceDirectory)

  const enqueueNextEvent = () => {
    const queueEntry = fileQueue.nextEvent()
    if (queueEntry) {
      const url = client._config.endpoints.notify
      fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Bugsnag-Api-Key': queueEntry.apiKey,
          'Bugsnag-Payload-Version': '4',
          'Bugsnag-Sent-At': (new Date()).toISOString(),
          'Bugsnag-Integrity': `sha1 ${BugsnagFileIO.sha1(queueEntry.eventString)}`
        },
        body: queueEntry.eventString
      }).then(response => {
        const statusCode = response.status
        if (isHttpStatusSuccess(statusCode) || isHttpStatusFatal(statusCode)) {
          fileQueue.deleteEvent(queueEntry.file)
          enqueueNextEvent()
        }
      }).catch(err => {
        // there is no retry *or* continue on an Error, we assume there is a network problem and wait
        // until a reconnection event, or another sendEvent call, or similar
        client._logger.error(err)
      })
    }
  }

  // try to send the first event immediately
  setImmediate(() => enqueueNextEvent())

  return {
    sendEvent: (event, cb = () => {}) => {
      try {
        const eventPayload = payload.event(event, client._config.redactedKeys)
        fileQueue.writeEvent(eventPayload, event.apiKey || client._config.apiKey)
        enqueueNextEvent(fileQueue, fetch)
        cb(null)
      } catch (err) {
        client._logger.error(err)
        cb(err)
      }
    },
    sendSession: (session, cb = () => {}) => {
      const url = client._config.endpoints.sessions
      const sessionPayload = payload.session(session, client._config.redactedKeys)
      fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Bugsnag-Api-Key': client._config.apiKey,
          'Bugsnag-Payload-Version': '1',
          'Bugsnag-Sent-At': (new Date()).toISOString(),
          'Bugsnag-Integrity': `sha1 ${BugsnagFileIO.sha1(sessionPayload)}`
        },
        body: sessionPayload
      }).then(() => {
        cb(null)
      }).catch(err => {
        client._logger.error(err)
        cb(err)
      })
    }
  }
}

export default delivery
