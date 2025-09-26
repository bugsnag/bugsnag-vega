Feature: Reporting handled errors

Scenario: Calling notify() with a caught Error
  When I run "HandledJsErrorScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "type" equals "reactnativejs"
  And the exception "message" equals "HandledJSError"
  And the event "unhandled" is false
  And the event "context" is null

  # App data
  And the event "app.id" equals "com.bugsnag.fixtures.vegatestapp"
  And the event "app.releaseStage" equals "production"
  And the event "app.version" equals "1.2.3"
  And the event "app.type" equals "kepler"
  And the event "app.binaryArch" is not null
  And the event "app.duration" is not null
  
  # Device data
  And the event "device.id" is not null
  And the event "device.id" matches "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
  And the event "device.manufacturer" equals "Amazon"
  And the event "device.model" is not null
  And the event "device.osName" equals "Kepler"
  And the event "device.osVersion" is not null
  And the event "device.runtimeVersions" is not null
  And the event "device.runtimeVersions.reactNative" is not null
  And the event "device.runtimeVersions.reactNativeJsEngine" equals "hermes"
  And the event "device.time" is not null

Scenario: Errors are stored when the network is unreachable
  # make the network unreachable
  # cannot set for just one return code because we are also sending sessions
  Given I set the HTTP status code to 429
  When I run "HandledJsErrorScenario"
  And I wait to receive 1 errors
  And I discard the oldest error
  And I set the HTTP status code to 200
  And I restart the test fixture
  And I start bugsnag for "HandledJsErrorScenario"
  And I wait to receive 1 errors
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "HandledJSError"
  And the event "unhandled" is false

Scenario: Errors are stored when the network is unreachable, only maxPersistedEvents sent
  # make the network unreachable
  # cannot set for just one return code because we are also sending sessions
  Given I set the HTTP status code to 429
  When I run "MaxPersistedEventsScenario"
  And I wait to receive 10 errors

  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error

  And I set the HTTP status code to 200
  And I restart the test fixture
  And I start bugsnag for "MaxPersistedEventsScenario"
  Then I wait to receive 3 errors
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "MaxPersistedEventsError7"
  And the event "unhandled" is false
  And I discard the oldest error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "MaxPersistedEventsError8"
  And the event "unhandled" is false
  And I discard the oldest error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "MaxPersistedEventsError9"
  And the event "unhandled" is false
