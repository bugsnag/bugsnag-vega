import Bugsnag, { KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    persistUser: true,
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.setUser('userID', 'abcd', 'Test User')
        Bugsnag.notify(new Error('SetUserClientError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>SetUserClientScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


