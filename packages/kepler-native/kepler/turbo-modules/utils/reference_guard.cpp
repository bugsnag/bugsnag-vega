#include <stdlib.h>

#include "reference_guard.h"

atomic_bool is_signal_handler_running(false);

// We are the first owner of ptr
void bsg_guarded_ptr_init(bsg_guarded_ptr *guard, void *ptr) {
  guard->protected_ptr = ptr;

  if (ptr == NULL) {
    atomic_init(&guard->counter, 0uLL);
  } else {
    atomic_init(&guard->counter, 1uLL);
  }
}

// We are the next owner of ptr, need to acquire officially
void bsg_guarded_ptr_init_copy(bsg_guarded_ptr *guard, bsg_guarded_ptr *other) {
  atomic_exchange(&guard->counter, other->counter);
  guard->protected_ptr = bsg_guarded_ptr_acquire(guard);
}

// Mark that pointer as being read/written, return ptr to use
void *bsg_guarded_ptr_acquire(bsg_guarded_ptr *guard) {
  uint_fast64_t current = atomic_load(&guard->counter);
  for (;;) {
    if (current == 0) {
      // released by another thread, so there is no valid ptr
      return NULL;
    }

    if (atomic_compare_exchange_strong(&guard->counter, &current,
                                       current + 1)) {
      break;
    }
  }
  return guard->protected_ptr;
}

// Release your mark on ptr, return true if it was properly freed
bool bsg_guarded_ptr_release(bsg_guarded_ptr *guard) {
  uint_fast64_t current = atomic_load(&guard->counter);
  for (;;) {
    if (current == 0) {
      // spurious extra release from somewhere
      // return and avoid an unsigned `0 - 1`
      return true;
    }

    if (atomic_compare_exchange_strong(&guard->counter, &current,
                                       current - 1)) {
      break;
    }
  }

  if (current == 1) {
    bool is_running = atomic_load(&is_signal_handler_running);
    if (is_running) {
      return false;
    }

    free(guard->protected_ptr);
    return true;
  }

  return false;
}
