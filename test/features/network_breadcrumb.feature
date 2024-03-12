Feature: Checking auto added breadcrumbs on network requests

Scenario: Getting breadcrumb from fetch request
  When I run "NetworkBreadcrumbScenario"
  Then I wait to receive an error
  And the exception "errorClass" equals "Error"
  And the exception "message" equals "NetworkBreadcrumbError"
  And the event has a "request" breadcrumb named "XMLHttpRequest succeeded"
  And the event contains a breadcrumb matching the JSON fixture in "features/breadcrumbs/network_breadcrumb_match.json"
  And the error Bugsnag-Integrity header is valid