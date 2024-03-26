Feature: Switching off automatic reporting

Scenario: setting autoDetectErrors option to false
  When I run "AutoDetectErrorsScenario"
  And I wait for 5 seconds
  Then I wait to receive an error
  And the event "unhandled" is false
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "manual notify does work"

Scenario: setting enabledErrorTypes.unhandledRejections option to false
  When I run "UnhandledPromiseRejectionsDisabledScenario"
  And I wait for 5 seconds
  Then I wait to receive an error
  And the event "unhandled" is false
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "manual notify does work"