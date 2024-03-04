import { runScenario } from "./commands/runScenario"
import { getCommand } from "./getCommand"
import delay from "./utils/delay"
import { clearLastCommandUUID, getLastCommandUUID, setLastCommandUUID } from "./utils/storage"

const COMMAND_INTERVAL = 500

let lastCommandUuid: string | null

export async function commandRunner (rootTag: number) {
    const command = await getCommand()

    // Populate last command uuid on first run
    if (lastCommandUuid === null) {
        lastCommandUuid = await getLastCommandUUID()
    }

    if (command.uuid === lastCommandUuid) {
        // Bail out if we're trying to run the same command again
        delay(COMMAND_INTERVAL)
        commandRunner(rootTag)
    }

    lastCommandUuid = command.uuid
    await setLastCommandUUID(lastCommandUuid)

    switch (command.action) {
        case 'noop':
            lastCommandUuid = null
            clearLastCommandUUID()
            console.log("[Bugsnag] noop - clearing command uuid")
            // immediately loop around
            commandRunner(rootTag)
            break
        case 'run-scenario':
            runScenario(
                rootTag,
                command.scenario_name,
                command.api_key,
                command.notify_endpoint,
                command.sessions_endpoint
            )
            break
        default:
            console.log('[Bugsnag] No command received')
    }

    // loop commandRunner
    delay(COMMAND_INTERVAL)
    commandRunner(rootTag)
}
