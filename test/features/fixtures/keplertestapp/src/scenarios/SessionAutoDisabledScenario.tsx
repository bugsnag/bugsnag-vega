import { type KeplerConfig } from '@bugsnag/kepler'
import React from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    autoTrackSessions: false
}

const App = () => {
    const styles = getStyles()

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>SessionAutoDisabledScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


