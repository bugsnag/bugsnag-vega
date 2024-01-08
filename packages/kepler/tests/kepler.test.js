const kepler = require('..')

describe('kepler', () => {
  it('returns a string', () => {
    expect(kepler()).toStrictEqual('Hello from kepler')
  })
})
