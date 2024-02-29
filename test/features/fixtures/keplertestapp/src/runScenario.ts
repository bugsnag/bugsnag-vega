import Busgnag from '@bugsnag/kepler'
import { AppRegistry } from 'react-native'
import * as Scenarios from './scenarios'

type ScenarioName = keyof typeof Scenarios

// @ts-expect-error
const isTurboModuleEnabled = () => global.__turboModuleProxy != null

export async function runScenario (rootTag: number, scenarioName: string, apiKey: string, endpoint: string) {
    console.error(`[Busgnag] Launching scenario: ${scenarioName}`)
    const scenario = Scenarios[scenarioName as ScenarioName]

    const config = {
        apiKey,
        endpoints: { notify: endpoint, sessions: endpoint },
        ...scenario.config
    }
  
    Busgnag.start(config)
  
    const appParams = { rootTag }
    // if (isTurboModuleEnabled()) {
    //     appParams.fabric = true
    //     appParams.initialProps = { concurrentRoot: true }
    // }

    AppRegistry.registerComponent(scenarioName, () => scenario.App)
    AppRegistry.runApplication(scenarioName, appParams)
  }
