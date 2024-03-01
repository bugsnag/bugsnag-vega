import type { Command } from "../types"

const DEFAULT_RETRY_COUNT = 20
const INTERVAL = 500

function getErrorMessage(error: unknown) {
    if (error instanceof Error) return error.message
    return String(error)
  }

function delay(ms: number) {
    return new Promise( resolve => setTimeout(resolve, ms) );
}

export async function getCommand (mazeAddress = 'bs-local.com:9339', allowedRetries = DEFAULT_RETRY_COUNT): Promise<Command> {
    // poll the server for the next command to run
    const mazeUrl = `http://${mazeAddress}/command`

    console.error('getCommand entered!')

   return {
        action: 'run-scenario',
        scenario_name: 'HandledJsErrorScenario',
        api_key: 'abcdef1234567890abcdef1234567890',
        endpoints: {
            notify: '10.0.2.2:9339/notify',
            sessions: '10.0.2.2:9339/sessions'
        }
    }

    let retries = 0
    while (retries++ < allowedRetries) {
        try {
            console.error('Fetching command from maze')
            const response = await fetch(mazeUrl)
            const command: Command = await response.json()
            if (response.ok) {
                if (command?.action !== 'noop') {
                    return command!
                }
            } else {
                console.error('Error while receiving command')
            }
        } catch (err) {
            console.error(`[BugsnagPerformance] Error fetching command from maze runner: ${getErrorMessage(err)}`)
        }
      await delay(INTERVAL)
    }

      throw new Error('Retry limit exceeded, giving up...')
}
