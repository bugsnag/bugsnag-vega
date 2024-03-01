import Busgnag from '@bugsnag/kepler'
import { AppRegistry } from 'react-native'
import * as Scenarios from '../scenarios'

type ScenarioName = keyof typeof Scenarios

// @ts-expect-error globalThis has no index signature
const isTurboModuleEnabled = () => global.__turboModuleProxy != null

export async function runScenario (rootTag: number, scenarioName: string, apiKey: string, notify: string, sessions: string) {
    console.log(`[Bugsnag] Launching scenario: ${scenarioName}`)

    const scenario = Scenarios[scenarioName as ScenarioName]

    const config = {
        apiKey,
        endpoints: { notify, sessions },
        ...scenario.config
    }
  
    console.log(`[Bugsnag] Calling Bugsnag.start with config: ${JSON.stringify(config)}`)
    Busgnag.start(config)
    
    const appParams = { rootTag }
    // if (isTurboModuleEnabled()) {
    //     appParams.fabric = true
    //     appParams.initialProps = { concurrentRoot: true }
    // }
        
    console.log(`[Bugsnag] Mounting App`)
    try {
        // AppRegistry.unmountApplicationComponentAtRootTag(rootTag)
        AppRegistry.registerComponent(scenarioName, () => scenario.App)
        AppRegistry.runApplication(scenarioName, appParams)
    } catch (err) {
        console.error(`[Bugsnag] Failed to mount app`)
        console.error(err)
    }
  }
