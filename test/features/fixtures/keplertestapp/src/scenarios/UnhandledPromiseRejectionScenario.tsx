import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { StyleSheet, Text, View } from "react-native"

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

const getStyles = () =>
    StyleSheet.create({
        background: {
            color: 'white',
            flex: 1,
            flexDirection: 'column',
        },
        headerContainer: {
            marginLeft: 200,
        },
        headerText: {
            color: 'white',
            fontSize: 80,
            marginBottom: 10,
        },
    });
