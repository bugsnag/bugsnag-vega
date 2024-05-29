#include <stdbool.h>
#include <stdlib.h>

#include "bsg_reference_guard.h"

atomic_bool is_signal_handler_running = false;

// We are the first owner of ptr
void bsg_ref_guard_init(bsg_ref_guard *guard, void *ptr) {
  if (ptr == NULL) {
    atomic_init(&guard->counter, 0uLL);
    atomic_init(&guard->protected_ptr, 0uL);
  } else {
    atomic_init(&guard->counter, 1uLL);
    atomic_init(&guard->protected_ptr, (uintptr_t)ptr);
  }
}

// We are the next owner of ptr, need to acquire officially
void bsg_ref_guard_init_copy(bsg_ref_guard *guard, bsg_ref_guard *other) {
  atomic_exchange(&guard->counter, other->counter);
  void *received = bsg_ref_guard_acquire(guard);
  atomic_exchange(&guard->protected_ptr, (uintptr_t)received);
}

// Mark that pointer as being read/written, return ptr to use
void *bsg_ref_guard_acquire(bsg_ref_guard *guard) {
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
  void *ptr = (void *)atomic_load(&guard->protected_ptr);
  return ptr;
}

void *bsg_ref_guard_move(bsg_ref_guard *guard) {
  uint_fast64_t current = atomic_load(&guard->counter);
  for (;;) {
    if (current == 0) {
      // released by another thread, so there is no valid ptr
      return NULL;
    }

    if (atomic_compare_exchange_strong(&guard->counter, &current, 0)) {
      break;
    }
  }

  void *ptr = (void *)atomic_exchange(&guard->protected_ptr, 0uL);
  return ptr;
}

// Release your mark on ptr, return true if it was properly freed
bool bsg_ref_guard_release(bsg_ref_guard *guard) {
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

    void *ptr = (void *)atomic_load(&guard->protected_ptr);
    free(ptr);
    return true;
  }

  return false;
}
