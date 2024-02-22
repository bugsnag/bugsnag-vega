#include <errno.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <memory>
#include <string>

#include "Kepler/turbomodule/TMLog.h"

#include "../BugsnagClient.h"

#define BSG_HANDLED_SIGNAL_COUNT 6
#define BSG_MAX_STACK_FRAMES 512

static const int bsg_native_signals[BSG_HANDLED_SIGNAL_COUNT + 1] = {
    SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGSEGV};
static struct sigaction bsg_previous_signal_handles[BSG_HANDLED_SIGNAL_COUNT];

static void bsg_invoke_previous_signal_handler(int signum, siginfo_t *info,
                                        void *user_context) {
  for (int i = 0; i < BSG_HANDLED_SIGNAL_COUNT; ++i) {
    const int signal = bsg_native_signals[i];
    if (signal == signum) {
      struct sigaction previous = bsg_previous_signal_handles[i];
      // From sigaction(2):
      //  > If act is non-zero, it specifies an action (SIG_DFL, SIG_IGN, or a
      //  handler routine)
      if (previous.sa_flags & SA_SIGINFO) {
        // This handler can handle signal number, info, and user context
        // (POSIX). From sigaction(2): > If this bit is set, the handler
        // function is assumed to be pointed to by the sa_sigaction member of
        // struct sigaction and should match the proto- type shown above or as
        // below in EXAMPLES.  This bit should not be set when assigning SIG_DFL
        // or SIG_IGN.
        previous.sa_sigaction(signum, info, user_context);
      } else if (previous.sa_handler == SIG_DFL) {
        raise(signum); // raise to trigger the default handler. It cannot be
                       // called directly.
      } else if (previous.sa_handler != SIG_IGN) {
        // This handler can only handle to signal number (ANSI C)
        void (*previous_handler)(int) = previous.sa_handler;
        previous_handler(signum);
      }
    }
  }
}

static void bsg_handle_signal(int signum, siginfo_t *info,
                       void *user_context) {

    TMWARN("Signal Handler Triggered!");

    static const char tab[] = "\t";
    static const char lf[] = "\n";

    void *stacktrace[BSG_MAX_STACK_FRAMES];
    Dl_info sym_info;
    int fd, stack_size, i;

    bugsnag::Client *client = bugsnag::global_client;
    if (!client) {
        TMWARN("No configured BugsnagClient");
        goto call_previous_handler;
    }

    fd = open(client->event_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        TMWARN("Could not open crash dump file:");
        TMWARN(client->event_filename.c_str());

        goto call_previous_handler;
    }

    stack_size = backtrace(stacktrace, BSG_MAX_STACK_FRAMES);
    for (i = 0; i < stack_size; i++) {
        if (dladdr(stacktrace[i], &sym_info) != 0) {
            if(sym_info.dli_fname) {
                write(fd, sym_info.dli_fname, strlen(sym_info.dli_fname));
            } else {
                write(fd, tab, 1);
            }

            if (sym_info.dli_sname) {
                write(fd, tab, 1);
                write(fd, sym_info.dli_sname, strlen(sym_info.dli_sname));
            }

            write(fd, lf, 1);
        } else {
            // oh no! anyway...
        }
    }

    close(fd);
call_previous_handler:
    bsg_invoke_previous_signal_handler(signum, info, user_context);
}


void install_signal_handlers() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = bsg_handle_signal;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;

    for (int i = 0; i < BSG_HANDLED_SIGNAL_COUNT; i++) {
        sigaction(bsg_native_signals[i], &action, &bsg_previous_signal_handles[i]);
    }
}
