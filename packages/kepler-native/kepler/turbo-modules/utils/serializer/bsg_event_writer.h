/**
 * async-safe functions for writing an event to disk
 */
#pragma once

#include "../bsg_event.h"

#ifdef __cplusplus
extern "C" {
#endif

bool bsg_event_write(bsg_event_payload *payload);

bool bsg_write_event_file(bsg_event_payload *payload, const char *filename);

size_t bsg_time_format(time_t time, char *dst);

#ifdef __cplusplus
}
#endif