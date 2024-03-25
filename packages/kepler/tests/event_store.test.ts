import createEventQueue from '../lib/event_store'
import payload from '@bugsnag/core/lib/json-payload'
import {BugsnagFileIO} from "@bugsnag/kepler-native"

const API_KEY = '030bab153e7c2349be364d23b5ae93b5'

describe('kepler event store', () => {
  beforeEach(() => {
    (BugsnagFileIO.listDirectory as jest.Mock).mockReturnValue([
      // we mock the filenames in reverse order, just to make sure the sort() does its work
      {name: '202403051448170469_030bab153e7c2349be364d23b5ae93b5.json', isFile: true, isDirectory: false},
      {name: '202403051448060148_030bab153e7c2349be364d23b5ae93b5.json', isFile: true, isDirectory: false},
      {name: '202403051447450978_030bab153e7c2349be364d23b5ae93b5.json', isFile: true, isDirectory: false},
    ])
  })
  afterEach(() => {
    jest.clearAllMocks()
  })

  it('nextEvent removes events from queue, but not files', () => {
    const eventQueue = createEventQueue('/tmp/queue')
    let event = eventQueue.nextEvent()

    expect(event).not.toBeNull()
    expect(event.apiKey).toEqual(API_KEY)
    expect(event.file.isFile).toBeTruthy()
    expect(event.file.name).toBe("202403051447450978_030bab153e7c2349be364d23b5ae93b5.json")

    event = eventQueue.nextEvent()
    expect(event).not.toBeNull()
    expect(event.apiKey).toEqual(API_KEY)
    expect(event.file.isFile).toBeTruthy()
    expect(event.file.name).toBe("202403051448060148_030bab153e7c2349be364d23b5ae93b5.json")

    event = eventQueue.nextEvent()
    expect(event).not.toBeNull()
    expect(event.apiKey).toEqual(API_KEY)
    expect(event.file.isFile).toBeTruthy()
    expect(event.file.name).toBe("202403051448170469_030bab153e7c2349be364d23b5ae93b5.json")

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(0)
  })

  it('deleteEvent', () => {
    const eventQueue = createEventQueue('/tmp/queue')
    // we fake the file, so we can check that it is also filtered from the in-memory queue
    const file = {
      name: '202403051447450978_030bab153e7c2349be364d23b5ae93b5.json',
      isFile: true,
      isDirectory: false,
    }

    eventQueue.deleteEvent(file)

    expect(eventQueue.queue).toHaveLength(2)
    expect(eventQueue.queue).not.toContainEqual(file)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(1)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('deletes files if over the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 1
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.checkMaxEvents(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(2)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
    expect(calls[1][0]).toEqual('/tmp/queue/202403051448060148_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('deletes one file if nearing the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 3
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.checkMaxEvents(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(1)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('does not delete files if under the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 4
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.checkMaxEvents(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(0)
  })

  it('does not save files if maxPersistedEvents is 0', () => {
    const maxPersistedEvents = 0
    const eventQueue = createEventQueue('/tmp/queue')
    const eventPayload = payload.event({ severity: "error" }, ['password'])
    eventQueue.writeEvent(eventPayload, API_KEY, maxPersistedEvents)

    const calls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(0)
  })
})
