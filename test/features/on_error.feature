Feature: onError callbacks

Scenario: Modifying report via onError callbacks
  When I run "OnErrorCallbackScenario"
  And I wait to receive an error
  Then the event "metaData.on_error.global" equals "works"
  And the event "metaData.on_error.addOnError" equals "works"
  And the event "metaData.on_error.notify_opts" equals "works"

Scenario: Ignoring report via onError callbacks
  When I run "OnErrorPreventSendScenario"
  And I wait to receive 1 error
  Then the exception "message" equals "send"
