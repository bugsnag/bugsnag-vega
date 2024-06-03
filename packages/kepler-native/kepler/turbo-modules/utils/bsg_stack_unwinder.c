#include <execinfo.h>
#include <stdio.h>

#define __USE_GNU
#include <dlfcn.h>

#include "bsg_stack_unwinder.h"

void bsg_fill_stack_info(bsg_stackframe *error_stacktrace, int *frames_count) {
  if (error_stacktrace == NULL) {
    return;
  }

  void *stacktrace[BSG_MAX_STACK_FRAMES];
  Dl_info sym_info;
  int fd, stack_size, i;
  struct link_map *ei = 0;

  stack_size = backtrace(stacktrace, BSG_MAX_STACK_FRAMES);
  *frames_count = stack_size;
  for (i = 0; i < stack_size; i++) {
    if (dladdr(stacktrace[i], &sym_info) != 0) {
      bsg_strncpy(error_stacktrace[i].filename, sym_info.dli_fname,
                  STACKFRAME_NAME);
      error_stacktrace[i].frame_address = (uintptr_t)sym_info.dli_fbase;

      bsg_strncpy(error_stacktrace[i].method, sym_info.dli_sname,
                  STACKFRAME_NAME);
      error_stacktrace[i].symbol_address = (uintptr_t)sym_info.dli_saddr;
    }
  }
}