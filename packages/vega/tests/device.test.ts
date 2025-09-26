import nativeDevice from '../lib/device_info_native'
import { Client } from '@bugsnag/core'
import { BugsnagFileIO, BugsnagVegaNative } from "@bugsnag/vega-native"
import createDeviceStore from '../lib/device_store'

// @ts-expect-error cannot find global
global.HermesInternal = {}

interface Device {
  id: string,
  manufacturer: string,
  model: string,
  osName: string,
  osVersion: string,
  runtimeVersions: {
    reactNative: string,
    reactNativeJsEngine: string
  }
}

interface DeviceWithState extends Device {
  time: Date
}

interface EventWithDevice {
  device: DeviceWithState
}

interface SessionWithDevice {
  device: Device
}

interface EventPayload {
  events: EventWithDevice[]
}

(BugsnagFileIO.readTextFile as jest.Mock).mockReturnValue(
  { content: `{ "id": "ab0c1482-2ffe-11eb-adc1-0242ac120002" }` }
)

describe('device plugin', () => {
  it('should add an onError callback which captures device information', () => {
    // @ts-expect-error client constructor is protected
    const client = new Client({ apiKey: 'API_KEY_YEAH', persistenceDirectory: '/tmp/user' }, undefined, [])

    const deviceStore = createDeviceStore(client._config.persistenceDirectory)
    const { id: deviceId } = deviceStore.load()

    nativeDevice.register(client, deviceId)
    expect(client._cbs.e).toHaveLength(1)

    const payloads: EventPayload[] = []
    client._setDelivery(() => ({ sendEvent: (payload: EventPayload) => payloads.push(payload), sendSession: () => {} }))
    client.notify(new Error('noooo'))

    expect(payloads.length).toEqual(1)
    expect(payloads[0].events[0].device).toBeDefined()
    expect(payloads[0].events[0].device.id).toBe('ab0c1482-2ffe-11eb-adc1-0242ac120002')
    expect(payloads[0].events[0].device.manufacturer).toBe('Amazon')
    expect(payloads[0].events[0].device.model).toBe('Tv Simulator')
    expect(payloads[0].events[0].device.osName).toBe('Kepler')
    expect(payloads[0].events[0].device.osVersion).toBe('1.1')
    expect(payloads[0].events[0].device.runtimeVersions).toBeDefined()
    expect(payloads[0].events[0].device.runtimeVersions.reactNative).toBe('0.72.0')
    expect(payloads[0].events[0].device.runtimeVersions.reactNativeJsEngine).toBe('hermes')
    expect(payloads[0].events[0].device.time instanceof Date).toBe(true)
  })

  it('should add an onSession callback which captures device information', () => {
    // @ts-expect-error client constructor is protected
    const client = new Client({ apiKey: 'API_KEY_YEAH', persistenceDirectory: '/tmp/user' }, undefined, [])

    const deviceStore = createDeviceStore(client._config.persistenceDirectory)
    const { id: deviceId } = deviceStore.load()

    nativeDevice.register(client, deviceId)
    expect(client._cbs.s).toHaveLength(1)

    const payloads: SessionWithDevice[] = []
    client._sessionDelegate = {
      startSession: (client: any, session: SessionWithDevice) => {
        client._delivery.sendSession(session, () => {})

        return client
      }
    }

    client._setDelivery(() => ({ sendEvent: () => {}, sendSession: (payload: SessionWithDevice) => payloads.push(payload) }))
    client.startSession()

    expect(payloads.length).toEqual(1)
    expect(payloads[0].device).toBeDefined()
    expect(payloads[0].device.id).toBe('ab0c1482-2ffe-11eb-adc1-0242ac120002')
    expect(payloads[0].device.manufacturer).toBe('Amazon')
    expect(payloads[0].device.model).toBe('Tv Simulator')
    expect(payloads[0].device.osName).toBe('Kepler')
    expect(payloads[0].device.osVersion).toBe('1.1')
    expect(payloads[0].device.runtimeVersions).toBeDefined()
    expect(payloads[0].device.runtimeVersions.reactNative).toBe('0.72.0')
    expect(payloads[0].device.runtimeVersions.reactNativeJsEngine).toBe('hermes')
  })

  it('syncs device info with native layer', () => {
    (BugsnagVegaNative.setDevice as jest.Mock).mockClear()
    const setDevice = BugsnagVegaNative.setDevice as jest.Mock
    expect(setDevice).not.toHaveBeenCalled()
    // @ts-expect-error client constructor is protected
    const client = new Client({ apiKey: 'API_KEY_YEAH', persistenceDirectory: '/tmp/user' }, undefined, [])

    const deviceStore = createDeviceStore(client._config.persistenceDirectory)
    const { id: deviceId } = deviceStore.load()

    nativeDevice.register(client, deviceId)
    expect(setDevice).toHaveBeenCalledTimes(1)
    expect(setDevice).toHaveBeenCalledWith({
      id: 'ab0c1482-2ffe-11eb-adc1-0242ac120002',
      manufacturer: 'Amazon',
      model: 'Tv Simulator',
      osName: 'Kepler',
      osVersion: '1.1',
      runtimeVersions: {
        reactNative: '0.72.0',
        reactNativeJsEngine: 'hermes'
      }
    })
  })
})
