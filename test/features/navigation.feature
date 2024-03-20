Feature: React navigation

Scenario: Navigation breadcrumbs enabled
  When I run "NavigationBreadcrumbScenario"
  Then I wait to receive 3 errors
  And the exception "message" equals "Error on Screen1"
  And the event "context" equals "Screen1"
  And the event "breadcrumbs.1.metaData.to" equals "Screen1"
  And I discard the oldest error
  And the exception "message" equals "Error on Screen2"
  And the event "context" equals "Screen2"
  And the event "breadcrumbs.2.metaData.to" equals "Screen2"
  And I discard the oldest error
  And the exception "message" equals "NaviBreadcrumbError"
  And the event "breadcrumbs.3.metaData.to" equals "Screen3"
  And the error Bugsnag-Integrity header is valid

Scenario: Navigation breadcrumbs disabled but context exists
  When I run "NavigationCrumbsDisabledScenario"
  Then I wait to receive 3 errors
  And the exception "message" equals "Error on Screen1"
  And the event "context" equals "Screen1"
  And the event does not have a "navigation" breadcrumb
  And I discard the oldest error
  And the exception "message" equals "Error on Screen2"
  And the event "context" equals "Screen2"
  And the event does not have a "navigation" breadcrumb
  And I discard the oldest error
  And the exception "message" equals "NaviBreadcrumbError"
  And the event does not have a "navigation" breadcrumb
  And the error Bugsnag-Integrity header is valid
