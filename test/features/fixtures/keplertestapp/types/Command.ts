export interface Command {
    action: string
    scenario_name: string
    api_key: string
    endpoints: {
        notify: string
        sessions: string
    }
}
