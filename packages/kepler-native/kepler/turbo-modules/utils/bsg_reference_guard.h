#ifndef BUGSNAG_REFGUARD_H
#define BUGSNAG_REFGUARD_H

#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  atomic_uint_fast64_t counter;
  void *protected_ptr;
} bsg_ref_guard;

extern atomic_bool is_signal_handler_running;

void bsg_ref_guard_init(bsg_ref_guard *guard, void *ptr);
void bsg_ref_guard_init_copy(bsg_ref_guard *guard, bsg_ref_guard *other);
void *bsg_ref_guard_acquire(bsg_ref_guard *guard);
bool bsg_ref_guard_release(bsg_ref_guard *guard);

#ifdef __cplusplus
}
#endif

#endif // BUGSNAG_REFGUARD_H
