#include "BugsnagBreadcrumbBuffer.h"

namespace bugsnag {

BreadcrumbBuffer::BreadcrumbBuffer(int max_breadcrumbs)
    : max_breadcrumbs{max_breadcrumbs}, crumb_count{0} {
  atomic_init(&this->index, 0);

  // TODO Limit max breadcrumbs to BUGSNAG_CRUMBS_MAX
  this->buffer =
      new SignalSafePtr<bsg_breadcrumb,
                        decltype(free_breadcrumb_fields)>[max_breadcrumbs];
}

BreadcrumbBuffer::~BreadcrumbBuffer() {
  if (this->buffer == nullptr) {
    return;
  }
    // safe shared_ptr destructor will take care of freeing crumb itself
    delete[] this->buffer;
}

void BreadcrumbBuffer::add(bsg_breadcrumb_type type, std::string message,
                           std::string metadata, time_t timestamp) {
  if (this->max_breadcrumbs == 0) {
    return;
  }

  (this->crumb_count)++;
  if (this->crumb_count > this->max_breadcrumbs) {
    this->crumb_count = this->max_breadcrumbs;
  }

  bsg_breadcrumb *crumb =
      new_breadcrumb(type, message.c_str(), metadata.c_str(), timestamp);

  int position = get_breadcrumb_index();
  this->buffer[position].reset(crumb);
}

void BreadcrumbBuffer::fill_buffer(bsg_breadcrumb **crumb_buffer) {
  for (int i = 0; i < this->max_breadcrumbs; ++i) {
    crumb_buffer[i] = this->buffer[i].acquire();
  }
}

int BreadcrumbBuffer::get_buffer_max_size() { return this->max_breadcrumbs; }

int BreadcrumbBuffer::get_buffer_filled_count() { return this->crumb_count; }

int BreadcrumbBuffer::get_breadcrumb_index() {
  while (true) {
    int current = atomic_load(&this->index);
    if (current == -1) {
      continue;
    }
    int next = (current + 1) % this->max_breadcrumbs;
    if (atomic_compare_exchange_strong(&this->index, &current, next)) {
      return current;
    }
  }
}
} // namespace bugsnag