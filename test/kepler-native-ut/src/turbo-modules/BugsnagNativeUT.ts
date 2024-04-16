//@ts-ignore
import type { KeplerTurboModule } from '@amzn/keplerscript-turbomodule-api';
//@ts-ignore
import { TurboModuleRegistry } from '@amzn/keplerscript-turbomodule-api';

export interface BugsnagNativeUT extends KeplerTurboModule {
  configure: (path: string) => void;
  runUnitTests: () => number;
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagNativeUT>(
  'BugsnagNativeUT',
) as BugsnagNativeUT;
