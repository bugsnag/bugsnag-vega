Feature: Metadata

Scenario: Calling addMetadata with a key/value pair shall add the data to the stored metadata for the specified section
  When I run "AddMetadataScenario"
  And I wait to receive an error
  Then the event "metaData.config.key" equals "value"
  And the event "metaData.add_metadata.key" equals "value"
  And the event "metaData.event_add_metadata.key" equals "value"
  And the event "metaData.metadata_section.key" equals "value"

Scenario: The notifier shall redact the value of any MetadataSection where the key matches any strings or regexes in the list of redactedKeys configuration property
  When I run "RedactMetadataScenario"
  And I wait to receive an error
  Then the event "metaData.config.key" equals "[REDACTED]"
  And the event "metaData.add_metadata.key" equals "[REDACTED]"
  And the event "metaData.event_add_metadata.key" equals "[REDACTED]"
  And the event "metaData.metadata_section.key" equals "[REDACTED]"
