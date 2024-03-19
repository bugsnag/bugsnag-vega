import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'

const config: Partial<KeplerConfig> = {
  autoDetectErrors: false
}

const App = () => {
  const styles = getStyles()

  useEffect(() => {
      Bugsnag.notify(new Error('manual notify does work'))

      Promise.reject(new Error('UnhandledPromiseRejectionsDisabledScenario'))
  }, [])

  return (
      <View style={styles.background}>
          <View style={styles.headerContainer}>
              <Text style={styles.headerText}>UnhandledPromiseRejectionsDisabledScenario</Text>
          </View>
      </View>
  )
}

export default { App, config }

