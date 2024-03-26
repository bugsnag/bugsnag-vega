import plugin from '../lib/device'
import { Client } from '@bugsnag/core'

// @ts-expect-error cannot find global
global.HermesInternal = {}

interface Device {
  osName: string,
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

describe('device plugin', () => {
  it('should add an onError callback which captures device information', () => {
    // @ts-expect-error client constructor is protected
    const client = new Client({ apiKey: 'API_KEY_YEAH' }, undefined, [plugin])
    const payloads: EventPayload[] = []

    expect(client._cbs.e).toHaveLength(1)

    // 
    client._setDelivery(() => ({ sendEvent: (payload: EventPayload) => payloads.push(payload), sendSession: () => {} }))
    client.notify(new Error('noooo'))

    expect(payloads.length).toEqual(1)
    expect(payloads[0].events[0].device).toBeDefined()
    expect(payloads[0].events[0].device.time instanceof Date).toBe(true)
    expect(payloads[0].events[0].device.runtimeVersions).toBeDefined()
    expect(payloads[0].events[0].device.runtimeVersions.reactNative).toBe('0.72.0')
    expect(payloads[0].events[0].device.runtimeVersions.reactNativeJsEngine).toBe('hermes')
    expect(payloads[0].events[0].device.osName).toBe('kepler')
  })

  it('should add an onSession callback which captures device information', () => {
    // @ts-expect-error client constructor is protected
    const client = new Client({ apiKey: 'API_KEY_YEAH' }, undefined, [plugin])
    const payloads: SessionWithDevice[] = []
    client._sessionDelegate = {
      startSession: (client: any, session: SessionWithDevice) => {
        client._delivery.sendSession(session, () => {})

        return client
      }
    }

    expect(client._cbs.s).toHaveLength(1)

    client._setDelivery(() => ({ sendEvent: () => {}, sendSession: (payload: SessionWithDevice) => payloads.push(payload) }))
    client.startSession()

    expect(payloads.length).toEqual(1)
    expect(payloads[0].device).toBeDefined()
    expect(payloads[0].device.runtimeVersions).toBeDefined()
    expect(payloads[0].device.runtimeVersions.reactNative).toBe('0.72.0')
    expect(payloads[0].device.runtimeVersions.reactNativeJsEngine).toBe('hermes')
    expect(payloads[0].device.osName).toBe('kepler')
  })
})