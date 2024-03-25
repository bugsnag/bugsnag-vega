import Bugsnag, { KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'

const config: Partial<KeplerConfig> = {
    enabledBreadcrumbTypes: [],
    maxPersistedEvents: 3,
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        (async () => {
            for (let i = 0; i < 10; i++) {
                let message = `MaxPersistedEventsError${i}`
                await delay(100)
                Bugsnag.notify(new Error(message))
            }
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>MaxPersistedEventsScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


