import { BugsnagFileIO } from "@bugsnag/kepler-native"
import createUserStore from "../lib/user_store"

describe('kepler user store', () => {
    afterEach(() => {
        jest.clearAllMocks()
    })

    it('accepts correct user from config and writes to file', () => {
        const userStore = createUserStore('/tmp/user', true)
        const user = { id: '1', name: 'Alice', email: 'abcd' }

        const acceptedUser = userStore.load(user)
        expect(acceptedUser).toEqual(user)
        const calls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(calls).toHaveLength(1)
    })

    it('does not save user if persistence is disabled', () => {
        const userStore = createUserStore('/tmp/user', false)
        const user = { id: '1', name: 'Alice', email: 'abcd' }

        const acceptedUser = userStore.load(user)
        expect(acceptedUser).toEqual(user)
        const calls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(calls).toHaveLength(0)
    })

    it('loads user from file if configuration not provided', () => {
        (BugsnagFileIO.readTextFile as jest.Mock).mockReturnValue(
            { content: `{ "id": "0", "name": "Bob", "email": "efgh" }` }
        )

        const userStore = createUserStore('/tmp/user', true)

        const acceptedUser = userStore.load(undefined)
        expect(acceptedUser).toEqual({ id: '0', name: 'Bob', email: 'efgh' })
        const readCalls = (BugsnagFileIO.readTextFile as jest.Mock).mock.calls
        expect(readCalls).toHaveLength(1)
        const writeCalls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(writeCalls).toHaveLength(0)
    })

    it('creates default user if not provided in config and not saved on disk', () => {
        (BugsnagFileIO.readTextFile as jest.Mock).mockReturnValue(
            { error: true, message: "could not read", code: 404 }
        )

        const userStore = createUserStore('/tmp/user', true)

        const acceptedUser = userStore.load(undefined, 'ab0c1482-2ffe-11eb-adc1-0242ac120002')
        expect(acceptedUser).toEqual({ id: 'ab0c1482-2ffe-11eb-adc1-0242ac120002', name: undefined, email: undefined })
        const readCalls = (BugsnagFileIO.readTextFile as jest.Mock).mock.calls
        expect(readCalls).toHaveLength(1)
        const writeCalls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(writeCalls).toHaveLength(1)
    })
})