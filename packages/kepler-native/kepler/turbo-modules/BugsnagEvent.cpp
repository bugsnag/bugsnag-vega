#include <string>

#include "./utils/bsg_breadcrumb.h"
#include "BugsnagEvent.h"

namespace bugsnag {
Event::Event(int max_breadcrumbs)
    : breadcrumb_buffer(max_breadcrumbs), metadata(), features(),
      enabled_error_types() {
  // default handling all error types to true
  this->enabled_error_types = bsg_error_types_all_enabled();
}

Event::~Event() { bsg_free_event_payload(this->payload); }

void Event::configure(std::string api_key, std::string event_dir) {
  this->payload =
      bsg_new_event_payload(api_key.c_str(), event_dir.c_str(), "4");

  // TODO fill with proper data
  bsg_set_event_notifier_info(this->payload, "Bugsnag Kepler", "0.0.0",
                              "https://github.com/bugsnag/bugsnag-kepler");
}

void Event::set_exception(const char *class_arg, const char *message_arg,
                          const char *type_arg, int stackframe_count) {
  if (!this->payload) {
    return;
  }

  bsg_set_event_exception(this->payload, class_arg, message_arg, type_arg);
  bsg_set_event_stacktrace_size(this->payload, stackframe_count);
}

bsg_stackframe *Event::get_exception_stackframe() {
  if (!this->payload) {
    return nullptr;
  }
  return this->payload->event.exception.stacktrace;
}

void Event::prepare_payload(time_t app_startup, bool is_launching,
                            bsg_breadcrumb **crumb_buffer,
                            int crumb_buffer_size) {
  if (!this->payload) {
    return;
  }
  this->payload->metadata = this->metadata.move();
  this->payload->features = this->features.move();

  this->payload->max_breadcrumbs_size =
      this->breadcrumb_buffer.get_buffer_max_size();
  this->payload->breadcrumbs_size =
      this->breadcrumb_buffer.fill_buffer(crumb_buffer, crumb_buffer_size);
  this->payload->breadcrumbs = crumb_buffer;

  auto now = std::chrono::system_clock::now();
  time_t now_time = std::chrono::system_clock::to_time_t(now);
  time_t time_from_start = difftime(now_time, app_startup);
  time_from_start = time_from_start * 1000; // event needs time in ms
  // TODO Calculate time in foreground
  bsg_set_event_app_duration(this->payload, time_from_start, time_from_start,
                             true);
  bsg_set_event_device_time(this->payload, now_time);
  bsg_set_notifier_start_time(this->payload, app_startup);
  bsg_set_event_is_launching(this->payload, is_launching);
}

bsg_event_payload *Event::get_payload() { return this->payload; }

void Event::leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                             std::string metadata, time_t timestamp) {
  this->breadcrumb_buffer.add(type, message, metadata, timestamp);
}

void Event::set_metadata(std::string metadata_str) {
  char *new_metadata = strdup(metadata_str.c_str());
  this->metadata.reset(new_metadata);
}

std::string Event::get_metadata() {
  std::string metadata_str;
  char *metadata_char = this->metadata.acquire();
  if (metadata_char != nullptr) {
    metadata_str = std::string(metadata_char);
  }

  this->metadata.release();

  return metadata_str;
}

void Event::clear_metadata() { this->metadata.reset(nullptr); }

void Event::set_features(std::string features_str) {
  char *new_features = strdup(features_str.c_str());
  this->features.reset(new_features);
}

std::string Event::get_features() {
  std::string features_str;
  char *features_char = this->features.acquire();
  if (features_char != nullptr) {
    features_str = std::string(features_char);
  }

  this->features.release();

  return features_str;
}

void Event::clear_features() { this->features.reset(nullptr); }

void Event::set_user_data(std::string id, std::string email, std::string name) {
  if (!this->payload) {
    return;
  }
  bsg_set_event_user(this->payload, id.c_str(), email.c_str(), name.c_str());
}

void Event::clear_user_data() {
  if (!this->payload) {
    return;
  }
  bsg_set_event_user(this->payload, NULL, NULL, NULL);
}

void Event::set_device_id(std::string device_id) {
  if (!this->payload) {
    return;
  }
  bsg_set_event_device_id(this->payload, device_id.c_str());
}

void Event::set_app_data(std::string id, std::string stage,
                         std::string type, std::string ver, std::string binary_arch) {
  if (!this->payload) {
    return;
  }
  bsg_set_event_app(this->payload, id.c_str(), stage.c_str(),
                    type.c_str(), ver.c_str(), binary_arch.c_str());
}

void Event::set_device_data(std::string manufacturer, std::string model,
                         std::string os_name, std::string os_version,
                         std::string rn_version, std::string js_engine) {
  if (!this->payload) {
    return;
  }
  bsg_set_event_device(this->payload, manufacturer.c_str(), model.c_str(),
                    os_name.c_str(), os_version.c_str(), rn_version.c_str(), js_engine.c_str());
}

} // namespace bugsnag