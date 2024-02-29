import { Command } from "../types"
import { getCommand } from "./getCommand"
import { runScenario } from "./runScenario"

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
                command.endpoint
            )
            break;
        default:
            // do nothing
    }
}
