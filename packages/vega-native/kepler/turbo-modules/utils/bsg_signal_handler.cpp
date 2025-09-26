#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <memory>
#include <signal.h>
#include <string>
#include <unistd.h>

#include "Kepler/turbomodule/TMLog.h"

#include "../BugsnagClient.h"
#include "./serializer/bsg_event_writer.h"
#include "bsg_reference_guard.h"
#include "bsg_stack_unwinder.h"
#include "bsg_types.h"

#define BSG_HANDLED_SIGNAL_COUNT 6

static const int bsg_native_signals[BSG_HANDLED_SIGNAL_COUNT + 1] = {
    SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGSEGV};
static const char bsg_native_signal_names[BSG_HANDLED_SIGNAL_COUNT + 1][8] = {
    "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGSEGV"};
static struct sigaction bsg_previous_signal_handles[BSG_HANDLED_SIGNAL_COUNT];
static const char bsg_native_signal_msgs[BSG_HANDLED_SIGNAL_COUNT + 1][60] = {
    "Illegal instruction",
    "Trace/breakpoint trap",
    "Abort program",
    "Bus error (bad memory access)",
    "Floating-point exception",
    "Segmentation violation (invalid memory reference)"};

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

void bsg_handler_uninstall_signal() {
  for (int i = 0; i < BSG_HANDLED_SIGNAL_COUNT; i++) {
    const int signal = bsg_native_signals[i];
    sigaction(signal, &bsg_previous_signal_handles[i], 0);
  }
}

static void bsg_handle_signal(int signum, siginfo_t *info, void *user_context) {
  TMWARN("[bugsnag] Signal Handler Triggered!");

  bool is_running = false;
  if (!atomic_compare_exchange_strong(&is_signal_handler_running, &is_running,
                                      true)) {
    TMWARN("[bugsnag] An exception/signal handler is already running");
    return;
  }

  int signal_idx = 0;
  for (signal_idx = 0; signal_idx < BSG_HANDLED_SIGNAL_COUNT; ++signal_idx) {
    if (bsg_native_signals[signal_idx] == signum) {
      break;
    }
  }

  bugsnag::Event *current_event;
  bugsnag::Client *client = bugsnag::global_client;
  if (!client) {
    TMWARN("[bugsnag] No configured BugsnagClient");
    goto call_previous_handler;
  }

  current_event = client->release_event();
  if (!current_event) {
    TMWARN("[bugsnag] No event to write to disc");
    goto call_previous_handler;
  }
  bsg_breadcrumb *crumb_buffer[BUGSNAG_CRUMBS_MAX];
  int stackframe_count;
  bsg_fill_stack_info(current_event->get_exception_stackframe(),
                      &stackframe_count);
  current_event->prepare_payload(client->get_app_start_time(),
                                 client->get_is_launching(), crumb_buffer,
                                 BUGSNAG_CRUMBS_MAX);
  current_event->set_exception(bsg_native_signal_names[signal_idx],
                               bsg_native_signal_msgs[signal_idx], "c",
                               stackframe_count);
  bsg_event_write(current_event->get_payload());

call_previous_handler:
  bsg_handler_uninstall_signal();
  bsg_invoke_previous_signal_handler(signum, info, user_context);
}

void bsg_install_signal_handlers() {
  static bool handlers_installed = false;

  if (handlers_installed) {
    TMINFO("[bugsnag] bsg_install_signal_handlers() called more than once");
    return;
  }

  struct sigaction action;
  handlers_installed = true;

  memset(&action, 0, sizeof(action));
  action.sa_sigaction = bsg_handle_signal;
  action.sa_flags = SA_SIGINFO | SA_ONSTACK;

  for (int i = 0; i < BSG_HANDLED_SIGNAL_COUNT; i++) {
    sigaction(bsg_native_signals[i], &action, &bsg_previous_signal_handles[i]);
  }
}