import { Plugin, Client } from '@bugsnag/core'
import { NavigationContainer } from "@amzn/react-navigation__native"

declare class BugsnagPluginReactNavigation implements Plugin {
  constructor()
  load(client: Client): BugsnagPluginReactNavigationResult
}

export interface BugsnagPluginReactNavigationResult {
  createNavigationContainer(NavigationContainerComponent: typeof NavigationContainer): typeof NavigationContainer
}

// add a new call signature for the getPlugin() method that types the react plugin result
declare module '@bugsnag/core' {
  interface Client {
    getPlugin(id: 'reactNavigation'): BugsnagPluginReactNavigationResult | undefined
  }
}

export default BugsnagPluginReactNavigation
