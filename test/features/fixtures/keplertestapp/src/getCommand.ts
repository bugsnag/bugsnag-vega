import type { Command } from "../types"

export async function getCommand (): Promise<Command> {
    // poll the server for the next command to run

    return {
        action: 'run-scenario',
        scenario_name: 'TestScenario1',
        api_key: '1234',
        endpoint: '/test'
    }
}
