import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config: Partial<VegaConfig> = {
    onError: (event) => {
        if (event.errors[0].errorMessage === 'config ignore') return false
    }
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.addOnError((event) => {
            if (event.errors[0].errorMessage === 'addOnError ignore') return false
        })

        Bugsnag.notify(new Error('config ignore'), () => {}, () => {
            setTimeout(() => {
                Bugsnag.notify(new Error('addOnError ignore'), () => {}, () => {
                    setTimeout(() => {
                        Bugsnag.notify(new Error('notify ignore'), () => {
                            return false
                        }, () => {
                            setTimeout(() => {
                                Bugsnag.notify(new Error('send'))
                            }, 250);                  
                        })
                    }, 250);
                })
            }, 250);
        })
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>OnErrorPreventSendScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


