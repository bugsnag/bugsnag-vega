import nativeUser from '../lib/user_native'
import { Client } from '@bugsnag/core'
import { schema } from '../lib/config'
import { BugsnagFileIO, BugsnagKeplerNative } from "@bugsnag/kepler-native"

const VALID_API_KEY = 'a1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6'

describe('user info', () => {
  beforeEach(() => {
    jest.clearAllMocks()
  })

  describe('register', () => {
    it('syncs config user with native layer', () => {
      const setNativeUser = BugsnagKeplerNative.setUser as jest.Mock
  
      const initialUser = { id: '123', name: 'Bug Snag', email: 'bug@sn.ag' }
  
      // @ts-expect-error client constructor is protected
      const client = new Client({ apiKey: VALID_API_KEY, persistenceDirectory: '/tmp/user', user: initialUser }, schema, [])
  
      nativeUser.register(client, 'abc123')
      expect(setNativeUser).toHaveBeenCalledWith(initialUser)
    })

    it('sets default user using device ID', () => {
      const setNativeUser = BugsnagKeplerNative.setUser as jest.Mock
    
      // @ts-expect-error client constructor is protected
      const client = new Client({ apiKey: VALID_API_KEY, persistenceDirectory: '/tmp/user' }, schema, [])
  
      nativeUser.register(client, 'deviceId')
      expect(setNativeUser).toHaveBeenCalledWith({ id: 'deviceId' })
    })
  })

  describe('setUser', () => {
    it('syncs user info with native layer when setUser is called', () => {
      const setNativeUser = BugsnagKeplerNative.setUser as jest.Mock
      const initialUser = { id: '123' }
  
      // @ts-expect-error client constructor is protected
      const client = new Client({ apiKey: VALID_API_KEY, persistenceDirectory: '/tmp/user', user: initialUser }, schema, [])
  
      nativeUser.register(client, 'deviceId')
      expect(setNativeUser).toHaveBeenCalledTimes(1)
      expect(setNativeUser).toHaveBeenCalledWith(initialUser)
  
      const updatedUser = { id: '456', email: 'bug@sn.ag', name: 'Bug Snag' }
      client.setUser(updatedUser.id, updatedUser.email, updatedUser.name)
  
      expect(setNativeUser).toHaveBeenCalledTimes(2)
      expect(setNativeUser.mock.calls[1][0]).toEqual(updatedUser)
    })

    it('saves user info to store when setUser is called', () => {
      const writeTextFile = BugsnagFileIO.writeTextFile as jest.Mock
  
      const initialUser = { id: '123' }
  
      // @ts-expect-error client constructor is protected
      const client = new Client({
        apiKey: VALID_API_KEY,
        user: initialUser,
        persistUser: true,
        persistenceDirectory: '/tmp/user',
      }, schema, [])
  
      nativeUser.register(client, 'deviceId')
      expect(writeTextFile).toHaveBeenCalledTimes(2)
  
      const updatedUser = { id: '456', email: 'bug@sn.ag', name: 'Bug Snag' }
      client.setUser(updatedUser.id, updatedUser.email, updatedUser.name)
  
      expect(writeTextFile).toHaveBeenCalledTimes(3)
      expect(writeTextFile.mock.calls[2][1]).toEqual(JSON.stringify(updatedUser))
    })

    it('clears native user when user is invalid', () => {
      const setNativeUser = BugsnagKeplerNative.setUser as jest.Mock
      const clearNativeUser = BugsnagKeplerNative.clearUser as jest.Mock

      // @ts-expect-error client constructor is protected
      const client = new Client({ apiKey: VALID_API_KEY, persistenceDirectory: '/tmp/user' }, schema, [])
  
      nativeUser.register(client, 'deviceId')
      expect(setNativeUser).toHaveBeenCalledTimes(1)
      expect(setNativeUser).toHaveBeenCalledWith({ id: 'deviceId' })
  
      client.setUser(null, null, null)
  
      expect(clearNativeUser).toHaveBeenCalledTimes(1)
    })
  })
})