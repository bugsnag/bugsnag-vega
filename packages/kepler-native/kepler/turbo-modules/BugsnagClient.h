#pragma once

#include <memory>
#include <string>

#include "BugsnagBreadcrumbBuffer.h"
namespace bugsnag {
struct Configuration {
  char api_key[64];
  std::string storage_dir;
  int maxBreadcrumbs = BUGSNAG_CRUMBS_MAX;
};

class Client {
public:
  Client(std::unique_ptr<Configuration> config);

  void markLaunchCompleted();

  void leaveBreadcrumb(bsg_breadcrumb_type type, std::string message,
                       std::string metadata, time_t timestamp);

  std::string event_dir;

private:
  std::atomic<bool> is_launching;
  std::unique_ptr<Configuration> config;
  BreadcrumbBuffer breadcrumb_buffer;
};

extern Client *global_client;
} // namespace bugsnag
