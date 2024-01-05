import Bugsnag from '..'

describe('kepler notifier', () => {
  describe('isStarted', () => {
    it('returns false when the notifier has not been initialised', () => {
      expect(Bugsnag.isStarted()).toBe(false)
    })

    it('returns true when the notifier has been initialised', () => {
      Bugsnag.start('abcd12abcd12abcd12abcd12abcd12abcd')
      expect(Bugsnag.isStarted()).toBe(true)
    })
  })
})
