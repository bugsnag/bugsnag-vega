#include <stdlib.h>
#include <string.h>

#include "bsg_event.h"

bsg_event_payload *bsg_new_event_payload(const char *api_key,
                                         const char *payload_ver) {
  bsg_event_payload *new_payload =
      (bsg_event_payload *)calloc(1, sizeof(bsg_event_payload));
  new_payload->api_key = strdup(api_key);
  new_payload->payload_version = strdup(payload_ver);
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

  payload->notifier.name = strdup(name_arg);
  payload->notifier.version = strdup(ver_arg);
  payload->notifier.url = strdup(url_arg);
}

void bsg_set_event_severity(bsg_event_payload *payload, const char *type_arg) {
  if (payload == NULL) {
    return;
  }

  payload->event.severity_reason.type = strdup(type_arg);
}

void bsg_set_event_user(bsg_event_payload *payload, const char *id_arg,
                        const char *email_arg, const char *name_arg) {
  if (payload == NULL) {
    return;
  }

  payload->event.user.id = strdup(id_arg);
  payload->event.user.email = strdup(email_arg);
  payload->event.user.name = strdup(name_arg);
}

void bsg_set_event_device(bsg_event_payload *payload, const char *id,
                          const char *host, const char *locale,
                          const char *manufacturer, const char *model,
                          const char *modelnr, const char *orient,
                          const char *osname, const char *osver) {

  if (payload == NULL) {
    return;
  }
  payload->event.device.id = strdup(id);
  payload->event.device.hostname = strdup(host);
  payload->event.device.locale = strdup(locale);
  payload->event.device.manufacturer = strdup(manufacturer);
  payload->event.device.model = strdup(model);
  payload->event.device.model_number = strdup(modelnr);
  payload->event.device.orientation = strdup(orient);
  payload->event.device.os_name = strdup(osname);
  payload->event.device.os_version = strdup(osver);
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

  payload->event.app.code_bundle_id = strdup(bundle_id);
  payload->event.app.release_stage = strdup(stage);
  payload->event.app.type = strdup(type);
  payload->event.app.version = strdup(ver);
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

  int class_len = strlen(class_arg);
  if (class_len > (ERROR_CLASS_SIZE - 1)) {
    class_len = ERROR_CLASS_SIZE - 1;
  }
  int i = 0;
  for (i = 0; i < class_len; ++i) {
    payload->event.exception.error_class[i] = class_arg[i];
  }
  payload->event.exception.error_class[i] = '\0';

  int msg_len = strlen(message_arg);
  if (msg_len > (ERROR_MESSAGE_SIZE - 1)) {
    msg_len = ERROR_MESSAGE_SIZE - 1;
  }
  for (i = 0; i < msg_len; ++i) {
    payload->event.exception.error_message[i] = message_arg[i];
  }
  payload->event.exception.error_message[i] = '\0';

  int type_len = strlen(type_arg);
  if (type_len > (ERROR_TYPE_SIZE - 1)) {
    type_len = ERROR_TYPE_SIZE - 1;
  }
  for (i = 0; i < type_len; ++i) {
    payload->event.exception.type[i] = type_arg[i];
  }
  payload->event.exception.type[i] = '\0';
}

void bsg_free_event_app(bsg_app *app) {
  if (app == NULL) {
    return;
  }

  if (app->code_bundle_id != NULL)
    free(app->code_bundle_id);
  if (app->release_stage != NULL)
    free(app->release_stage);
  if (app->type != NULL)
    free(app->type);
  if (app->version != NULL)
    free(app->version);
}

void bsg_free_event_device(bsg_device *device) {
  if (device == NULL) {
    return;
  }

  if (device->id != NULL)
    free(device->id);
  if (device->hostname != NULL)
    free(device->hostname);
  if (device->locale != NULL)
    free(device->locale);
  if (device->manufacturer != NULL)
    free(device->manufacturer);
  if (device->model != NULL)
    free(device->model);
  if (device->model_number != NULL)
    free(device->model_number);
  if (device->orientation != NULL)
    free(device->orientation);
  if (device->os_name != NULL)
    free(device->os_name);
  if (device->os_version != NULL)
    free(device->os_version);
}

void bsg_free_event_user(bsg_user *user) {
  if (user == NULL) {
    return;
  }

  if (user->id != NULL)
    free(user->id);
  if (user->email != NULL)
    free(user->email);
  if (user->name != NULL)
    free(user->name);
}

void bsg_free_event_severity(bsg_event_severity_reason *reason) {
  if (reason == NULL) {
    return;
  }

  if (reason->type != NULL)
    free(reason->type);
}

void bsg_free_event_notifier_info(bsg_notifier_info *info) {
  if (info == NULL) {
    return;
  }

  if (info->name != NULL)
    free(info->name);
  if (info->version != NULL)
    free(info->version);
  if (info->url != NULL)
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