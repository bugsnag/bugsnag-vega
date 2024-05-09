#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bsg_event.h"

bsg_event_payload *bsg_new_event_payload(const char *api_key,
                                         const char *payload_ver) {
  bsg_event_payload *new_payload =
      (bsg_event_payload *)calloc(1, sizeof(bsg_event_payload));

  if (api_key != NULL) {
    new_payload->api_key = strdup(api_key);
  }
  if (payload_ver != NULL) {
    new_payload->payload_version = strdup(payload_ver);
  }

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

  if (name_arg != NULL) {
    payload->notifier.name = strdup(name_arg);
  }
  if (ver_arg != NULL) {
    payload->notifier.version = strdup(ver_arg);
  }
  if (url_arg != NULL) {
    payload->notifier.url = strdup(url_arg);
  }
}

void bsg_set_event_severity(bsg_event_payload *payload, const char *type_arg) {
  if (payload == NULL) {
    return;
  }

  if (type_arg != NULL) {
    payload->event.severity_reason.type = strdup(type_arg);
  }
}

void bsg_set_event_user(bsg_event_payload *payload, const char *id_arg,
                        const char *email_arg, const char *name_arg) {
  if (payload == NULL) {
    return;
  }

  if (id_arg != NULL) {
    payload->event.user.id = strdup(id_arg);
  }
  if (email_arg != NULL) {
    payload->event.user.email = strdup(email_arg);
  }
  if (name_arg != NULL) {
    payload->event.user.name = strdup(name_arg);
  }
}

void bsg_set_event_device(bsg_event_payload *payload, const char *id,
                          const char *host, const char *locale,
                          const char *manufacturer, const char *model,
                          const char *modelnr, const char *orient,
                          const char *osname, const char *osver) {

  if (payload == NULL) {
    return;
  }
  if (id != NULL) {
    payload->event.device.id = strdup(id);
  }
  if (host != NULL) {
    payload->event.device.hostname = strdup(host);
  }
  if (locale != NULL) {
    payload->event.device.locale = strdup(locale);
  }
  if (manufacturer != NULL) {
    payload->event.device.manufacturer = strdup(manufacturer);
  }
  if (model != NULL) {
    payload->event.device.model = strdup(model);
  }
  if (modelnr != NULL) {
    payload->event.device.model_number = strdup(modelnr);
  }
  if (orient != NULL) {
    payload->event.device.orientation = strdup(orient);
  }
  if (osname != NULL) {
    payload->event.device.os_name = strdup(osname);
  }
  if (osver != NULL) {
    payload->event.device.os_version = strdup(osver);
  }
}

void bsg_set_event_device_time(bsg_event_payload *payload, time_t time_arg) {
  if (payload == NULL) {
    return;
  }
  payload->event.device.time = time_arg;
}

void bsg_set_event_app(bsg_event_payload *payload, const char *bundle_id,
                       const char *stage, const char *type, const char *ver) {
  if (payload == NULL) {
    return;
  }

  if (bundle_id != NULL) {
    payload->event.app.code_bundle_id = strdup(bundle_id);
  }
  if (stage != NULL) {
    payload->event.app.release_stage = strdup(stage);
  }
  if (type != NULL) {
    payload->event.app.type = strdup(type);
  }
  if (ver != NULL) {
    payload->event.app.version = strdup(ver);
  }
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

void bsg_free_event_app(bsg_app *app) {
  if (app == NULL) {
    return;
  }

  free(app->code_bundle_id);
  free(app->release_stage);
  free(app->type);
  free(app->version);
}

void bsg_free_event_device(bsg_device *device) {
  if (device == NULL) {
    return;
  }

  free(device->id);
  free(device->hostname);
  free(device->locale);
  free(device->manufacturer);
  free(device->model);
  free(device->model_number);
  free(device->orientation);
  free(device->os_name);
  free(device->os_version);
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

size_t bsg_strncpy(char *dst, const char *src, size_t dst_size) {
  if (dst == NULL || dst_size == 0) {
    return 0;
  }
  dst[0] = '\0';
  if (src != NULL) {
    return strlcat(dst, src, dst_size);
  }
  return 0;
}