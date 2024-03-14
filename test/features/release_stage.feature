Feature: Reporting errors with release stages

Scenario: Errors are not reported when release stage is disabled
  When I run "ReleaseStageDisabledScenario"
  Then I should receive no errors

Scenario: Errors are reported when release stage is enabled
  When I run "ReleaseStageEnabledScenario"
  And I wait to receive an error
  Then the exception "errorClass" equals "Error"
  And the exception "type" equals "reactnativejs"
  And the exception "message" equals "ReleaseStageEnabledScenario"
  And the event "app.releaseStage" equals "test"

