Feature: Reporting unhandled errors

Scenario: Catching an unhandled promise rejection
  When I run "UnhandledPromiseRejectionScenario"
  Then I wait to receive an error
  And the exception "errorClass" equals "Error"
  And the exception "type" equals "reactnativejs"
  And the event "unhandled" is true
  And the event "severityReason.type" equals "unhandledPromiseRejection"
  And the exception "message" equals "UnhandledPromiseRejectionScenario"

  # App data
  And the event "app.releaseStage" equals "production"
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
