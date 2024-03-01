import { Command } from "../types"
import { runScenario } from "./commands/runScenario"
import { getCommand } from "./getCommand"

export async function commandRunner (rootTag: number) {
    let command: Command

    // if environment variables specify a scenario to launch, use that
    if (false) {

    } else {
        command = await getCommand()
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
