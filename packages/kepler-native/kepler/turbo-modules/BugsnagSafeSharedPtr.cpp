#include "BugsnagSafeSharedPtr.h"

namespace bugsnag {

template <typename T> SafeSharedPtr<T>::SafeSharedPtr() {
  bsg_guarded_ptr_init(this->guarded_ptr, nullptr);
}

template <typename T> SafeSharedPtr<T>::SafeSharedPtr(T *ptr) {
  bsg_guarded_ptr_init(this->guarded_ptr, ptr);
}

template <typename T> SafeSharedPtr<T>::~SafeSharedPtr() { this->release(); }

/** COPY OPERATIONS **/
template <typename T>
SafeSharedPtr<T>::SafeSharedPtr(const SafeSharedPtr &other) {
  // just copying underlying wrappers pointers, not their contents
  this->guarded_ptr = other->guarded_ptr;
}

template <typename T>
SafeSharedPtr<T> &SafeSharedPtr<T>::operator=(const SafeSharedPtr<T> &other) {
  // just copying underlying wrappers pointers, not their contents
  this->guarded_ptr = other->guarded_ptr;

  return *this;
}

template <typename T> bool SafeSharedPtr<T>::reset(T *ptr) {
  bool res = bsg_guarded_ptr_release(this->guarded_ptr);
  if (res) {
    bsg_guarded_ptr_init(this->guarded_ptr, ptr);
  }

  return res;
}

template <typename T> T *SafeSharedPtr<T>::acquire() {
  void *ptr = bsg_guarded_ptr_acquire(this->guarded_ptr);
  T *casted = static_cast<T *>(ptr);
  return casted;
}

template <typename T> bool SafeSharedPtr<T>::release() {
  return bsg_guarded_ptr_release(this->guarded_ptr);
}
} // namespace bugsnag