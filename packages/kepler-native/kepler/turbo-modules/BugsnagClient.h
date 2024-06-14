#pragma once

#include <memory>
#include <string>

#include "BugsnagEvent.h"
namespace bugsnag {
struct Configuration {
  std::string api_key;
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

  void set_device_id(std::string device_id);
  void set_device_data(std::string manufacturer, std::string model,
                       std::string os_name, std::string os_version,
                       std::string rn_version, std::string js_engine);

  void set_app_data(std::string bundle_id, std::string stage, std::string type,
                    std::string ver);

  void set_user_data(std::string id, std::string email, std::string name);
  void clear_user_data();

  Event *release_event();

  time_t get_app_start_time();
  bool get_is_launching();

  std::string event_dir;

private:
  time_t start_time;
  std::atomic<bool> is_launching;
  std::unique_ptr<Configuration> config;
  std::unique_ptr<Event> current_event;
};

extern Client *global_client;
} // namespace bugsnag
