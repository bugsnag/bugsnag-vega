#include "BugsnagBreadcrumbBuffer.h"

namespace bugsnag {

BreadcrumbBuffer::BreadcrumbBuffer(int maxBreadcrumbs)
    : maxBreadcrumbs{maxBreadcrumbs} {
  atomic_init(&this->index, 0);
  this->buffer = new SafeSharedPtr<bsg_breadcrumb>[maxBreadcrumbs];
}

BreadcrumbBuffer::~BreadcrumbBuffer() {
  if (this->buffer != nullptr) {
    for (int i = 0; i < this->maxBreadcrumbs; ++i) {
      // free char* fields in crumb
      free_breadcrumb_fields(this->buffer[i].get());
    }

    // safe shared_ptr destructor will take care of freeing crumb itself
    delete[] this->buffer;
  }
}

void BreadcrumbBuffer::add(bsg_breadcrumb_type type, std::string message,
                           std::string metadata, time_t timestamp) {
  if (this->maxBreadcrumbs == 0) {
    return;
  }

  bsg_breadcrumb *crumb =
      new_breadcrumb(type, message.c_str(), metadata.c_str(), timestamp);

  int position = getBreadcrumbIndex();
  this->buffer[position].reset(crumb);
}

int BreadcrumbBuffer::getBreadcrumbIndex() {
  while (true) {
    int current = atomic_load(&this->index);
    if (current == -1) {
      continue;
    }
    int next = (current + 1) % this->maxBreadcrumbs;
    if (atomic_compare_exchange_strong(&this->index, &current, next)) {
      return current;
    }
  }
}
} // namespace bugsnag