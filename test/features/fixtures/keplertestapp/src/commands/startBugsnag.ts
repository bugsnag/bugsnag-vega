import Busgnag from '@bugsnag/kepler'
import * as Scenarios from '../scenarios'

type ScenarioName = keyof typeof Scenarios

export async function startBugsnag(scenarioName: string, apiKey: string, notify: string, sessions: string) {
  console.log(`[Bugsnag] Starting Bugsnag for scenario: ${scenarioName}`)

  const scenario = Scenarios[scenarioName as ScenarioName]
  const config = {
    apiKey,
    endpoints: {notify, sessions},
    ...scenario.config
  }

  console.log(`[Bugsnag] Calling Bugsnag.start with config: ${JSON.stringify(config)}`)
  Busgnag.start(config)
}
