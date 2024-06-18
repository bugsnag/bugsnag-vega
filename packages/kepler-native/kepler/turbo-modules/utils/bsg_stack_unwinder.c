#include <execinfo.h>
#include <stdio.h>

#define __USE_GNU
#include <dlfcn.h>

#include "../external/buildid/build-id.h"
#include "bsg_stack_unwinder.h"

void bsg_fill_stack_info(bsg_stackframe *error_stacktrace, int *frames_count) {
  if (error_stacktrace == NULL) {
    return;
  }

  void *stacktrace[BSG_MAX_STACK_FRAMES];
  char build_id[256];
  Dl_info sym_info;
  int fd, stack_size, i;

  stack_size = backtrace(stacktrace, BSG_MAX_STACK_FRAMES);
  *frames_count = stack_size;
  for (i = 0; i < stack_size; i++) {
    if (dladdr(stacktrace[i], &sym_info) != 0) {
      bsg_strncpy(error_stacktrace[i].filename, sym_info.dli_fname,
                  STACKFRAME_NAME);
      error_stacktrace[i].frame_address = (uintptr_t)sym_info.dli_fbase;
      error_stacktrace[i].load_address = (uintptr_t)stacktrace[i];
      error_stacktrace[i].line_number =
          (uintptr_t)stacktrace[i] - (uintptr_t)sym_info.dli_fbase;

      bsg_strncpy(error_stacktrace[i].method, sym_info.dli_sname,
                  STACKFRAME_NAME);
      error_stacktrace[i].symbol_address = (uintptr_t)sym_info.dli_saddr;

      if (get_build_id(build_id, sym_info.dli_fbase) == 0) {
        bsg_strncpy(error_stacktrace[i].code_identifier, build_id,
                    STACKFRAME_CODE);
      }
    }
  }
}

int get_build_id(char *buffer, void *fbase) {
  const struct build_id_note *note = build_id_find_nhdr_by_fbase(fbase);
  if (!note)
    return -1;

  char buf[50];
  int length = 0;

  ElfW(Word) len = build_id_length(note);
  const uint8_t *build_id = build_id_data(note);
  for (ElfW(Word) i = 0; i < len; i++) {
    snprintf(buf, 3, "%02x", build_id[i]);
    bsg_strncpy(buffer + length, buf, 3);
    length += 2;
  }

  return 0;
}
