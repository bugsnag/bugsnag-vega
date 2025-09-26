import Bugsnag from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'

const config = {
    context: 'context-config'
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.notify(new Error('ContextJsCustomScenarioA'), () => {
        }, () => {
          setTimeout(() => {
            Bugsnag.setContext('context-client')
            Bugsnag.notify(new Error('ContextJsCustomScenarioB'), () => {
              setTimeout(() => {
                Bugsnag.notify(new Error('ContextJsCustomScenarioC'), event => {
                  event.context = 'context-onerror'
                })
              }, 500)
            })
          }, 500)
        })
    }, [])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>ContextJsCustomScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


