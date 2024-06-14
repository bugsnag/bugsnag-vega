Feature: Sessions

Scenario: Automatic sessions enabled by default
  When I run "HandledJsErrorScenario"
  And I wait to receive a session
  Then the session is valid for the session reporting API version "1" for the "Bugsnag Kepler" notifier
  And the session payload has a valid sessions array

  # Device data
  And the session payload field "device.id" matches the regex "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
  And the session payload field "device.manufacturer" equals "Amazon"
  And the session payload field "device.model" is not null
  And the session payload field "device.osName" equals "Kepler"
  And the session payload field "device.osVersion" is not null
  And the session payload field "device.runtimeVersions.reactNative" is not null
  And the session payload field "device.runtimeVersions.reactNativeJsEngine" equals "hermes"

Scenario: Automatic sessions disabled
  When I run "SessionAutoDisabledScenario"
  Then I should receive no sessions

Scenario: Manual sessions
  When I run "SessionManualScenario"
  Then I wait to receive 2 sessions
  And I wait to receive 4 errors

  # Initial session
  Then the session is valid for the session reporting API version "1" for the "Bugsnag Kepler" notifier
  And the session payload has a valid sessions array
  And the session payload field "sessions.0.id" is stored as the value "initial_session_id"

  And I discard the oldest session

  # Error triggered after the first session is started
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "SessionManualScenarioA"
  And the event "unhandled" is false
  And the event "session" is not null
  And the error payload field "events.0.session.id" equals the stored value "initial_session_id"
  And the event "session.events.handled" equals 1

  And I discard the oldest error

  # Error triggered after the first session is paused
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "SessionManualScenarioB"
  And the event "unhandled" is false
  And the event "session" is null

  And I discard the oldest error

  # Error triggered after the first session is resumed
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "SessionManualScenarioC"
  And the event "unhandled" is false
  And the event "session" is not null
  And the error payload field "events.0.session.id" equals the stored value "initial_session_id"
  And the event "session.events.handled" equals 2

  And I discard the oldest error

  # Second session
  And the session payload has a valid sessions array
  And the session payload field "sessions.0.id" does not equal the stored value "initial_session_id"
  And the session payload field "sessions.0.id" is stored as the value "second_session_id"

  # Error triggered after the second session is started
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "SessionManualScenarioD"
  And the event "unhandled" is false
  And the event "session" is not null
  And the error payload field "events.0.session.id" equals the stored value "second_session_id"
  And the event "session.events.handled" equals 1

Scenario: Modifying sessions via onSession callbacks
  When I run "OnSessionCallbackScenario"
  And I wait to receive a session
  Then the session is valid for the session reporting API version "1" for the "Bugsnag Kepler" notifier
  And the session payload has a valid sessions array
  And the session payload field "app.releaseStage" equals "test"
  And the session payload field "sessions.0.user.id" equals "123"
  And the session payload field "sessions.0.user.name" equals "Test User"
  And the session payload field "sessions.0.user.email" equals "test@bugsnag.com"

Scenario: Sessions are stored when the network is unreachable, only maxPersistedSessions sent
  # make the network unreachable
  # cannot set for just one return code because we are also sending events
  Given I set the HTTP status code to 429
  When I run "MaxPersistedSessionsScenario"

  And I wait to receive 10 sessions
  And I wait to receive 10 errors

  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session
  And I discard the oldest session

  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error
  And I discard the oldest error

  And I set the HTTP status code to 200
  And I restart the test fixture
  And I start bugsnag for "MaxPersistedSessionsScenario"
  Then I wait to receive 3 sessions
  And I wait to receive 10 errors
  And the session payload field "sessions.0.id" is stored as the value "MaxPersistedSessions7"
  And I discard the oldest session
  And the session payload field "sessions.0.id" is stored as the value "MaxPersistedSessions8"
  And I discard the oldest session
  And the session payload field "sessions.0.id" is stored as the value "MaxPersistedSessions9"
