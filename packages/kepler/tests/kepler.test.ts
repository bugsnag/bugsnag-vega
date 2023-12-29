import kepler from '../lib/kepler'

describe('kepler', () => {
  it('returns a string', () => {
    expect(kepler()).toStrictEqual('Hello from kepler')
  })
})
