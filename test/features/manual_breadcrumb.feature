Feature: Checking manual breadcrumb

Scenario: Saving manual breadcrumb
  When I run "ManualBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "message" equals "ManualBreadcrumbError"
  And the event has a "manual" breadcrumb named "manualBreadcrumb"
  And the event "breadcrumbs.1.metaData.key" equals "myval"
  And the error Bugsnag-Integrity header is valid