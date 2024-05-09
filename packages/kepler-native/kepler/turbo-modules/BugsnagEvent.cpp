#include <string>

#include "./utils/bsg_breadcrumb.h"
#include "BugsnagEvent.h"

namespace bugsnag {
Event::Event(int max_breadcrumbs)
    : breadcrumb_buffer(max_breadcrumbs), metadata(), features(),
      enabled_error_types() {
  // default handling all error types to true
  this->enabled_error_types = bsg_error_types_all_enabled();
}

Event::~Event() { bsg_free_event_payload(this->payload); }

void Event::configure(std::string api_key) {
  this->payload = bsg_new_event_payload(api_key.c_str(), "4");

  // TODO fill with proper data
  bsg_set_event_notifier_info(this->payload, "kepler", "version", "url");
  bsg_set_event_user(this->payload, "id", "email", "name");
  bsg_set_event_app(this->payload, "bundle_id", "dev", "apptype", "version");
}

void Event::leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                             std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}

void Event::set_metadata(std::string metadata_str) {
  char *new_metadata = strdup(metadata_str.c_str());
  this->metadata.reset(new_metadata);
}

std::string Event::get_metadata() {
  std::string metadata_str;
  char *metadata_char = this->metadata.acquire();
  if (metadata_char != nullptr) {
    metadata_str = std::string(metadata_char);
  }

  this->metadata.release();

  return metadata_str;
}

void Event::clear_metadata() { this->metadata.reset(nullptr); }

void Event::set_features(std::string features_str) {
  char *new_features = strdup(features_str.c_str());
  this->features.reset(new_features);
}

std::string Event::get_features() {
  std::string features_str;
  char *features_char = this->features.acquire();
  if (features_char != nullptr) {
    features_str = std::string(features_char);
  }

  this->features.release();

  return features_str;
}

void Event::clear_features() { this->features.reset(nullptr); }
} // namespace bugsnag