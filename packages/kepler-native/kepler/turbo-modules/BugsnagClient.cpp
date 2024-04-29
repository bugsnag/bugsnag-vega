#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : event_dir(config->storage_dir), is_launching(true),
      breadcrumb_buffer(config->maxBreadcrumbs), metadata() {
  this->config = std::move(config);
}

void Client::markLaunchCompleted() { is_launching = false; }

void Client::leaveBreadcrumb(bsg_breadcrumb_type type, std::string message,
                             std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}

void Client::addMetadata(std::string metadataStr) {
  char *newMetadata = strdup(metadataStr.c_str());
  this->metadata.reset(newMetadata);
}

std::string Client::getMetadata() {
  std::string metadataStr;
  char *metadataChar = this->metadata.acquire();
  if (metadataChar != nullptr) {
    metadataStr = std::string(metadataChar);
  }

  this->metadata.release();

  return metadataStr;
}

void Client::clearMetadata() { this->metadata.reset(nullptr); }
} // namespace bugsnag
