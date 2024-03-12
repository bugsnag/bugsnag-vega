import type { Command } from "../types"
import delay from './utils/delay'

const INTERVAL = 500

function getErrorMessage(error: unknown) {
    if (error instanceof Error) return error.message
    return String(error)
}

export async function getCommand (mazeAddress = '10.0.2.2:9339'): Promise<Command> {
    // poll the server for the next command to run
    const mazeUrl = `http://${mazeAddress}/command`

    console.log('[Bugsnag] getCommand entered!')

    while (true) {
        try {
            console.log('[Bugsnag] Fetching command from maze')
            const response = await fetch(mazeUrl)
            const command: Command = await response.json()
            if (response.ok) {
                if (command?.action !== 'noop') {
                    return command!
                }
            } else {
                console.log('[Bugsnag] Error while receiving command')
            }
        } catch (err) {
            console.log(`[Bugsnag] Error fetching command from maze runner: ${getErrorMessage(err)}`)
        }
      await delay(INTERVAL)
    }
}
