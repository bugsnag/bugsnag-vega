import type KeplerBugsnagStatic from '..'

describe('kepler notifier', () => {
  let Bugsnag: typeof KeplerBugsnagStatic

  beforeAll(() => {
    jest.spyOn(console, 'debug').mockImplementation(() => {})
    jest.spyOn(console, 'warn').mockImplementation(() => {})
  })

  beforeEach(() => {
    jest.isolateModules(() => {
      Bugsnag = require('..')
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
