#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bsg_event.h"

bsg_event_payload *bsg_new_event_payload(const char *api_key,
                                         const char *event_dir,
                                         const char *payload_ver) {
  bsg_event_payload *new_payload =
      (bsg_event_payload *)calloc(1, sizeof(bsg_event_payload));

  bsg_free_and_strdup(&(new_payload->api_key), api_key);
  bsg_free_and_strdup(&(new_payload->event_path), event_dir);
  bsg_free_and_strdup(&(new_payload->payload_version), payload_ver);

  new_payload->event.severity = BSG_SEVERITY_ERR;
  new_payload->event.unhandled = true;
  return new_payload;
}

void bsg_set_event_notifier_info(bsg_event_payload *payload,
                                 const char *name_arg, const char *ver_arg,
                                 const char *url_arg) {
  if (payload == NULL) {
    return;
  }

  bsg_free_and_strdup(&(payload->notifier.name), name_arg);
  bsg_free_and_strdup(&(payload->notifier.version), ver_arg);
  bsg_free_and_strdup(&(payload->notifier.url), url_arg);
}

void bsg_set_event_severity(bsg_event_payload *payload, const char *type_arg) {
  if (payload == NULL) {
    return;
  }

  bsg_free_and_strdup(&(payload->event.severity_reason.type), type_arg);
}

void bsg_set_event_user(bsg_event_payload *payload, const char *id_arg,
                        const char *email_arg, const char *name_arg) {
  if (payload == NULL) {
    return;
  }

  bsg_free_and_strdup(&(payload->event.user.id), id_arg);
  bsg_free_and_strdup(&(payload->event.user.email), email_arg);
  bsg_free_and_strdup(&(payload->event.user.name), name_arg);
}

void bsg_set_event_device(bsg_event_payload *payload,
                          const char *manufacturer, const char *model,
                          const char *osname, const char *osver,
                          const char *rnver, const char *js_engine) {
  if (payload == NULL) {
    return;
  }

  bsg_free_and_strdup(&(payload->event.device.manufacturer), manufacturer);
  bsg_free_and_strdup(&(payload->event.device.model), model);
  bsg_free_and_strdup(&(payload->event.device.os_name), osname);
  bsg_free_and_strdup(&(payload->event.device.os_version), osver);
  bsg_free_and_strdup(&(payload->event.device.rn_version), rnver);
  bsg_free_and_strdup(&(payload->event.device.js_engine), js_engine);
}

void bsg_set_event_device_time(bsg_event_payload *payload, time_t time_arg) {
  if (payload == NULL) {
    return;
  }
  payload->event.device.time = time_arg;
}

void bsg_set_event_device_id(bsg_event_payload *payload, const char *id) {
  if (payload == NULL) {
    return;
  }
  bsg_free_and_strdup(&(payload->event.device.id), id);
}

void bsg_set_event_app(bsg_event_payload *payload, const char *id,
                       const char *stage, const char *type, const char *ver, const char *binary_arch) {
  if (payload == NULL) {
    return;
  }

  bsg_free_and_strdup(&(payload->event.app.id), id);
  bsg_free_and_strdup(&(payload->event.app.release_stage), stage);
  bsg_free_and_strdup(&(payload->event.app.type), type);
  bsg_free_and_strdup(&(payload->event.app.version), ver);
  bsg_free_and_strdup(&(payload->event.app.binary_arch), binary_arch);
}

void bsg_set_event_app_duration(bsg_event_payload *payload, int duration,
                                int duration_foreground, bool in_foreground) {
  if (payload == NULL) {
    return;
  }

  payload->event.app.duration = duration;
  payload->event.app.duration_in_foreground = duration_foreground;
  payload->event.app.in_foreground = in_foreground;
}

void bsg_set_event_exception(bsg_event_payload *payload, const char *class_arg,
                             const char *message_arg, const char *type_arg) {
  if (payload == NULL) {
    return;
  }

  if (class_arg != NULL) {
    bsg_strncpy(payload->event.exception.error_class, class_arg,
                ERROR_CLASS_SIZE);
  }
  if (message_arg != NULL) {
    bsg_strncpy(payload->event.exception.error_message, message_arg,
                ERROR_MESSAGE_SIZE);
  }
  if (type_arg != NULL) {
    bsg_strncpy(payload->event.exception.type, type_arg, ERROR_TYPE_SIZE);
  }
}

bsg_stackframe *bsg_get_event_stacktrace(bsg_event_payload *payload) {
  if (payload == NULL) {
    return NULL;
  }
  return payload->event.exception.stacktrace;
}

void bsg_set_event_stacktrace_size(bsg_event_payload *payload, int size) {
  if (payload == NULL) {
    return;
  }
  payload->event.exception.frame_count = size;
}

void bsg_set_event_is_launching(bsg_event_payload *payload, bool is_launching) {
  payload->is_launching = is_launching;
}

void bsg_set_notifier_start_time(bsg_event_payload *payload, time_t time_arg) {
  payload->start_time = time_arg;
}

void bsg_free_event_app(bsg_app *app) {
  if (app == NULL) {
    return;
  }

  free(app->id);
  free(app->release_stage);
  free(app->type);
  free(app->version);
  free(app->binary_arch);
}

void bsg_free_event_device(bsg_device *device) {
  if (device == NULL) {
    return;
  }

  free(device->id);
  free(device->manufacturer);
  free(device->model);
  free(device->os_name);
  free(device->os_version);
  free(device->rn_version);
  free(device->js_engine);
}

void bsg_free_event_user(bsg_user *user) {
  if (user == NULL) {
    return;
  }

  free(user->id);
  free(user->email);
  free(user->name);
}

void bsg_free_event_severity(bsg_event_severity_reason *reason) {
  if (reason == NULL) {
    return;
  }

  free(reason->type);
}

void bsg_free_event_notifier_info(bsg_notifier_info *info) {
  if (info == NULL) {
    return;
  }

  free(info->name);
  free(info->version);
  free(info->url);
}

void bsg_free_event_payload(bsg_event_payload *payload) {
  if (payload == NULL) {
    return;
  }

  bsg_free_event_app(&(payload->event.app));
  bsg_free_event_device(&(payload->event.device));
  bsg_free_event_notifier_info(&(payload->notifier));
  bsg_free_event_severity(&(payload->event.severity_reason));
  bsg_free_event_user(&(payload->event.user));

  free(payload->api_key);
  free(payload->payload_version);
  free(payload);
}

bsg_error_types bsg_error_types_all_enabled() {
  bsg_error_types et;
  et.anrs = true;
  et.app_hangs = true;
  et.cpp_exceptions = true;
  et.mach_exceptions = true;
  et.ndk_crashes = true;
  et.ooms = true;
  et.signals = true;
  et.unhandled_exceptions = true;
  et.unhandled_rejections = true;
  return et;
}

/*
 * Copy null terminated c-string from src to dst
 * dst_size should include null-terminator
 */
void bsg_strncpy(char *dst, const char *src, size_t dst_size) {
  if (dst == NULL || dst_size == 0) {
    return;
  }
  dst[0] = '\0';
  if (src != NULL) {
    // strncat is dangerous because it actually copies dst_size + 1
    // if src_length >= dst_size
    // we do -1 to make sure null terminator
    // will be appended safely to resulting string
    strncat(dst, src, dst_size - 1);
  }
}

void bsg_free_and_strdup(char **dst, const char *src) {
  free(*dst);
  *dst = src != NULL ? strdup(src) : NULL;
}