import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
    autoTrackSessions: false,
    onSession: (session) => {
        if (session.app) session.app.releaseStage = 'test'
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addOnSession((session) => {
            session.setUser('123', 'test@bugsnag.com', 'Test User')
        })

        Bugsnag.startSession()
    },[])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>OnSessionCallbackScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


