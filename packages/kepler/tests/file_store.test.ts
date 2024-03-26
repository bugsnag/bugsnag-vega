import createEventQueue from '../lib/file_store'
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

  it('nextItem removes events from queue, but not files', () => {
    const eventQueue = createEventQueue('/tmp/queue')
    let event = eventQueue.nextItem()

    expect(event).not.toBeNull()
    expect(event.apiKey).toEqual(API_KEY)
    expect(event.file.isFile).toBeTruthy()
    expect(event.file.name).toBe("202403051447450978_030bab153e7c2349be364d23b5ae93b5.json")

    event = eventQueue.nextItem()
    expect(event).not.toBeNull()
    expect(event.apiKey).toEqual(API_KEY)
    expect(event.file.isFile).toBeTruthy()
    expect(event.file.name).toBe("202403051448060148_030bab153e7c2349be364d23b5ae93b5.json")

    event = eventQueue.nextItem()
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

    eventQueue.deleteItem(file)

    expect(eventQueue.queue).toHaveLength(2)
    expect(eventQueue.queue).not.toContainEqual(file)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(1)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('deletes files if over the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 1
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.deleteOldItemsIfNeeded(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(2)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
    expect(calls[1][0]).toEqual('/tmp/queue/202403051448060148_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('deletes one file if nearing the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 3
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.deleteOldItemsIfNeeded(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(1)
    expect(calls[0][0]).toEqual('/tmp/queue/202403051447450978_030bab153e7c2349be364d23b5ae93b5.json')
  })

  it('does not delete files if under the maxPersistedEvents limit', () => {
    const maxPersistedEvents = 4
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.deleteOldItemsIfNeeded(maxPersistedEvents)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(0)
  })

  it('does not save files if maxPersistedEvents is 0', () => {
    const maxPersistedEvents = 0
    const eventQueue = createEventQueue('/tmp/queue')
    const eventPayload = payload.event({ severity: "error" }, ['password'])
    eventQueue.writeItem(eventPayload, API_KEY, maxPersistedEvents)

    const calls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(0)
  })

  it('deletes items older than 60 days', () => {
    const file = {
      name: '202312110100000000_030bab153e7c2349be364d23b5ae93b5.json',
      isFile: true,
      isDirectory: false,
    }
    const event = {
      file,
      apiKey: API_KEY,
      valueString: ""
    }
    const eventQueue = createEventQueue('/tmp/queue')
    eventQueue.discardIfPastRetention(event)

    const calls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(calls).toHaveLength(1)
  })

  it('does not delete items newer than 60 days', () => {
    (BugsnagFileIO.listDirectory as jest.Mock).mockReturnValue([])
    const eventQueue = createEventQueue('/tmp/queue')

    // create fresh event with today's date
    const eventPayload = payload.event({ severity: "error" }, ['password'])
    eventQueue.writeItem(eventPayload, API_KEY, 10)
    const event = eventQueue.nextItem()
    const writeCalls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
    expect(writeCalls).toHaveLength(1)

    eventQueue.discardIfPastRetention(event)

    const deleteCalls = (BugsnagFileIO.deleteFile as jest.Mock).mock.calls
    expect(deleteCalls).toHaveLength(0)
  })
})
