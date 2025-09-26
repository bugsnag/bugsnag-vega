import nativeApp from '../lib/app_info_native'
import { Client } from '@bugsnag/core'
import { BugsnagVegaNative } from "@bugsnag/vega-native"

interface App {
  bundleId: string
  id: string
  releaseStage: string
  type: string
  version: string
  binaryArch: string
}

interface EventWithApp {
  app: App
}

interface EventPayload {
  events: EventWithApp[]
}

describe('app plugin', () => {
  it('syncs app info with native layer', () => {
    const setApp = BugsnagVegaNative.setApp as jest.Mock
    expect(setApp).not.toHaveBeenCalled()

    const config = {
      apiKey: 'API_KEY_YEAH',
      appType: 'test-app',
      appVersion: '1.2.3',
      releaseStage: 'test'
    }

    // @ts-expect-error client constructor is protected
    const client = new Client(config, undefined, [])
    const nativeStaticAppInfo = { binaryArch: 'amd64' }
    nativeApp.register(client, nativeStaticAppInfo)
    expect(setApp).toHaveBeenCalledTimes(1)
    expect(setApp).toHaveBeenCalledWith({
      id: 'com.bugsnag.fixtures.vegatestapp',
      type: 'test-app',
      version: '1.2.3',
      releaseStage: 'test',
      binaryArch: 'amd64'
    })
  })

  it('sets app data in events', () => {
    const config = {
      apiKey: 'API_KEY_YEAH',
      appType: 'test-app',
      appVersion: '1.2.3',
      releaseStage: 'test'
    }

    // @ts-expect-error client constructor is protected
    const client = new Client(config, undefined, [])
    const nativeStaticAppInfo = { binaryArch: 'amd64' }
    nativeApp.register(client, nativeStaticAppInfo)

    const payloads: EventPayload[] = []
    client._setDelivery(() => ({ sendEvent: (payload: EventPayload) => payloads.push(payload), sendSession: () => {} }))
    client.notify(new Error('noooo'))

    expect(payloads.length).toEqual(1)
    expect(payloads[0].events[0].app).toBeDefined()
    expect(payloads[0].events[0].app.id).toBe('com.bugsnag.fixtures.vegatestapp')
    expect(payloads[0].events[0].app.version).toBe('1.2.3')
    expect(payloads[0].events[0].app.releaseStage).toBe('test')
    expect(payloads[0].events[0].app.type).toBe('test-app')
    expect(payloads[0].events[0].app.binaryArch).toBe('amd64')
  })
})