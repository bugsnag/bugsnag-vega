import type KeplerBugsnagStatic from '..'

const API_KEY = '030bab153e7c2349be364d23b5ae93b5'

const config = {
  apiKey: API_KEY,
  enabledBreadcrumbTypes: [],
  autoTrackSessions: false
}

describe('kepler notifier', () => {
  let Bugsnag: typeof KeplerBugsnagStatic
  let BugsnagFileIO: any

  beforeAll(() => {
    jest.spyOn(console, 'debug').mockImplementation(() => {
    })
    jest.spyOn(console, 'warn').mockImplementation(() => {
    })
  })

  beforeEach(() => {
    jest.isolateModules(() => {
      Bugsnag = require('../lib/notifier').default
      BugsnagFileIO = require('@bugsnag/kepler-native').BugsnagFileIO
    })
  })

  it('notifies handled errors', (done) => {
    // @ts-expect-error global is not defined
    global.fetch = jest.fn().mockResolvedValue({ status: 200 })
    Bugsnag.start(config)
    Bugsnag.notify(new Error('123'), undefined, (err, event) => {
      if (err) done(err)
      expect(event.originalError.message).toBe('123')
      expect(BugsnagFileIO.writeTextFile).toHaveBeenCalledWith(
        expect.stringMatching(RegExp(`^\\/data\\/bugsnag\\/errors\\/\\d+_${API_KEY}\\.json$`)),
        expect.stringMatching(/^\{.*}$/)
      )
      done()
    })
  })

  describe('isStarted', () => {
    it('returns false when the notifier has not been initialised', () => {
      expect(Bugsnag.isStarted()).toBe(false)
    })

    it('returns true when the notifier has been initialised', () => {
      Bugsnag.start(config)
      expect(Bugsnag.isStarted()).toBe(true)
    })
  })
})
