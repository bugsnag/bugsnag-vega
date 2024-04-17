#include "BugsnagSafeSharedPtr.h"

namespace bugsnag {

template <typename T> SafeSharedPtr<T>::SafeSharedPtr() {
  bsg_guarded_ptr_init(this->guarded_ptr, nullptr);
}

template <typename T> SafeSharedPtr<T>::SafeSharedPtr(T *ptr) {
  bsg_guarded_ptr_init(this->guarded_ptr, ptr);
}

template <typename T> SafeSharedPtr<T>::~SafeSharedPtr() { this->releasePtr(); }

/** COPY OPERATIONS **/
template <typename T>
SafeSharedPtr<T>::SafeSharedPtr(const SafeSharedPtr &other) {
  bsg_guarded_ptr_init_from(this->guarded_ptr, other->guarded_ptr);
}

template <typename T>
SafeSharedPtr<T> &SafeSharedPtr<T>::operator=(const SafeSharedPtr<T> &other) {
  if (this != &other) {
    this->releasePtr();

    bsg_guarded_ptr_init_from(this->guarded_ptr, other->guarded_ptr);
  }

  return *this;
}

/** MOVE OPERATIONS **/
template <typename T> SafeSharedPtr<T>::SafeSharedPtr(SafeSharedPtr &&other) {
  bsg_guarded_ptr_init_move(this->guarded_ptr, other->guarded_ptr);
}

template <typename T>
SafeSharedPtr<T> &SafeSharedPtr<T>::operator=(SafeSharedPtr<T> &&other) {
  if (this != &other) {
    this->releasePtr();

    bsg_guarded_ptr_init_move(this->guarded_ptr, other->guarded_ptr);
  }

  return *this;
}

template <typename T> T *SafeSharedPtr<T>::get() {
  void *ptr = bsg_guarded_ptr_acquire(this->guarded_ptr);
  T *casted = static_cast<T *>(ptr);
  return casted;
}

template <typename T> void SafeSharedPtr<T>::releasePtr() {
  bsg_guarded_ptr_release(this->guarded_ptr);
}
} // namespace bugsnag