#ifndef BUGSNAG_SAFESHAREDPTR_H
#define BUGSNAG_SAFESHAREDPTR_H

#include "./utils/bsg_reference_guard.h"

namespace bugsnag {

template <typename T, typename Deallocator = void (*)(T *)>
class SafeSharedPtr {
public:
  SafeSharedPtr() {
    this->deallocator = nullptr;
    this->guarded_ptr = new bsg_ref_guard;
    bsg_ref_guard_init(this->guarded_ptr, nullptr);
  }

  SafeSharedPtr(T *ptr, Deallocator *funcPtr = nullptr) {
    this->deallocator = funcPtr;
    this->guarded_ptr = new bsg_ref_guard;
    bsg_ref_guard_init(this->guarded_ptr, ptr);
  }

  ~SafeSharedPtr() {
    if (this->deallocator) {
      void *ptr = this->guarded_ptr->protected_ptr;
      T *casted = static_cast<T *>(ptr);
      (*this->deallocator)(casted);
    }
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
    bool res = bsg_ref_guard_release(this->guarded_ptr);
    if (res) {
      bsg_ref_guard_init(this->guarded_ptr, ptr);
    }

    return res;
  }

  T *acquire() {
    void *ptr = bsg_ref_guard_acquire(this->guarded_ptr);
    T *casted = static_cast<T *>(ptr);
    return casted;
  }

  bool release() { return bsg_ref_guard_release(this->guarded_ptr); }

private:
  bsg_ref_guard *guarded_ptr;
  Deallocator *deallocator;
};

} // namespace bugsnag

#endif // BUGSNAG_SAFESHAREDPTR_H