import { BugsnagNativeUT } from '@bugsnag/kepler-native-ut'
import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        const result = BugsnagNativeUT.runUnitTests()
        console.log('[bugsnag] NativeUnitTests result:', result)
        Bugsnag.notify(new Error(JSON.stringify(result)))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NativeUnitTests</Text>
            </View>
        </View>
    )
}

export default { App, config }


