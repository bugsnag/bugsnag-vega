import { BugsnagNativeUT } from '@bugsnag/vega-native-ut'
import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {}

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


