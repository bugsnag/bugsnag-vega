Feature: Reporting with feature flags

Scenario: Sends handled exception which includes feature flags
  When I run "FeatureFlagsScenario"
  Then I wait to receive an error
  And the exception "errorClass" equals "Error"
  And the event "unhandled" is false
  And event 0 contains the feature flag "demo_mode" with no variant
  And event 0 contains the feature flag "sample_group" with variant "a"
  And event 0 does not contain the feature flag "should_not_be_reported_1"
  And event 0 does not contain the feature flag "should_not_be_reported_2"
  And event 0 does not contain the feature flag "should_not_be_reported_3"