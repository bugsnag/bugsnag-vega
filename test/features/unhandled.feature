Feature: Reporting unhandled errors

Scenario: Catching an unhandled promise rejection
  When I run "UnhandledPromiseRejectionScenario"
  Then I wait to receive an error
  And the exception "errorClass" equals "Error"
  And the event "unhandled" is true
  And the event "severityReason.type" equals "unhandledPromiseRejection"
  And the exception "message" equals "UnhandledPromiseRejectionScenario"
