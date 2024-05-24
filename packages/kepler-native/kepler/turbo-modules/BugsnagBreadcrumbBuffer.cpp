#include "BugsnagBreadcrumbBuffer.h"

namespace bugsnag {

BreadcrumbBuffer::BreadcrumbBuffer(int max_breadcrumbs)
    : max_breadcrumbs{max_breadcrumbs} {
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

  bsg_breadcrumb *crumb =
      new_breadcrumb(type, message.c_str(), metadata.c_str(), timestamp);

  int position = get_breadcrumb_index();
  this->buffer[position].reset(crumb);
}

int BreadcrumbBuffer::fill_buffer(bsg_breadcrumb **crumb_buffer,
                                  int crumb_buffer_size) {
  int crumb_copy_count = crumb_buffer_size;

  // lock access to buffer before getting pointers
  int last_idx_value = -1;
  while (last_idx_value == -1) {
    last_idx_value = atomic_exchange(&this->index, -1);
  }

  for (int i = 0; i < crumb_buffer_size; ++i) {
    bsg_breadcrumb *ptr = this->buffer[i].move();
    if (ptr == NULL) {
      crumb_copy_count = i;
      break;
    }
    crumb_buffer[i] = ptr;
  }

  // return index to previous value
  atomic_exchange(&this->index, last_idx_value);

  return crumb_copy_count;
}

int BreadcrumbBuffer::get_buffer_max_size() { return this->max_breadcrumbs; }

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