import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    redactedKeys: ['key'],
    metadata: {
        config: {
            key: 'value'
        }
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addMetadata('add_metadata', 'key', 'value')
        Bugsnag.addMetadata('metadata_section', {'key': 'value'})
        Bugsnag.notify(new Error('AddMetadata'), (event) => {
            event.addMetadata('event_add_metadata', 'key', 'value')
        })
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>RedactMetadataScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
