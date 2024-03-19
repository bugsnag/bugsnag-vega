Feature: Checking breadcrumbs

Scenario: Getting breadcrumb from console log
  When I run "ConsoleBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "message" equals "ConsoleBreadcrumbError"
  And the event has a "log" breadcrumb named "Console output"
  And the event contains a breadcrumb matching the JSON fixture in "features/breadcrumbs/console_breadcrumb_match.json"
  And the error Bugsnag-Integrity header is valid

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

Scenario: Getting breadcrumb from fetch request
  When I run "NetworkBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "NetworkBreadcrumbError"
  And the event has a "request" breadcrumb named "XMLHttpRequest succeeded"
  And the event contains a breadcrumb matching the JSON fixture in "features/breadcrumbs/network_breadcrumb_match.json"
  And the error Bugsnag-Integrity header is valid

Scenario: With breadcrumbs disabled none should be added
  When I run "DisabledBreadcrumbScenario"
  Then I wait to receive 3 errors
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "ConsoleBreadcrumbDisabledError"
  And the event has a "state" breadcrumb named "Bugsnag loaded"
  And the event does not have a "log" breadcrumb
  And I discard the oldest error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "ManualBreadcrumbDisabledError"
  And the event has a "manual" breadcrumb named "manualBreadcrumb"
  And the event "breadcrumbs.1.metaData.key" equals "myval"
  And I discard the oldest error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "NetworkBreadcrumbDisabledError"
  And the event does not have a "request" breadcrumb

Scenario: Saving manual breadcrumb
  When I run "MaxBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "message" equals "MaxBreadcrumbError"
  And the event has 5 breadcrumbs
  And the event "breadcrumbs.0.metaData.key" equals 15
  And the event "breadcrumbs.1.metaData.key" equals 16
  And the event "breadcrumbs.2.metaData.key" equals 17
  And the event "breadcrumbs.3.metaData.key" equals 18
  And the event "breadcrumbs.4.metaData.key" equals 19
  And the error Bugsnag-Integrity header is valid

Scenario: Navigation breadcrumbs enabled
  When I run "NavigationBreadcrumbScenario"
  Then I wait to receive 2 errors
  And the exception "message" equals "NaviBreadcrumbError"
  And the event has a "navigation" breadcrumb named "React Navigation onReady"
  And the event "breadcrumbs.1.metaData.to" equals "Screen2"
  And the event "breadcrumbs.2.metaData.to" equals "Screen3"
  And the error Bugsnag-Integrity header is valid
