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
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"
  And the event "metaData.add_metadata.key" equals "value"
  # And event 0 contains the feature flag "myfeature1" with no variant

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
  And the exception "errorClass" equals "SIGABRT"
  And the exception "message" equals "Abort program"
  And the event "unhandled" is true

# Scenario: Run native crash - throwing exception
#   When I run "NativeCrashExceptionScenario"
#   Then I should receive no errors
#   And I copy error file
#   And I restart the test fixture
#   And I start bugsnag for "NativeCrashExceptionScenario"
#   Then I wait to receive 1 errors
#   And the exception "errorClass" equals "SIGABRT"
#   And the exception "message" equals "Abort program"
#   And the event "unhandled" is true