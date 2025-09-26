import { BugsnagNativeUT } from '@bugsnag/vega-native-ut'
import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
  apiKey: '12345678901234567890123456789012'
}

const App = () => {
  const styles = getStyles()

  useEffect(() => {
    const utOutputFile = (config.persistenceDirectory ? config.persistenceDirectory : '/data/bugsnag') + '/utOutput.txt'
    BugsnagNativeUT.configure(utOutputFile)
    BugsnagNativeUT.manualAbortCrash()
  }, [])

  return (
    <View style={styles.background}>
      <View style={styles.headerContainer}>
        <Text style={styles.headerText}>NativeCrashAbortScenario</Text>
      </View>
    </View>
  )
}

export default { App, config }


