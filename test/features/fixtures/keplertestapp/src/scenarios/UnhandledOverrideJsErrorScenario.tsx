import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import { type KeplerConfig } from '@bugsnag/kepler'

const config: Partial<KeplerConfig> = {
  onError: (event) => {
    event.unhandled = false
  }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Promise.reject(new Error('UnhandledOverrideJsErrorScenario'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>UnhandledOverrideJsErrorScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }

