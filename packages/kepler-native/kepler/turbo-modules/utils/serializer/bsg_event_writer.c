#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../bsg_breadcrumb.h"
#include "../bsg_types.h"
#include "BSG_KSCrashStringConversion.h"
#include "BSG_KSJSONCodec.h"
#include "bsg_buffered_writer.h"
#include "bsg_event_writer.h"

#define CHECKED(e)                                                             \
  if ((e) != BSG_KSJSON_OK) {                                                  \
    goto error;                                                                \
  }

#define JSON_CONSTANT_ELEMENT(name, value)                                     \
  bsg_ksjsonaddStringElement(json, name, value,                                \
                             sizeof(value) - 1 /* remove terminator */)
#define JSON_LIMITED_STRING_ELEMENT(name, value)                               \
  bsg_ksjsonaddStringElement(json, (name), (value),                            \
                             strnlen((value), sizeof((value))))

#define STR_CONST_CAT(dst, src) bsg_strncpy((dst), (src), sizeof(src))

static inline bool string_is_not_empty(const char *restrict s) {
  return (*(s) != 0);
}

static inline bool string_is_empty(const char *restrict s) {
  return (*(s) == 0);
}

// we use a single buffer for all of the hex encoded strings
// the bsg_uint64_to_hex doesn't prefix the '0x' so we pre-place that in the
// buffer, and then overwrite the rest of the buffer
static char hex_str[2 /* '0x' */ + 20 /* number */ + 1 /* NULL */] = "0x";

static bool bsg_write_metadata(BSG_KSJSONEncodeContext *json, char *metadata);
static bool bsg_write_severity_reason(BSG_KSJSONEncodeContext *json,
                                      bsg_event *event);
static bool bsg_write_user(BSG_KSJSONEncodeContext *json, bsg_user *user);
static bool bsg_write_error(BSG_KSJSONEncodeContext *json, bsg_error *error);
static bool bsg_write_app(BSG_KSJSONEncodeContext *json, bsg_app *app,
                          bool is_launching);
static bool bsg_write_device(BSG_KSJSONEncodeContext *json, bsg_device *device);
static bool bsg_write_breadcrumbs(BSG_KSJSONEncodeContext *json,
                                  bsg_breadcrumb **breadcrumbs, int crumb_count,
                                  int max_crumb_count);
static bool bsg_write_feature_flags(BSG_KSJSONEncodeContext *json,
                                    char *features);
static int bsg_write(const char *data, size_t length, void *userData) {
  bsg_buffered_writer *writer = userData;
  return writer->write(writer, data, length) ? BSG_KSJSON_OK
                                             : BSG_KSJSON_ERROR_CANNOT_ADD_DATA;
}

/*
 * Build the event filename in the format:
 * "[timestamp]_[apiKey].json"
 */
static size_t build_filename(bsg_event_payload *payload, char *out) {
  int length = strnlen(payload->event_path, 4096);
  memcpy(out, payload->event_path, length);
  out[length++] = '/';

  // the timestamp formatted
  length += bsg_time_format(payload->event.device.time, &out[length]);

  // append the api_key to the filename
  out[length++] = '_';
  length +=
      bsg_strncpy(out + length, payload->api_key, strlen(payload->api_key) + 1);

  length += bsg_strncpy(out + length, ".json", strlen(".json") + 1);

  return length;
}

bool bsg_event_write(bsg_event_payload *payload) {
  char filename[sizeof(payload->event_path) + 256];
  filename[0] = '\0';
  build_filename(payload, filename);

  return bsg_write_event_file(payload, filename);
}

bool bsg_write_event_file(bsg_event_payload *payload, const char *filename) {
  BSG_KSJSONEncodeContext jsonContext;
  bsg_buffered_writer writer;
  bsg_event *event = &payload->event;
  BSG_KSJSONEncodeContext *json = &jsonContext;

  if (!bsg_buffered_writer_open(&writer, filename)) {
    return false;
  }

  bsg_ksjsonbeginEncode(json, false, &bsg_write, &writer);

  CHECKED(bsg_ksjsonbeginObject(json, NULL));
  {
    if (!bsg_write_metadata(json, payload->metadata)) {
      goto error;
    }

    if (!bsg_write_severity_reason(json, event)) {
      goto error;
    }

    // Write the exception / error info
    if (!bsg_write_error(json, &event->exception)) {
      goto error;
    }

    // Write user info
    if (!bsg_write_user(json, &event->user)) {
      goto error;
    }

    // Write diagnostics
    if (!bsg_write_app(json, &event->app, payload->is_launching)) {
      goto error;
    }
    if (!bsg_write_device(json, &event->device)) {
      goto error;
    }
    if (!bsg_write_breadcrumbs(json, payload->breadcrumbs,
                               payload->breadcrumbs_size,
                               payload->max_breadcrumbs_size)) {
      goto error;
    }
    if (!bsg_write_feature_flags(json, payload->features)) {
      goto error;
    }
  }
  CHECKED(bsg_ksjsonendContainer(json));

  bsg_ksjsonendEncode(json);
  writer.dispose(&writer);
  return true;
error:
  writer.dispose(&writer);
  return false;
}

static bool bsg_write_metadata(BSG_KSJSONEncodeContext *json, char *metadata) {
  if (!metadata) {
    return true;
  }

  CHECKED(
      bsg_ksjsonaddJSONElement(json, "metaData", metadata, strlen(metadata)));
  return true;
error:
  return false;
}

static bool bsg_write_severity(BSG_KSJSONEncodeContext *json,
                               bsg_severity severity) {

  switch (severity) {
  case BSG_SEVERITY_ERR:
    CHECKED(JSON_CONSTANT_ELEMENT("severity", "error"));
    break;
  case BSG_SEVERITY_WARN:
    CHECKED(JSON_CONSTANT_ELEMENT("severity", "warning"));
    break;
  case BSG_SEVERITY_INFO:
    CHECKED(JSON_CONSTANT_ELEMENT("severity", "info"));
    break;
  }
  return true;
error:
  return false;
}

static bool bsg_write_severity_reason(BSG_KSJSONEncodeContext *json,
                                      bsg_event *event) {

  if (!bsg_write_severity(json, event->severity)) {
    goto error;
  }
  CHECKED(bsg_ksjsonaddBooleanElement(json, "unhandled", event->unhandled));

  CHECKED(bsg_ksjsonbeginObject(json, "severityReason"));
  {
    // unhandled == false always means that the state has been overridden by the
    // user, as this codepath is only executed for unhandled native errors
    CHECKED(bsg_ksjsonaddBooleanElement(json, "unhandledOverridden",
                                        !event->unhandled));
    CHECKED(JSON_CONSTANT_ELEMENT("type", "signal"));

    bsg_error *error = &event->exception;
    CHECKED(bsg_ksjsonbeginObject(json, "attributes"));
    { CHECKED(JSON_LIMITED_STRING_ELEMENT("signalType", error->error_class)); }
    CHECKED(bsg_ksjsonendContainer(json));
  }
  CHECKED(bsg_ksjsonendContainer(json));
  return true;
error:
  return false;
}

static char *bsg_uint64_to_prefixed_hex(uint64_t value) {
  char *hex_output_buffer = &hex_str[2];
  bsg_uint64_to_hex(value, hex_output_buffer, 1);
  return hex_str;
}

static bool bsg_write_stackframe(BSG_KSJSONEncodeContext *json,
                                 bsg_stackframe *frame, bool isPC) {

  CHECKED(bsg_ksjsonbeginObject(json, NULL));
  {
    CHECKED(JSON_LIMITED_STRING_ELEMENT(
        "frameAddress", bsg_uint64_to_prefixed_hex(frame->frame_address)));
    CHECKED(JSON_LIMITED_STRING_ELEMENT(
        "symbolAddress", bsg_uint64_to_prefixed_hex(frame->symbol_address)));
    CHECKED(JSON_LIMITED_STRING_ELEMENT(
        "loadAddress", bsg_uint64_to_prefixed_hex(frame->load_address)));

    CHECKED(
        bsg_ksjsonaddUIntegerElement(json, "lineNumber", frame->line_number));

    if (isPC) {
      CHECKED(bsg_ksjsonaddBooleanElement(json, "isPC", true));
    }

    if (string_is_not_empty(frame->filename)) {
      CHECKED(JSON_LIMITED_STRING_ELEMENT("file", frame->filename));
    }

    if (string_is_not_empty(frame->method)) {
      CHECKED(JSON_LIMITED_STRING_ELEMENT("method", frame->method));
    } else {
      CHECKED(JSON_LIMITED_STRING_ELEMENT(
          "method", bsg_uint64_to_prefixed_hex(frame->symbol_address)));
    }

    if (string_is_not_empty(frame->code_identifier)) {
      CHECKED(JSON_LIMITED_STRING_ELEMENT("codeIdentifier",
                                          frame->code_identifier));
    }
  }
  CHECKED(bsg_ksjsonendContainer(json));

  return true;
error:
  return false;
}

static bool bsg_write_stacktrace(BSG_KSJSONEncodeContext *json,
                                 bsg_stackframe *stacktrace,
                                 size_t frame_count) {

  for (int findex = 0; findex < frame_count; findex++) {
    if (!bsg_write_stackframe(json, &stacktrace[findex], findex == 0)) {
      goto error;
    }
  }

  return true;
error:
  return false;
}

static bool bsg_write_error(BSG_KSJSONEncodeContext *json, bsg_error *error) {
  CHECKED(bsg_ksjsonbeginArray(json, "exceptions"));
  {
    CHECKED(bsg_ksjsonbeginObject(json, NULL));
    {
      CHECKED(JSON_LIMITED_STRING_ELEMENT("errorClass", error->error_class));
      CHECKED(JSON_LIMITED_STRING_ELEMENT("message", error->error_message));
      CHECKED(JSON_CONSTANT_ELEMENT("type", "c"));

      const ssize_t frame_count = error->frame_count;
      // assuming that the initial frame is the program counter. This logic will
      // need to be revisited if (for example) we add more intelligent
      // processing for stack overflow-type errors, like discarding the top
      // frames, which would mean no stored frame is the program counter.
      if (frame_count > 0) {
        CHECKED(bsg_ksjsonbeginArray(json, "stacktrace"));
        {
          if (!bsg_write_stacktrace(json, error->stacktrace, frame_count)) {
            goto error;
          }
        }
        CHECKED(bsg_ksjsonendContainer(json));
      }
    }
    CHECKED(bsg_ksjsonendContainer(json));
  }
  CHECKED(bsg_ksjsonendContainer(json));

  return true;
error:
  return false;
}

static bool bsg_write_user(BSG_KSJSONEncodeContext *json, bsg_user *user) {
  const bool has_id = string_is_not_empty(user->id);
  const bool has_name = string_is_not_empty(user->name);
  const bool has_email = string_is_not_empty(user->email);

  const bool has_user = has_id || has_name || has_email;
  if (has_user) {
    CHECKED(bsg_ksjsonbeginObject(json, "user"));
    {
      if (has_id) {
        CHECKED(bsg_ksjsonaddStringElement(json, "id", user->id,
                                           BSG_KSJSON_SIZE_AUTOMATIC));
      }

      if (has_name) {
        CHECKED(bsg_ksjsonaddStringElement(json, "name", user->name,
                                           BSG_KSJSON_SIZE_AUTOMATIC));
      }

      if (has_email) {
        CHECKED(bsg_ksjsonaddStringElement(json, "email", user->email,
                                           BSG_KSJSON_SIZE_AUTOMATIC));
      }
    }
    CHECKED(bsg_ksjsonendContainer(json));
  }
  return true;
error:
  return false;
}

static bool bsg_write_app(BSG_KSJSONEncodeContext *json, bsg_app *app,
                          bool is_launching) {
  CHECKED(bsg_ksjsonbeginObject(json, "app"));
  {
    CHECKED(bsg_ksjsonaddStringElement(json, "version", app->version,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "id", app->code_bundle_id,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "type", app->type,
                                       BSG_KSJSON_SIZE_AUTOMATIC));

    CHECKED(bsg_ksjsonaddStringElement(json, "releaseStage", app->release_stage,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddIntegerElement(json, "duration", app->duration));
    CHECKED(bsg_ksjsonaddIntegerElement(json, "durationInForeground",
                                        app->duration_in_foreground));
    CHECKED(
        bsg_ksjsonaddBooleanElement(json, "inForeground", app->in_foreground));
    CHECKED(bsg_ksjsonaddBooleanElement(json, "isLaunching", is_launching));
  }
  CHECKED(bsg_ksjsonendContainer(json));
  return true;
error:
  return false;
}

static bool bsg_write_device(BSG_KSJSONEncodeContext *json,
                             bsg_device *device) {

  CHECKED(bsg_ksjsonbeginObject(json, "device"));
  {
    CHECKED(bsg_ksjsonaddStringElement(json, "osName", device->os_name,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "id", device->id,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "locale", device->locale,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "osVersion", device->os_version,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(
        json, "manufacturer", device->manufacturer, BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "model", device->model,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    CHECKED(bsg_ksjsonaddStringElement(json, "orientation", device->orientation,
                                       BSG_KSJSON_SIZE_AUTOMATIC));

    if (device->time > 0) {
      char buffer[sizeof "2018-10-08T12:07:09Z"];
      bsg_time_to_simplified_iso8601_string(device->time, buffer);
      CHECKED(JSON_LIMITED_STRING_ELEMENT("time", buffer));
    }
  }
  CHECKED(bsg_ksjsonendContainer(json));
  return true;
error:
  return false;
}

static bool bsg_write_breadcrumb_type(BSG_KSJSONEncodeContext *json,
                                      enum bsg_breadcrumb_type type) {

  switch (type) {
  case ERROR:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "error"));
    break;
  case LOG:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "log"));
    break;
  case MANUAL:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "manual"));
    break;
  case NAVIGATION:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "navigation"));
    break;
  case PROCESS:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "process"));
    break;
  case REQUEST:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "request"));
    break;
  case STATE:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "state"));
    break;
  case USER:
    CHECKED(JSON_CONSTANT_ELEMENT("type", "user"));
    break;
  }
  return true;
error:
  return false;
}

static bool bsg_write_breadcrumb(BSG_KSJSONEncodeContext *json,
                                 bsg_breadcrumb *breadcrumb) {
  if (!breadcrumb) {
    return true;
  }
  CHECKED(bsg_ksjsonbeginObject(json, NULL));
  {
    if (breadcrumb->timestamp > 0) {
      char buffer[sizeof "2018-10-08T12:07:09Z"];
      bsg_time_to_simplified_iso8601_string(breadcrumb->timestamp, buffer);
      CHECKED(JSON_LIMITED_STRING_ELEMENT("timestamp", buffer));
    }
    CHECKED(bsg_ksjsonaddStringElement(json, "name", breadcrumb->message,
                                       BSG_KSJSON_SIZE_AUTOMATIC));
    if (!bsg_write_breadcrumb_type(json, breadcrumb->type)) {
      goto error;
    }
    if (!bsg_write_metadata(json, breadcrumb->metadata)) {
      goto error;
    }
  }
  CHECKED(bsg_ksjsonendContainer(json));
  return true;
error:
  return false;
}

static bool bsg_write_breadcrumbs(BSG_KSJSONEncodeContext *json,
                                  bsg_breadcrumb **breadcrumbs, int crumb_count,
                                  int max_crumb_count) {
  CHECKED(bsg_ksjsonbeginArray(json, "breadcrumbs"));
  {
    for (int i = 0; i < crumb_count; ++i) {
      int crumb_index = i % max_crumb_count;
      if (!bsg_write_breadcrumb(json, breadcrumbs[crumb_index])) {
        goto error;
      }
    }
  }
  CHECKED(bsg_ksjsonendContainer(json));
  return true;
error:
  return false;
}

static bool bsg_write_feature_flags(BSG_KSJSONEncodeContext *json,
                                    char *features) {
  if (!features) {
    return true;
  }

  CHECKED(bsg_ksjsonbeginArray(json, "featureFlags"));
  { CHECKED(bsg_ksjsonaddRawJSONData(json, features, strlen(features))); }
  CHECKED(bsg_ksjsonendContainer(json));

  return true;
error:
  return false;
}
