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
        Bugsnag.notify(new Error('NoUserError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NoUserScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


