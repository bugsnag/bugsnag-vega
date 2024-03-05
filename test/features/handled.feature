Feature: Reporting handled errors

Scenario: Calling notify() with a caught Error
  When I run "HandledJsErrorScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "type" equals "reactnativejs"
  And the exception "message" equals "HandledJSError"
  And the event "unhandled" is false
