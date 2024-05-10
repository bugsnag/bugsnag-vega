#ifndef BUGSNAG_EVENTWRAPPER_H
#define BUGSNAG_EVENTWRAPPER_H

#include "./utils/bsg_event.h"
#include "BugsnagBreadcrumbBuffer.h"

namespace bugsnag {

class Event {
public:
  Event(int max_breadcrumbs);
  ~Event();

  void configure(std::string api_key);
  void set_exception(const char *class_arg, const char *message_arg,
                     const char *type_arg);
  void set_app_duration(time_t app_startup);

  void leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                        std::string metadata, time_t timestamp);

  void set_metadata(std::string metadata_str);
  std::string get_metadata();
  void clear_metadata();

  void set_features(std::string features_str);
  std::string get_features();
  void clear_features();

private:
  BreadcrumbBuffer breadcrumb_buffer;
  SignalSafePtr<char> metadata;
  SignalSafePtr<char> features;
  bsg_error_types enabled_error_types;
  bsg_event_payload *payload;
};
} // namespace bugsnag

#endif // BUGSNAG_EVENTWRAPPER_H