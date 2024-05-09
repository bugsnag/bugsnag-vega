#ifndef BUGSNAG_ERRORTYPE_H
#define BUGSNAG_ERRORTYPE_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

const int ERROR_CLASS_SIZE = 64;
const int ERROR_MESSAGE_SIZE = 256;
const int ERROR_TYPE_SIZE = 32;

typedef enum {
  BSG_SEVERITY_ERR,
  BSG_SEVERITY_WARN,
  BSG_SEVERITY_INFO,
} bsg_severity;

typedef struct {
  char *type;
} bsg_event_severity_reason;

typedef struct {
  bool anrs;
  bool app_hangs;
  bool cpp_exceptions;
  bool mach_exceptions;
  bool ndk_crashes;
  bool ooms;
  bool signals;
  bool unhandled_exceptions;
  bool unhandled_rejections;
} bsg_error_types;

typedef struct {
  char *id;
  char *email;
  char *name;
} bsg_user;

typedef struct {
  char *id;
  char *hostname;
  char *locale;
  char *manufacturer;
  char *model;
  char *model_number;
  char *orientation;
  char *os_name;
  char *os_version;
  time_t time;
} bsg_device;

typedef struct {
  char *code_bundle_id;
  int duration;
  int duration_in_foreground;
  bool in_foreground;
  char *release_stage;
  char *type;
  char *version;
} bsg_app;

typedef struct {
  char *name;
  char *version;
  char *url;
} bsg_notifier_info;

typedef struct {
  char error_class[ERROR_CLASS_SIZE];
  char error_message[ERROR_MESSAGE_SIZE];
  char type[ERROR_TYPE_SIZE];
} bsg_error;

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_ERRORTYPE_H