Feature: Reporting handled errors

Scenario: Calling notify() with a caught Error
  When I launch the app
  And I run 'HandledJsErrorScenario'
  And I wait to receive an error
  Then the exception 'errorClass' equals 'Error'
  And the exception 'message' equals 'HandledJSError'
  # And the event 'unhandled' is false
