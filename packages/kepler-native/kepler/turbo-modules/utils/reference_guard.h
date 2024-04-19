#ifndef BUGSNAG_REFGUARD_H
#define BUGSNAG_REFGUARD_H

#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  atomic_uint_fast64_t counter;
  void *protected_ptr;
} bsg_guarded_ptr;

extern atomic_bool is_signal_handler_running;

void bsg_guarded_ptr_init(bsg_guarded_ptr *guard, void *ptr);
void bsg_guarded_ptr_init_copy(bsg_guarded_ptr *guard, bsg_guarded_ptr *other);
void *bsg_guarded_ptr_acquire(bsg_guarded_ptr *guard);
bool bsg_guarded_ptr_release(bsg_guarded_ptr *guard);

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_REFGUARD_H
