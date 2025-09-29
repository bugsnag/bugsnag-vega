import type { KeplerTurboModule } from '@amazon-devices/keplerscript-turbomodule-api';
import { TurboModuleRegistry } from '@amazon-devices/keplerscript-turbomodule-api';

export interface BugsnagNativeUT extends KeplerTurboModule {
  configure: (path: string) => void;
  runUnitTests: () => number;
  readOnlyMemoryCrash: () => void;
  nullptrCrash: () => void;
  manualAbortCrash: () => void;
  throwExceptionCrash: () => void;
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagNativeUT>(
  'BugsnagNativeUT',
) as BugsnagNativeUT;
