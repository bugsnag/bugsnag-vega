//@ts-ignore
import type {KeplerTurboModule} from '@amzn/keplerscript-turbomodule-api';
//@ts-ignore
import {TurboModuleRegistry} from '@amzn/keplerscript-turbomodule-api';

export interface BugsnagKeplerNative extends KeplerTurboModule {
  // Exported methods.
  getConstants: () => {
    const1: boolean;
    const2: number;
    const3: string;
  };
  getMajorVersion: () => number;
  getMinorVersion: () => number;
  getPatchVersion: () => number;
  voidFunc: () => void;
  getBool: (arg: boolean) => boolean;
  getNumber: (arg: number) => number;
  getString: (arg: string) => string;
  getArray: (arg: Array<any>) => Array<any>;
  getArrayBuffer: (arg: ArrayBuffer) => ArrayBuffer;
  getObject: (arg: Object) => Object;
  getValue: (x: number, y: string, z: Object) => Object;
  getValueWithCallback: (callback: (value: string) => void) => void;
  getValueWithPromise: (error: boolean) => Promise<string>;
}

// prettier-ignore
export default TurboModuleRegistry.getEnforcing<BugsnagKeplerNative>(
  'BugsnagKeplerNative',
) as BugsnagKeplerNative;
