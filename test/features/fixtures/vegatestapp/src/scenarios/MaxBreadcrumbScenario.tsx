import Bugsnag, { type VegaConfig }  from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
    enabledBreadcrumbTypes: null,
    maxBreadcrumbs: 5,
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        for (let i = 0; i < 20; i++) {
            Bugsnag.leaveBreadcrumb('manualBreadcrumb', { key: i })
        }
        Bugsnag.notify(new Error('MaxBreadcrumbError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>MaxBreadcrumbScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


