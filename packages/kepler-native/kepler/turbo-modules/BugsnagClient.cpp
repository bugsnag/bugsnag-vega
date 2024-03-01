#include "BugsnagClient.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config) :
    config(std::move(config)),
    event_filename(config->storage_dir),
    is_launching(true) {

   event_filename.append("crashfile.txt");
}

void Client::markLaunchCompleted() {
    is_launching = false;
}
}
