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
  BreadcrumbBuffer(int maxBreadcrumbs);
  ~BreadcrumbBuffer();

  void add(bsg_breadcrumb_type type, std::string message, std::string metadata,
           time_t timestamp);
  // bsg_breadcrumb *copy();

private:
  int getBreadcrumbIndex();

  SignalSafePtr<bsg_breadcrumb, decltype(free_breadcrumb_fields)> *buffer;
  atomic_int index;
  int maxBreadcrumbs;
};

} // namespace bugsnag

#endif // BUGSNAG_BREADCRUMBBUFF_H