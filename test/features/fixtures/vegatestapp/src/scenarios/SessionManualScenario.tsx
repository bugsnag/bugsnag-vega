import Bugsnag, { type VegaConfig } from '@bugsnag/vega'
import React, { useEffect } from 'react'
import { Text, View } from "react-native"
import { getStyles } from '../utils/defaultStyle'
import delay from '../utils/delay'

const config: Partial<VegaConfig> = {
    autoTrackSessions: false
}

const App = () => {
    const styles = getStyles()

    useEffect(() => {
        Bugsnag.startSession();

        (async () => {
            await delay(500)
        
            Bugsnag.notify(new Error('SessionManualScenarioA'), () => {
            }, async () => {
              await delay(750)
              Bugsnag.pauseSession()
              await delay(750)
              Bugsnag.notify(new Error('SessionManualScenarioB'), () => {
              }, async () => {
                await delay(2500)
                Bugsnag.resumeSession()
                await delay(750)
                Bugsnag.notify(new Error('SessionManualScenarioC'), () => {
                }, async () => {
                  await delay(750)
                  Bugsnag.pauseSession()
                  await delay(750)
                  Bugsnag.startSession()
                  await delay(750)
                  Bugsnag.notify(new Error('SessionManualScenarioD'))
                })
              })
            })
        })()
    },[])

    return (
        <View style={styles.background}>
            <View style={styles.headerContainer}>
                <Text style={styles.headerText}>SessionManualScenario</Text>
            </View>
        </View>
    )
}

export default { App, config }


