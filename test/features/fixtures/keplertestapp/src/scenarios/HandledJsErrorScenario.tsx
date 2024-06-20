import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {
    appVersion: '1.2.3',
    appType: 'kepler'
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.notify(new Error('HandledJSError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>HandledJsErrorScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


