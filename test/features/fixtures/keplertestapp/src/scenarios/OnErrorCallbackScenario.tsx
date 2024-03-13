import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    onError: (event) => {
        event.addMetadata('on_error', 'global', 'works')
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addOnError((event) => {
            event.addMetadata('on_error', 'addOnError', 'works')
        })

        Bugsnag.notify(new Error('OnErrorCallback'), (event) => {
            event.addMetadata('on_error', 'notify_opts', 'works')
        })
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>OnErrorCallbackScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


