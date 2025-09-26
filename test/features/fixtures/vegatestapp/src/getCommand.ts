import type { Command } from "../types"
import delay from './utils/delay'
import { mazeRunnerBaseAddress } from "./utils/config"

const INTERVAL = 500

function getErrorMessage(error: unknown) {
    if (error instanceof Error) return error.message
    return String(error)
}

export async function getCommand (): Promise<Command> {
    // poll the server for the next command to run
    const mazeUrl = `http://${mazeRunnerBaseAddress}/command`

    console.log('[Bugsnag] getCommand entered!')

    while (true) {
        try {
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
