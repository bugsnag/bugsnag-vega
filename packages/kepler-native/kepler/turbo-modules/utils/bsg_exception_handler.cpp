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

#include "bsg_exception_handler.h"

/**
 * Previously installed termination handler
 */
std::terminate_handler bsg_global_terminate_previous;
static bool terminate_handlers_installed;

void bsg_uninstall_terminate_handler() {
  if (!terminate_handlers_installed)
    return;
  std::set_terminate(bsg_global_terminate_previous);
  terminate_handlers_installed = false;
}

static void bsg_handle_terminate() {
  TMWARN("[bugsnag] Terminate handler triggered!");

  bool is_running = false;
  if (!atomic_compare_exchange_strong(&is_signal_handler_running, &is_running,
                                      true)) {
    TMWARN("[bugsnag] An exception/signal handler is already running");
    return;
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
  current_event->set_exception("EXCEPTION", "native exception", "c",
                               stackframe_count);
  bsg_event_write(current_event->get_payload());

call_previous_handler:
  bsg_uninstall_terminate_handler();
  if (bsg_global_terminate_previous != NULL) {
    bsg_global_terminate_previous();
  }
}

void bsg_install_terminate_handlers() {
  terminate_handlers_installed = false;

  if (terminate_handlers_installed) {
    TMINFO("[bugsnag] bsg_install_terminate_handlers() called more than once");
    return;
  }

  bsg_global_terminate_previous = std::set_terminate(bsg_handle_terminate);
  terminate_handlers_installed = true;
}