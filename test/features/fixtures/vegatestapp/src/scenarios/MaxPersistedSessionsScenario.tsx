import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'

const config: Partial<VegaConfig> = {
    autoTrackSessions: false,
    maxPersistedSessions: 3,
}

const App = () => {
    const styles = getStyles()
    let counter = 0

    useEffect(() => {
        (async () => {
            Bugsnag.addOnSession((session) => {
                session.id = `MaxPersistedSessions${counter.toString()}`
                counter++
            })
            for (let i = 0; i < 10; i++) {
                Bugsnag.startSession();
                await delay(100)
                Bugsnag.notify(new Error(`MaxPersistedSessionsError${i}`))
                await delay(100)
            }
        })()
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>MaxPersistedSessionsScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


