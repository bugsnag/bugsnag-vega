Feature: Native crashes tests

Scenario: Run native crash - readonly memory location
  When I run "NativeCrashScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashScenario"
  Then I wait to receive 1 errors
  And the exception "errorClass" equals "SIGSEGV"
  And the exception "message" equals "Segmentation violation (invalid memory reference)"
  And the event "unhandled" is true
  
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

  # App data
  And the event "app.id" equals "com.bugsnag.fixtures.vegatestapp"
  And the event "app.releaseStage" equals "production"
  And the event "app.binaryArch" is not null

  # User data
  And the error payload field "events.0.device.id" is stored as the value "device_id" 
  And the error payload field "events.0.user.id" equals the stored value "device_id"

Scenario: Run native crash - readonly memory location, with full config
  When I run "NativeCrashFullConfigScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashFullConfigScenario"
  Then I wait to receive 1 errors
  And the exception "errorClass" equals "SIGSEGV"
  And the exception "message" equals "Segmentation violation (invalid memory reference)"
  And the event "unhandled" is true
  And the event "metaData.add_metadata.key" equals "value"
  # And event 0 contains the feature flag "myfeature1" with no variant

  # User data
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"

  # App data
  And the event "app.id" equals "com.bugsnag.fixtures.vegatestapp"
  And the event "app.releaseStage" equals "test"
  And the event "app.version" equals "1.2.3"
  And the event "app.type" equals "kepler"
  And the event "app.binaryArch" is not null

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

Scenario: Run native crash - manual calling abort func
  When I run "NativeCrashAbortScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashAbortScenario"
  Then I wait to receive 1 errors
  And the exception "errorClass" equals "SIGABRT"
  And the exception "message" equals "Abort program"
  And the event "unhandled" is true

Scenario: Run native crash - dereferencing nullptr
  When I run "NativeCrashNullptrScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashNullptrScenario"
  Then I wait to receive 1 errors
  And the exception "errorClass" equals "SIGSEGV"
  And the exception "message" equals "Segmentation violation (invalid memory reference)"
  And the event "unhandled" is true

Scenario: Run native crash - native errors disabled
  When I run "NativeCrashDisabledScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashDisabledScenario"
  Then I should receive no errors

Scenario: Run native crash - throwing exception
  When I run "NativeCrashExceptionScenario"
  Then I should receive no errors
  And I copy error file
  And I restart the test fixture
  And I start bugsnag for "NativeCrashExceptionScenario"
  Then I wait to receive 1 errors
  And the exception "errorClass" equals "SIGABRT"
  And the exception "message" equals "Abort program"
  And the event "unhandled" is true
