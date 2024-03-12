#pragma once

#include <memory>
#include <string>

namespace bugsnag {
struct Configuration {
    char api_key[64];
    std::string storage_dir;
};

class Client {
public:
    Client(std::unique_ptr<Configuration> config);

    void markLaunchCompleted();

    std::string event_dir;
private:
    std::atomic<bool> is_launching;
    std::unique_ptr<Configuration> config;
};

extern Client *global_client;
}
