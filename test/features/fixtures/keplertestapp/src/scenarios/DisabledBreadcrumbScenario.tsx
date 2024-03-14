import Bugsnag from '@bugsnag/kepler'
import { type KeplerConfig }  from '@bugsnag/kepler'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'


const config: Partial<KeplerConfig> = {
    enabledBreadcrumbTypes: [],
}

function consoleCrumb() {
    console.log('my console log')
    Bugsnag.notify(new Error('ConsoleBreadcrumbDisabledError'))
}

function manualCrumb() {
    Bugsnag.leaveBreadcrumb('manualBreadcrumb', { key: "myval" })
    Bugsnag.notify(new Error('ManualBreadcrumbDisabledError'))
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        consoleCrumb();

        setTimeout(() => {
            manualCrumb();
          }, 500);

        setTimeout(() => {
            (async () => {
                try {
                    const response = await fetch("https://postman-echo.com/get")
                    if (response.ok) {
                        Bugsnag.notify(new Error("NetworkBreadcrumbDisabledError"))
                    }
                } catch (e) {
                    Bugsnag.notify(new Error(JSON.stringify(e)))
                }
            })();
          }, 1000);
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>DisabledBreadcrumbScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
