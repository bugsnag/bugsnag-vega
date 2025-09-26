import { BugsnagNativeUT } from '@bugsnag/vega-native-ut'
import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
    apiKey: '12345678901234567890123456789012',
    appVersion: '1.2.3',
    appType: 'kepler',
    releaseStage: 'test',
    user: {
        id: 'userID',
        name: 'Test User',
        email: 'abcd'
    },
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        const utOutputFile = (config.persistenceDirectory ? config.persistenceDirectory : '/data/bugsnag') + '/utOutput.txt'
        BugsnagNativeUT.configure(utOutputFile)

        Bugsnag.addMetadata('add_metadata', 'key', 'value')
        Bugsnag.addMetadata('metadata_section', { 'key': 'value' })
        Bugsnag.addFeatureFlag('myfeature1')

        BugsnagNativeUT.readOnlyMemoryCrash()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NativeCrashFullConfigScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


