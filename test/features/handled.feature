Feature: Reporting handled errors

Scenario: Calling notify() with a caught Error
  When I run "HandledJsErrorScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "type" equals "reactnativejs"
  And the exception "message" equals "HandledJSError"
  And the event "unhandled" is false
  And the event "context" is null
  And the event "app.binaryArch" is not null

Scenario: Errors are stored when the network is unreachable
  # make the network unreachable
  Given I configure the endpoints to "127.0.0.1:8000"
  When I run "HandledJsErrorScenario"
  * I should receive no errors
  * I restart the test fixture
  * I configure the endpoints to default
  * I start bugsnag for "HandledJsErrorScenario"
  * I wait to receive an error
  * the exception "errorClass" equals "Error"
  * the exception "message" equals "HandledJSError"
  * the event "unhandled" is false

Scenario: Errors are stored when the network is unreachable, only maxPersistedEvents sent
  # make the network unreachable
  Given I configure the endpoints to "127.0.0.1:8000"
  When I run "MaxPersistedEventsScenario"
  Then I should receive no errors
  And I restart the test fixture
  And I configure the endpoints to default
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
