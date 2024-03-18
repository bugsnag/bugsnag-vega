import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.notify(new Error('HandledOverrideJsErrorScenario'), event => {
          event.unhandled = true
        })
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>HandledOverrideJsErrorScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
