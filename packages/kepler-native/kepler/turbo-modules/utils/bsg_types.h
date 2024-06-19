#ifndef BUGSNAG_ERRORTYPE_H
#define BUGSNAG_ERRORTYPE_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSG_MAX_STACK_FRAMES 512
#define ERROR_CLASS_SIZE 64
#define ERROR_MESSAGE_SIZE 256
#define ERROR_TYPE_SIZE 32
#define STACKFRAME_NAME 256
#define STACKFRAME_CODE 65

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
  char *manufacturer;
  char *model;
  char *os_name;
  char *os_version;
  char *rn_version;
  char *js_engine;
  time_t time;
} bsg_device;

typedef struct {
  char *id;
  int duration;
  int duration_in_foreground;
  bool in_foreground;
  char *release_stage;
  char *type;
  char *version;
  char *binary_arch;
} bsg_app;

typedef struct {
  char *name;
  char *version;
  char *url;
} bsg_notifier_info;

typedef struct {
  uintptr_t frame_address;
  uintptr_t symbol_address;
  uintptr_t load_address;
  uintptr_t line_number;

  char filename[STACKFRAME_NAME];
  char method[STACKFRAME_NAME];
  char code_identifier[STACKFRAME_CODE];
} bsg_stackframe;

typedef struct {
  char error_class[ERROR_CLASS_SIZE];
  char error_message[ERROR_MESSAGE_SIZE];
  char type[ERROR_TYPE_SIZE];

  /**
   * The number of frames used in the stacktrace. Must be less than
   * BSG_MAX_STACK_FRAMES.
   */
  ssize_t frame_count;
  /**
   * An ordered list of stack frames from the oldest to the most recent
   */
  bsg_stackframe stacktrace[BSG_MAX_STACK_FRAMES];
} bsg_error;

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_ERRORTYPE_H