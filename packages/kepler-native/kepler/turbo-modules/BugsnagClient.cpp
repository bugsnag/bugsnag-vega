#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : config(std::move(config)), event_dir(config->storage_dir),
      is_launching(true) {}

void Client::markLaunchCompleted() { is_launching = false; }
} // namespace bugsnag
