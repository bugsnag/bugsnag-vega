Feature: Checking manual breadcrumb

Scenario: Saving manual breadcrumb
  When I run "ManualBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "message" equals "ManualBreadcrumbError"
  And the event has a "manual" breadcrumb named "manualBreadcrumb"
  And the event "breadcrumbs.1.metaData.key" equals "myval"
  And the error Bugsnag-Integrity header is valid

Scenario: Modifying breadcrumbs via onBreadcrumb callbacks
  When I run "OnBreadcrumbCallbackScenario"
  Then I wait to receive an error
  And the exception "message" equals "ManualBreadcrumbError"
  And the event has 2 breadcrumbs
  And the event has a "manual" breadcrumb named "manualBreadcrumb"
  And the event "breadcrumbs.1.metaData.config" equals "works"
  And the event "breadcrumbs.1.metaData.addOnBreadcrumb" equals "works"
