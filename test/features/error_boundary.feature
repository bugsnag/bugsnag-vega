Feature: Capturing render errors

Scenario: Render errors are captured by the ErrorBoundary
  When I run "ErrorBoundaryScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "message" equals "render error"
  And the event "metaData.react.componentStack" is not null
