Feature: Checking user information stored in events

Scenario: Default user is set where no config provided or persisted
  When I run "NoUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "NoUserError"
  And the event "user.id" equals "deviceId"
  And the event "user.name" is null
  And the event "user.email" is null

Scenario: User is persisted after it's configured first time
  When I run "SetUserScenario"
  Then I wait to receive 1 error
  And the exception "message" equals "SetUserError"
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
  And the event "user.id" equals "deviceId"
  And the event "user.name" is null
  And the event "user.email" is null
