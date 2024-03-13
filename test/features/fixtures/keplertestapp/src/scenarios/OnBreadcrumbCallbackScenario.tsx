import Bugsnag, { type KeplerConfig } from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<KeplerConfig> = {
    enabledBreadcrumbTypes: [],
    onBreadcrumb: (breadcrumb) => {
        if (breadcrumb.message === 'config discard') return false
        breadcrumb.metadata.config = 'works'
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addOnBreadcrumb((breadcrumb) => {
            if (breadcrumb.message === 'addOnBreadcrumb discard') return false
            breadcrumb.metadata.addOnBreadcrumb = 'works'
        })

        Bugsnag.leaveBreadcrumb('config discard')
        Bugsnag.leaveBreadcrumb('addOnBreadcrumb discard')

        Bugsnag.leaveBreadcrumb('manualBreadcrumb')
        Bugsnag.notify(new Error('ManualBreadcrumbError'))
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>OnBreadcrumbCallbackScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


