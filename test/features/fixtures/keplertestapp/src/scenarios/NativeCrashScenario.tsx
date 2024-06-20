import { BugsnagNativeUT } from '@bugsnag/kepler-native-ut'
import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'

const config: Partial<KeplerConfig> = {
    apiKey: '12345678901234567890123456789012'
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        (async () => {
            const utOutputFile = (config.persistenceDirectory ? config.persistenceDirectory : '/data/bugsnag') + '/utOutput.txt'
            BugsnagNativeUT.configure(utOutputFile)
            await delay(2000)
            BugsnagNativeUT.readOnlyMemoryCrash()
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NativeCrashScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


