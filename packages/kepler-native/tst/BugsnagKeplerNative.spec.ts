import 'react-native';

import {BugsnagKeplerNative} from '../src';

jest.mock('../src/turbo-modules/BugsnagKeplerNative');

describe('Template Turbomodule library tests', () => {
  it('Sample Test Case', () => {
    expect(BugsnagKeplerNative.getMajorVersion()).toBe(1);
  });
});
