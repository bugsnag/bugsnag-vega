#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : config(std::move(config)), event_dir(config->storage_dir),
      is_launching(true), breadcrumb_buffer(config->maxBreadcrumbs) {}

void Client::markLaunchCompleted() { is_launching = false; }

void Client::leaveBreadcrumb(bsg_breadcrumb_type type, std::string message,
                             std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}
} // namespace bugsnag
