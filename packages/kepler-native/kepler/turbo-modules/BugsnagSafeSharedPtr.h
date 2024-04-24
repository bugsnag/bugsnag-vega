#ifndef BUGSNAG_SAFESHAREDPTR_H
#define BUGSNAG_SAFESHAREDPTR_H

#include "./utils/reference_guard.h"

namespace bugsnag {

template <typename T> class SafeSharedPtr {
public:
  SafeSharedPtr() {
    this->guarded_ptr = new bsg_guarded_ptr;
    bsg_guarded_ptr_init(this->guarded_ptr, nullptr);
  }

  SafeSharedPtr(T *ptr) {
    this->guarded_ptr = new bsg_guarded_ptr;
    bsg_guarded_ptr_init(this->guarded_ptr, ptr);
  }

  ~SafeSharedPtr() {
    this->release();
    delete this->guarded_ptr;
  }

  SafeSharedPtr(const SafeSharedPtr &other) {
    // just copying underlying wrappers pointers, not their contents
    this->guarded_ptr = other->guarded_ptr;
  }

  SafeSharedPtr &operator=(const SafeSharedPtr &other) {
    // just copying underlying wrappers pointers, not their contents
    this->guarded_ptr = other->guarded_ptr;

    return *this;
  }

  SafeSharedPtr(SafeSharedPtr &&other) = delete;
  SafeSharedPtr &operator=(SafeSharedPtr &&other) = delete;

  bool reset(T *ptr) {
    bool res = bsg_guarded_ptr_release(this->guarded_ptr);
    if (res) {
      bsg_guarded_ptr_init(this->guarded_ptr, ptr);
    }

    return res;
  }

  T *acquire() {
    void *ptr = bsg_guarded_ptr_acquire(this->guarded_ptr);
    T *casted = static_cast<T *>(ptr);
    return casted;
  }

  bool release() { return bsg_guarded_ptr_release(this->guarded_ptr); }

private:
  bsg_guarded_ptr *guarded_ptr;
};

} // namespace bugsnag

#endif // BUGSNAG_SAFESHAREDPTR_H