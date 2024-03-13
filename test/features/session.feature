Feature: Sessions

Scenario: Automatic sessions enabled by default
  When I run "HandledJsErrorScenario"
  And I wait to receive a session
  Then the session is valid for the session reporting API version "1" for the "Bugsnag Kepler" notifier
  And the session payload has a valid sessions array

Scenario: Automatic sessions disabled
  When I run "SessionAutoDisabledScenario"
  And I wait for 5 seconds
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