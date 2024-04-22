#include <stdlib.h>
#include <string.h>

#include "bsg_breadcrumb.h"

bsg_breadcrumb *new_breadcrumb(bsg_breadcrumb_type type, const char *message,
                               const char *metadata, time_t timestamp) {
  bsg_breadcrumb *crumb = (bsg_breadcrumb *)calloc(1, sizeof(bsg_breadcrumb));
  strcpy(crumb->message, message);
  strcpy(crumb->metadata, metadata);
  crumb->type = type;
  crumb->timestamp = timestamp;

  return crumb;
}
