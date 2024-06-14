Feature: Device id

Scenario: Device id is persisted across app starts
  When I run "HandledJsErrorScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "message" equals "HandledJSError"
  And the event "device.id" matches "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
  And the error payload field "events.0.device.id" is stored as the value "device_id"

  And I discard the oldest error
  And I restart the test fixture
  And I run "HandledJsErrorScenario"
  And I wait to receive an error
  Then the error payload field "events.0.device.id" equals the stored value "device_id"
