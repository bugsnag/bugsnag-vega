Feature: Checking user information stored in events

Scenario: Default user is set where no config provided or persisted
  When I run "NoUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "NoUserError"
  And the error payload field "events.0.device.id" is stored as the value "device_id"
  And the error payload field "events.0.user.id" equals the stored value "device_id"
  And the event "user.id" matches "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
  And the event "user.name" is null
  And the event "user.email" is null

Scenario: User is persisted when set via config
  When I run "SetUserConfigScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "SetUserConfigError"
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"
  And I discard the oldest error
  And I restart the test fixture
  And I run "NoUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "NoUserError"
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"

Scenario: User is persisted when set via client
  When I run "SetUserClientScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "SetUserClientError"
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"
  And I discard the oldest error
  And I restart the test fixture
  And I run "NoUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "NoUserError"
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"

Scenario: User is not persisted when the option is false
  When I run "SetUserNotPersistedScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "SetUserNotPersistedError"
  And the event "user.id" equals "userID"
  And the event "user.name" equals "Test User"
  And the event "user.email" equals "abcd"
  And I discard the oldest error
  And I restart the test fixture
  And I run "NoUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "NoUserError"
  And the error payload field "events.0.device.id" is stored as the value "device_id"
  And the error payload field "events.0.user.id" equals the stored value "device_id"
  And the event "user.id" matches "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
  And the event "user.name" is null
  And the event "user.email" is null
