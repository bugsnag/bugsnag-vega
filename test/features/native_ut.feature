Feature: Native Side unit tests

Scenario: Run unit tests scenario on simulator
  When I run "RunNativeUnitTestsScenario"
  And I wait to receive an error
  And the exception "message" equals "0"