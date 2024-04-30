#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : event_dir(config->storage_dir), is_launching(true),
      breadcrumb_buffer(config->max_breadcrumbs), metadata() {
  this->config = std::move(config);
}

void Client::mark_launch_completed() { is_launching = false; }

void Client::leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                              std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}

void Client::set_metadata(std::string metadata_str) {
  char *new_metadata = strdup(metadata_str.c_str());
  this->metadata.reset(new_metadata);
}

std::string Client::get_metadata() {
  std::string metadata_str;
  char *metadata_char = this->metadata.acquire();
  if (metadata_char != nullptr) {
    metadata_str = std::string(metadata_char);
  }

  this->metadata.release();

  return metadata_str;
}

void Client::clear_metadata() { this->metadata.reset(nullptr); }

void Client::set_features(std::string features_str) {
  char *new_features = strdup(features_str.c_str());
  this->features.reset(new_features);
}

std::string Client::get_features() {
  std::string features_str;
  char *features_char = this->features.acquire();
  if (features_char != nullptr) {
    features_str = std::string(features_char);
  }

  this->features.release();

  return features_str;
}

void Client::clear_features() { this->features.reset(nullptr); }
} // namespace bugsnag
