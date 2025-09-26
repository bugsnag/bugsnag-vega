import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Promise.reject(new Error('UnhandledPromiseRejectionScenario'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>UnhandledPromiseRejectionScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }

