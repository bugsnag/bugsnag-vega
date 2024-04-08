import { BugsnagFileIO, BugsnagKeplerNative } from "@bugsnag/kepler-native"
import createDeviceStore from "../lib/device_store"

describe('kepler device store', () => {
    afterEach(() => {
        jest.clearAllMocks()
    })

    it('loads device id from file if it exists', () => {
        (BugsnagFileIO.readTextFile as jest.Mock).mockReturnValue(
            { content: `{ "id": "ab0c1482-2ffe-11eb-adc1-0242ac120002" }` }
        )

        const deviceStore = createDeviceStore('/tmp/user')
        const deviceInfo = deviceStore.load()
        expect(deviceInfo.id).toEqual("ab0c1482-2ffe-11eb-adc1-0242ac120002")

        const readCalls = (BugsnagFileIO.readTextFile as jest.Mock).mock.calls
        expect(readCalls).toHaveLength(1)
        const writeCalls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(writeCalls).toHaveLength(0)
        const getCalls = (BugsnagKeplerNative.getDeviceID as jest.Mock).mock.calls
        expect(getCalls).toHaveLength(0)
        const setCalls = (BugsnagKeplerNative.setDeviceID as jest.Mock).mock.calls
        expect(setCalls).toHaveLength(1)
    })

    it('generates and saves device id if file did not exist', () => {
        (BugsnagFileIO.readTextFile as jest.Mock).mockReturnValue({ error: true, message: "could not read", code: 404 });
        (BugsnagKeplerNative.getDeviceID as jest.Mock).mockReturnValue("ab0c1482-2ffe-11eb-adc1-0242ac120002");

        const deviceStore = createDeviceStore('/tmp/user')
        const deviceInfo = deviceStore.load()
        expect(deviceInfo.id).toEqual("ab0c1482-2ffe-11eb-adc1-0242ac120002")

        const readCalls = (BugsnagFileIO.readTextFile as jest.Mock).mock.calls
        expect(readCalls).toHaveLength(1)
        const writeCalls = (BugsnagFileIO.writeTextFile as jest.Mock).mock.calls
        expect(writeCalls).toHaveLength(1)
        const getCalls = (BugsnagKeplerNative.getDeviceID as jest.Mock).mock.calls
        expect(getCalls).toHaveLength(1)
        const setCalls = (BugsnagKeplerNative.setDeviceID as jest.Mock).mock.calls
        expect(setCalls).toHaveLength(0)
    })
})