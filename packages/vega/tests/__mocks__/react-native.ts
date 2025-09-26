type ReactNativeVersion = {
  major: number,
  minor: number,
  patch: number,
  prerelease?: number | null | undefined
}

type PlatformConstants = { reactNativeVersion: ReactNativeVersion }

export const Platform = new class {
  get OS (): 'kepler' {
    return 'kepler'
  }

  get constants (): PlatformConstants {
    return {
      reactNativeVersion: {
        major: 0,
        minor: 72,
        patch: 0
      }
    }
  }
}()
