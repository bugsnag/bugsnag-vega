import Bugsnag from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { BreadcrumbType } from '@bugsnag/core/types'
import { getStyles } from '../utils/defaultStyle'
import { mazeRunnerBaseAddress } from '../utils/config'


const config = {
    enabledBreadcrumbTypes: ['request'] as BreadcrumbType[],
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
      (async () => {
            try {
                const response = await fetch(`http://${mazeRunnerBaseAddress}/reflect`)
                if (response.ok) {
                    Bugsnag.notify(new Error("NetworkBreadcrumbError"))
                }
            } catch (e) {
                Bugsnag.notify(new Error(JSON.stringify(e)))
            }
        })();
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>NetworkBreadcrumbScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }
