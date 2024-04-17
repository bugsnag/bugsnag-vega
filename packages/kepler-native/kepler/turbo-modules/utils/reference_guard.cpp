#include <stdlib.h>

#include "reference_guard.h"

atomic_bool is_signal_handler_running(false);

void bsg_guarded_ptr_init(bsg_guarded_ptr *guard, void *ptr) {
  guard->protected_ptr = ptr;

  if (ptr == NULL) {
    atomic_init(guard->counter, 0uLL);
  } else {
    atomic_init(guard->counter, 1uLL);
  }
}

void bsg_guarded_ptr_init_copy(bsg_guarded_ptr *guard, bsg_guarded_ptr *other) {
  atomic_exchange(guard->counter, *other->counter);
  guard->protected_ptr = other->protected_ptr;

  uint_fast64_t current = atomic_load(guard->counter);
  atomic_compare_exchange_strong(guard->counter, &current, current + 1);
}

void bsg_guarded_ptr_init_move(bsg_guarded_ptr *guard, bsg_guarded_ptr *other) {
  atomic_exchange(guard->counter, *other->counter);
  guard->protected_ptr = other->protected_ptr;
  atomic_store(other->counter, NULL);
  other->protected_ptr = NULL;
}

void *bsg_guarded_ptr_acquire(bsg_guarded_ptr *guard) {
  return guard->protected_ptr;
}

void bsg_guarded_ptr_release(bsg_guarded_ptr *guard) {
  uint_fast64_t current = atomic_load(guard->counter);
  atomic_compare_exchange_strong(guard->counter, &current, current - 1);
  current = atomic_load(guard->counter);

  if (current == 0) {
    bool is_running = atomic_load(&is_signal_handler_running);
    if (is_running) {
      return;
    }

    free(guard->protected_ptr);
  }
}
