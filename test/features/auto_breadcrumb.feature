Feature: Checking auto added breadcrumbs

Scenario: Getting breadcrumb from console log
  When I run "ConsoleBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "message" equals "ConsoleBreadcrumbError"
  And the event has a "log" breadcrumb named "Console output"
  And the event contains a breadcrumb matching the JSON fixture in "features/breadcrumbs/console_breadcrumb_match.json"
  And the error Bugsnag-Integrity header is valid