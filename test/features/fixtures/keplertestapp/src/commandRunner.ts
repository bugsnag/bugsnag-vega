import { Command } from "../types"
import { runScenario } from "./commands/runScenario"
import { getCommand } from "./getCommand"
import { AsyncStorage } from "@amzn/react-native-kepler"

const lastCommandKey: string = "lastCommandUUID"
var lastCommandUuid: string | null

async function setLastCommandUUID(uuid: string) {
    try {
        await AsyncStorage.setItem(lastCommandKey, uuid)
    } catch (error) {
        throw new Error('[Bugsnag] Could not save last command uuid in storage')
    }
}

async function getLastCommandUUID(){
    try {
        const value = await AsyncStorage.getItem(lastCommandKey)
        return value
    } catch (error) {
        throw new Error('[Bugsnag] Could not read last command uuid from storage')
    }
}

async function clearLastCommandUUID() {
    try {
        await AsyncStorage.removeItem(lastCommandKey)
    } catch (error) {
        throw new Error('[Bugsnag] Could not remove last command uuid from storage')
    }
}

export async function commandRunner (rootTag: number) {
    let command: Command

    // TODO run in a loop?

    // if environment variables specify a scenario to launch, use that
    if (false) {

    } else {
        command = await getCommand()
    }

    lastCommandUuid = command.uuid
    await setLastCommandUUID(lastCommandUuid)

    if (command.action === "noop") {
        lastCommandUuid = null
        clearLastCommandUUID()
        console.log("[Bugsnag] noop - clearing command uuid")
        // immediately loop around
        //continue
    }

    switch (command.action) {
        case 'run-scenario':
            runScenario(
                rootTag,
                command.scenario_name,
                command.api_key,
                command.notify_endpoint,
                command.sessions_endpoint
            )
            break;
        default:
            // do nothing
    }
}
