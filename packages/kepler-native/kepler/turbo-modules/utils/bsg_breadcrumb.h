#ifndef BUGSNAG_BREADCRUMB_H
#define BUGSNAG_BREADCRUMB_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

enum bsg_breadcrumb_type {
  // An error was sent to Bugsnag (internal use only)
  ERROR,
  // A log message
  LOG,
  // A manual invocation of `leaveBreadcrumb` (default)
  MANUAL,
  // A navigation event, such as a window opening or closing
  NAVIGATION,
  // A background process such as a database query
  PROCESS,
  // A network request
  REQUEST,
  // A change in application state, such as launch or memory warning
  STATE,
  // A user action, such as tapping a button
  USER
};

typedef struct {
  bsg_breadcrumb_type type;
  char *message;
  char *metadata;
  time_t timestamp;
} bsg_breadcrumb;

bsg_breadcrumb *new_breadcrumb(bsg_breadcrumb_type type, const char *message,
                               const char *metadata, time_t timestamp);

void free_breadcrumb_fields(bsg_breadcrumb *crumb);

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_BREADCRUMB_H