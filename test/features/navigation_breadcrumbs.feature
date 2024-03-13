Feature: Checking navigation breadcrumb

Scenario: Navigation breadcrumbs enabled
  When I run "NavigationBreadcrumbScenario"
  Then I wait to receive 2 errors
  And the exception "message" equals "NaviBreadcrumbError"
  And the event has a "navigation" breadcrumb named "React Navigation onReady"
  And the event "breadcrumbs.1.metaData.to" equals "Screen2"
  And the event "breadcrumbs.2.metaData.to" equals "Screen3"
  And the error Bugsnag-Integrity header is valid