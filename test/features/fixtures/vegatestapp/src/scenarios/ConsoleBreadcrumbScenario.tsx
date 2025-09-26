import Bugsnag, { type VegaConfig }  from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'


const config: Partial<VegaConfig> = {
    enabledBreadcrumbTypes: ['error', 'log', 'manual'],
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        console.log('my console log')
        Bugsnag.notify(new Error('ConsoleBreadcrumbError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>ConsoleBreadcrumbScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
