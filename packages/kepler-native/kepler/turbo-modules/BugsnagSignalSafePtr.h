#ifndef BUGSNAG_SIGNALSAFEPTR_H
#define BUGSNAG_SIGNALSAFEPTR_H

#include "./utils/bsg_reference_guard.h"

namespace bugsnag {

template <typename T, typename Deallocator = void (*)(T *)>
class SignalSafePtr {
public:
  SignalSafePtr() {
    this->deallocator = nullptr;
    this->ref_guard = new bsg_ref_guard;
    bsg_ref_guard_init(this->ref_guard, nullptr);
  }

  SignalSafePtr(T *ptr, Deallocator *funcPtr = nullptr) {
    this->deallocator = funcPtr;
    this->ref_guard = new bsg_ref_guard;
    bsg_ref_guard_init(this->ref_guard, ptr);
  }

  ~SignalSafePtr() {
    if (this->deallocator) {
      void *ptr = (void *)atomic_load(&this->ref_guard->protected_ptr);
      T *casted = static_cast<T *>(ptr);
      (*this->deallocator)(casted);
    }
    this->release();
    delete this->ref_guard;
  }

  SignalSafePtr(const SignalSafePtr &other) {
    // just copying underlying wrappers pointers, not their contents
    this->ref_guard = other->ref_guard;
  }

  SignalSafePtr &operator=(const SignalSafePtr &other) {
    // just copying underlying wrappers pointers, not their contents
    this->ref_guard = other->ref_guard;

    return *this;
  }

  SignalSafePtr(SignalSafePtr &&other) = delete;
  SignalSafePtr &operator=(SignalSafePtr &&other) = delete;

  bool reset(T *ptr) {
    bool res = bsg_ref_guard_release(this->ref_guard);
    if (res) {
      bsg_ref_guard_init(this->ref_guard, ptr);
    }

    return res;
  }

  T *acquire() {
    void *ptr = bsg_ref_guard_acquire(this->ref_guard);
    T *casted = static_cast<T *>(ptr);
    return casted;
  }

  T *move() {
    void *ptr = bsg_ref_guard_move(this->ref_guard);
    T *casted = static_cast<T *>(ptr);
    return casted;
  }

  bool release() { return bsg_ref_guard_release(this->ref_guard); }

private:
  bsg_ref_guard *ref_guard;
  Deallocator *deallocator;
};

} // namespace bugsnag

#endif // BUGSNAG_SIGNALSAFEPTR_H