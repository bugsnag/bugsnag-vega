#pragma once

#include <memory>
#include <string>

#include "BugsnagBreadcrumbBuffer.h"
namespace bugsnag {
struct Configuration {
  char api_key[64];
  std::string storage_dir;
  int max_breadcrumbs = BUGSNAG_CRUMBS_MAX;
};

class Client {
public:
  Client(std::unique_ptr<Configuration> config);

  void mark_launch_completed();

  void leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                        std::string metadata, time_t timestamp);

  void set_metadata(std::string metadata_str);
  std::string get_metadata();
  void clear_metadata();

  void set_features(std::string features_str);
  std::string get_features();
  void clear_features();

  std::string event_dir;

private:
  std::atomic<bool> is_launching;
  std::unique_ptr<Configuration> config;
  BreadcrumbBuffer breadcrumb_buffer;
  SignalSafePtr<char> metadata;
  SignalSafePtr<char> features;
};

extern Client *global_client;
} // namespace bugsnag
