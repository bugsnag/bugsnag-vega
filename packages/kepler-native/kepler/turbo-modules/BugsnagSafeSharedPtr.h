#ifndef BUGSNAG_SAFESHAREDPTR_H
#define BUGSNAG_SAFESHAREDPTR_H

#include "./utils/reference_guard.h"

namespace bugsnag {

template <typename T> class SafeSharedPtr {
public:
  SafeSharedPtr();
  SafeSharedPtr(T *ptr);
  ~SafeSharedPtr();
  // copy constructor/operator
  SafeSharedPtr(const SafeSharedPtr &other);
  SafeSharedPtr &operator=(const SafeSharedPtr &other);
  // move constructor/operator
  SafeSharedPtr(SafeSharedPtr &&other) = delete;
  SafeSharedPtr &operator=(SafeSharedPtr &&other) = delete;

  bool reset(T *ptr);
  T *acquire();
  bool release();

private:
  bsg_guarded_ptr *guarded_ptr;
};

} // namespace bugsnag

#endif // BUGSNAG_SAFESHAREDPTR_H