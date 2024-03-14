import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    releaseStage: 'test',
    enabledReleaseStages: ['production', 'test']
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.notify(new Error('ReleaseStageEnabledScenario'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>ReleaseStageEnabledScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


