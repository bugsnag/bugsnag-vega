#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : event_dir(config->storage_dir), is_launching(true),
      breadcrumb_buffer(config->max_breadcrumbs), metadata() {
  this->config = std::move(config);
}

void Client::markLaunchCompleted() { is_launching = false; }

void Client::leaveBreadcrumb(bsg_breadcrumb_type type, std::string message,
                             std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}

void Client::addMetadata(std::string metadata_str) {
  char *new_metadata = strdup(metadata_str.c_str());
  this->metadata.reset(new_metadata);
}

std::string Client::getMetadata() {
  std::string metadata_str;
  char *metadata_char = this->metadata.acquire();
  if (metadata_char != nullptr) {
    metadata_str = std::string(metadata_char);
  }

  this->metadata.release();

  return metadata_str;
}

void Client::clearMetadata() { this->metadata.reset(nullptr); }

void Client::addFeatures(std::string features_str) {
  char *new_features = strdup(features_str.c_str());
  this->features.reset(new_features);
}

std::string Client::getFeatures() {
  std::string features_str;
  char *features_char = this->features.acquire();
  if (features_char != nullptr) {
    features_str = std::string(features_char);
  }

  this->features.release();

  return features_str;
}

void Client::clearFeatures() { this->features.reset(nullptr); }
} // namespace bugsnag
