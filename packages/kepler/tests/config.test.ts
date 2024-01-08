import type KeplerBugsnagStatic from '..'

const API_KEY = '030bab153e7c2349be364d23b5ae93b5'

describe('kepler config', () => {
  let Bugsnag: typeof KeplerBugsnagStatic

  beforeEach(() => {
    jest.isolateModules(() => {
      Bugsnag = require('..')
    })
  })

  describe('logger', () => {
    it('uses the supplied logger', () => {
      const logger = { debug: jest.fn(), info: jest.fn(), warn: jest.fn(), error: jest.fn() }
      Bugsnag.start({ apiKey: API_KEY, logger })
      expect(logger.debug).toHaveBeenCalledWith('Loaded!')
    })

    it('uses the default logger when none is supplied', () => {
      jest.spyOn(console, 'debug').mockImplementation(() => {})
      Bugsnag.start({ apiKey: API_KEY })
      expect(console.debug).toHaveBeenCalledWith('[bugsnag]', 'Loaded!')
    })
  })
})
