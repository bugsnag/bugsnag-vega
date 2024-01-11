import type KeplerBugsnagStatic from '..'

const API_KEY = '030bab153e7c2349be364d23b5ae93b5'

const typedGlobal: any = global

function mockFetch () {
  typedGlobal.fetch = jest.fn(() => Promise.resolve({ json: () => Promise.resolve() }))
}

describe('kepler notifier', () => {
  let Bugsnag: typeof KeplerBugsnagStatic

  beforeAll(() => {
    jest.spyOn(console, 'debug').mockImplementation(() => {})
    jest.spyOn(console, 'warn').mockImplementation(() => {})
  })

  beforeEach(() => {
    mockFetch()
    jest.isolateModules(() => {
      Bugsnag = require('../lib/notifier').default
    })
  })

  it('notifies handled errors', (done) => {
    Bugsnag.start(API_KEY)
    Bugsnag.notify(new Error('123'), undefined, (err, event) => {
      if (err) done(err)
      expect(event.originalError.message).toBe('123')
      expect(typedGlobal.fetch).toHaveBeenCalledWith('https://notify.bugsnag.com', expect.objectContaining({
        method: 'POST',
        headers: expect.objectContaining({
          'Bugsnag-Api-Key': API_KEY,
          'Bugsnag-Payload-Version': '4',
          'Bugsnag-Sent-At': expect.stringMatching(/^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z$/),
          'Content-Type': 'application/json'
        })
      }))
      done()
    })
  })

  describe('isStarted', () => {
    it('returns false when the notifier has not been initialised', () => {
      expect(Bugsnag.isStarted()).toBe(false)
    })

    it('returns true when the notifier has been initialised', () => {
      Bugsnag.start('030bab153e7c2349be364d23b5ae93b5')
      expect(Bugsnag.isStarted()).toBe(true)
    })
  })
})
