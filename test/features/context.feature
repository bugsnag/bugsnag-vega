Feature: Context

Scenario: JS custom context
  When I run "ContextJsCustomScenario"
  And I wait to receive 3 errors
  Then the received errors match:
    | exceptions.0.errorClass | exceptions.0.message     | context          | 
    | Error                   | ContextJsCustomScenarioA | context-config   |
    | Error                   | ContextJsCustomScenarioB | context-client   | 
    | Error                   | ContextJsCustomScenarioC | context-onerror  |
