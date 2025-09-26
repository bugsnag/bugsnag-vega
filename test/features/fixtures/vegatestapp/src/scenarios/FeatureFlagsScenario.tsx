import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
    onError: (event) => {
        event.addFeatureFlag('sample_group', 'a')
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addFeatureFlag('demo_mode')

        Bugsnag.addFeatureFlags([
        { name: 'should_not_be_reported_1' },
        { name: 'should_not_be_reported_2' },
        { name: 'should_not_be_reported_3' }
        ])

        Bugsnag.clearFeatureFlag('should_not_be_reported_3')
        Bugsnag.clearFeatureFlag('should_not_be_reported_2')
        Bugsnag.clearFeatureFlag('should_not_be_reported_1')

        Bugsnag.notify(new Error('FeatureFlagScenario'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>FeatureFlagsScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
