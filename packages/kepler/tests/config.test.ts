import type KeplerBugsnagStatic from '..'

const API_KEY = '030bab153e7c2349be364d23b5ae93b5'

const config = {
  apiKey: API_KEY,
  enabledBreadcrumbTypes: [],
  autoTrackSessions: false
}

describe('kepler config', () => {
  let Bugsnag: typeof KeplerBugsnagStatic

  beforeEach(() => {
    jest.spyOn(console, 'debug').mockImplementation(() => {})
    jest.spyOn(console, 'warn').mockImplementation(() => {})
    jest.isolateModules(() => {
      Bugsnag = require('../lib/notifier').default
    })
  })

  describe('logger', () => {
    it('uses the supplied logger', () => {
      const logger = { debug: jest.fn(), info: jest.fn(), warn: jest.fn(), error: jest.fn() }
      Bugsnag.start({ logger, ...config })
      expect(logger.debug).toHaveBeenCalledWith('Loaded!')
    })

    it('uses the default logger when none is supplied', () => {
      Bugsnag.start(config)
      expect(console.debug).toHaveBeenCalledWith('[bugsnag]', 'Loaded!')
    })

    it('logs the error message when an Error object is passed to logger.warn', () => {
      const client = Bugsnag.start(config)

      // @ts-expect-error property '_logger' does not exist on type 'Client'
      client._logger.warn(new Error('oh no'))
      expect(console.warn).toHaveBeenCalledWith('[bugsnag]', 'oh no')
    })
  })
})
