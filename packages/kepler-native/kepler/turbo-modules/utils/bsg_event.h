#ifndef BUGSNAG_EVENT_H
#define BUGSNAG_EVENT_H

#include "bsg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  bsg_severity severity;
  bool unhandled;
  bsg_event_severity_reason severity_reason;
  bsg_user user;
  bsg_device device;
  bsg_app app;
  bsg_error exception;
} bsg_event;

typedef struct {
  char *api_key;
  char *payload_version;
  bsg_notifier_info notifier;
  bsg_event event;
} bsg_event_payload;

bsg_event_payload *bsg_new_event_payload(const char *api_key,
                                         const char *payload_ver);
void bsg_set_event_notifier_info(bsg_event_payload *payload,
                                 const char *name_arg, const char *ver_arg,
                                 const char *url_arg);
void bsg_set_event_severity(bsg_event_payload *payload, const char *type_arg);
void bsg_set_event_user(bsg_event_payload *payload, const char *id_arg,
                        const char *email_arg, const char *name_arg);
void bsg_set_event_device(bsg_event_payload *payload, const char *id,
                          const char *host, const char *locale,
                          const char *manufacturer, const char *model,
                          const char *modelnr, const char *orient,
                          const char *osname, const char *osver);
void bsg_set_event_device_time(bsg_event_payload *payload, time_t time_arg);
void bsg_set_event_app(bsg_event_payload *payload, const char *bundle_id,
                       const char *stage, const char *type, const char *ver);
void bsg_set_event_app_duration(bsg_event_payload *payload, int duration,
                                int duration_foreground, bool in_foreground);
void bsg_set_event_exception(bsg_event_payload *payload, const char *class_arg,
                             const char *message_arg, const char *type_arg);

void bsg_free_event_app(bsg_app *app);
void bsg_free_event_device(bsg_device *device);
void bsg_free_event_user(bsg_user *user);
void bsg_free_event_severity(bsg_event_severity_reason *reason);
void bsg_free_event_notifier_info(bsg_notifier_info *info);
void bsg_free_event_payload(bsg_event_payload *payload);

bsg_error_types bsg_error_types_all_enabled();

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_EVENT_H