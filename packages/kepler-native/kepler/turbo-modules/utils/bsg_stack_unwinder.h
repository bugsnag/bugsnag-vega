#ifndef BUGSNAG_STACKUNWIND_H
#define BUGSNAG_STACKUNWIND_H

#include "bsg_event.h"
#include "bsg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void bsg_fill_stack_info(bsg_stackframe *error_stacktrace, int *frames_count);

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_EVENT_H