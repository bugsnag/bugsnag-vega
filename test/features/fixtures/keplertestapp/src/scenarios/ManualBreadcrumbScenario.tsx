import Bugsnag from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import { BreadcrumbType } from '@bugsnag/core/types'

const config = {
    enabledBreadcrumbTypes: ['error','manual'] as BreadcrumbType[],
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.leaveBreadcrumb('manualBreadcrumb', { key: "myval" })
        Bugsnag.notify(new Error('ManualBreadcrumbError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>ManualBreadcrumbScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


