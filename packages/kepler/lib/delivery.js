import createFileQueue from './file_store'
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

const EVENT_STORE_SUBDIRECTORY = 'errors'
const SESSION_STORE_SUBDIRECTORY = 'sessions'

const delivery = (client, fetch = global.fetch) => {
  const eventQueue = createFileQueue(client._config.persistenceDirectory + '/' + EVENT_STORE_SUBDIRECTORY)
  const sessionQueue = createFileQueue(client._config.persistenceDirectory + '/' + SESSION_STORE_SUBDIRECTORY)
  let inFlightEvent = false
  let inFlightSession = false

  const enqueueNextEvent = () => {
    const queueEntry = eventQueue.nextItem()
    if (queueEntry && !inFlightEvent) {
      inFlightEvent = true
      const url = client._config.endpoints.notify
      fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Bugsnag-Api-Key': queueEntry.apiKey,
          'Bugsnag-Payload-Version': '4',
          'Bugsnag-Sent-At': (new Date()).toISOString(),
          'Bugsnag-Integrity': `sha1 ${BugsnagFileIO.sha1(queueEntry.valueString)}`
        },
        body: queueEntry.valueString
      }).then(response => {
        const statusCode = response.status
        if (isHttpStatusSuccess(statusCode) || isHttpStatusFatal(statusCode)) {
          eventQueue.deleteItem(queueEntry.file)
          inFlightEvent = false
          enqueueNextEvent()
        } else {
          eventQueue.discardIfOlderThan60days(queueEntry)
        }
      }).catch(err => {
        // there is no retry *or* continue on an Error, we assume there is a network problem and wait
        // until a reconnection event, or another sendEvent call, or similar
        client._logger.error(err)
      }).finally(() => {
        inFlightEvent = false
      })
    }
  }

  const enqueueNextSession = () => {
    const queueEntry = sessionQueue.nextItem()
    if (queueEntry && !inFlightSession) {
      inFlightSession = true
      const url = client._config.endpoints.sessions
      fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Bugsnag-Api-Key': queueEntry.apiKey,
          'Bugsnag-Payload-Version': '1',
          'Bugsnag-Sent-At': (new Date()).toISOString(),
          'Bugsnag-Integrity': `sha1 ${BugsnagFileIO.sha1(queueEntry.valueString)}`
        },
        body: queueEntry.valueString
      }).then(response => {
        const statusCode = response.status
        if (isHttpStatusSuccess(statusCode) || isHttpStatusFatal(statusCode)) {
          sessionQueue.deleteItem(queueEntry.file)
          inFlightSession = false
          enqueueNextSession()
        } else {
          sessionQueue.discardIfOlderThan60days(queueEntry)
        }
      }).catch(err => {
        // there is no retry *or* continue on an Error, we assume there is a network problem and wait
        // until a reconnection event, or another sendEvent call, or similar
        client._logger.error(err)
      }).finally(() => {
        inFlightSession = false
      })
    }
  }

  // try to send the first event immediately
  enqueueNextEvent()
  enqueueNextSession()

  return {
    sendEvent: (event, cb = () => {}) => {
      try {
        const eventPayload = payload.event(event, client._config.redactedKeys)
        eventQueue.writeItem(eventPayload, event.apiKey || client._config.apiKey, client._config.maxPersistedEvents)
        enqueueNextEvent(eventQueue, fetch)
        cb(null)
      } catch (err) {
        client._logger.error(err)
        cb(err)
      }
    },
    sendSession: (session, cb = () => {}) => {
      try {
        const sessionPayload = payload.session(session, client._config.redactedKeys)
        sessionQueue.writeItem(sessionPayload, session.apiKey || client._config.apiKey, client._config.maxPersistedSessions)
        enqueueNextSession(sessionQueue, fetch)
        cb(null)
      } catch (err) {
        client._logger.error(err)
        cb(err)
      }
    }
  }
}

export default delivery
