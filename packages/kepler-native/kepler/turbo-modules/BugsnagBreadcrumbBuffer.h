#ifndef BUGSNAG_BREADCRUMBBUFF_H
#define BUGSNAG_BREADCRUMBBUFF_H

#include <string>

#include "./utils/bsg_breadcrumb.h"
#include "BugsnagSignalSafePtr.h"

#ifndef BUGSNAG_CRUMBS_MAX
// Max number of breadcrumbs in an event. Configures a default if not defined.
#define BUGSNAG_CRUMBS_MAX 50
#endif

namespace bugsnag {

class BreadcrumbBuffer {
public:
  BreadcrumbBuffer(int max_breadcrumbs);
  ~BreadcrumbBuffer();

  void add(bsg_breadcrumb_type type, std::string message, std::string metadata,
           time_t timestamp);

  void fill_buffer(bsg_breadcrumb **crumb_buffer, int crumb_buffer_size);
  int get_buffer_max_size();
  int get_buffer_filled_count();

private:
  int get_breadcrumb_index();

  SignalSafePtr<bsg_breadcrumb, decltype(free_breadcrumb_fields)> *buffer;
  atomic_int index;
  int max_breadcrumbs;
  int crumb_count;
};

} // namespace bugsnag

#endif // BUGSNAG_BREADCRUMBBUFF_H