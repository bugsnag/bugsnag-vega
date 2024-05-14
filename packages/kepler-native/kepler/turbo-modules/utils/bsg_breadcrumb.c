#include <stdlib.h>
#include <string.h>

#include "bsg_breadcrumb.h"

bsg_breadcrumb *new_breadcrumb(enum bsg_breadcrumb_type type,
                               const char *message, const char *metadata,
                               time_t timestamp) {
  bsg_breadcrumb *crumb = (bsg_breadcrumb *)calloc(1, sizeof(bsg_breadcrumb));
  crumb->message = strdup(message);
  crumb->metadata = strdup(metadata);
  crumb->type = type;
  crumb->timestamp = timestamp;

  return crumb;
}

void free_breadcrumb_fields(bsg_breadcrumb *crumb) {
  if (crumb == NULL) {
    return;
  }

  if (crumb->message != NULL) {
    free(crumb->message);
  }
  if (crumb->metadata != NULL) {
    free(crumb->metadata);
  }
}
