/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates.  All rights reserved.
 *
 * PROPRIETARY/CONFIDENTIAL.  USE IS SUBJECT TO LICENSE TERMS.
 */

import { AppRegistry, LogBox } from 'react-native';
import { App } from './src/App';
import { name as appName } from './app.json';
import Bugsnag from '@bugsnag/kepler'

Bugsnag.start({
  apiKey: 'abcdef1234567890abcdef1234567890',
  endpoints: {
    notify: '10.0.2.2:9339/notify',
    sessions: '10.0.2.2:9339/sessions'
  }
})

Bugsnag.notify(new Error('Oops!'))

// Temporary workaround for problem with nested text
// not working currently.
LogBox.ignoreAllLogs();

AppRegistry.registerComponent(appName, () => App);
