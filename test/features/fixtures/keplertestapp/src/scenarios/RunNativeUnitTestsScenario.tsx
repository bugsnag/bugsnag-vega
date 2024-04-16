import { BugsnagNativeUT } from '@bugsnag/kepler-native-ut'
import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        const utOutputFile = (config.persistenceDirectory ? config.persistenceDirectory : '/data/bugsnag') + '/utOutput.txt'
        BugsnagNativeUT.configure(utOutputFile)

        const result = BugsnagNativeUT.runUnitTests()
        console.log('[Bugsnag] NativeUnitTests result:', result)
        Bugsnag.notify(new Error(JSON.stringify(result)))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NativeUnitTests</Text>
            </View>
        </View>
    )
}

export default { App, config }


