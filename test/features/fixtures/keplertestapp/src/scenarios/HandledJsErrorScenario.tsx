import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { StyleSheet, Text, View } from "react-native"

export default {
    config: {},
    App: () => {
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
}

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
