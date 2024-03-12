#include "Kepler/turbomodule/TMLog.h"

#include "BugsnagKeplerNative.h"
#include "utils/signal_handler.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
static utils::json::JsonContainer createStaticApp() {
  auto app = utils::json::JsonContainer::createJsonObject();

  #if defined(__i386__)
    const char *binary_arch = "x86";
  #elif defined(__x86_64__)
    const char *binary_arch = "x86_64";
  #elif defined(__arm__)
    const char *binary_arch = "arm32";
  #elif defined(__aarch64__)
    const char *binary_arch = "arm64";
  #else
    const char *binary_arch = "unknown";
  #endif
  app.insert("binaryArch", std::string(binary_arch));

  return app;
}

BugsnagKeplerNative::BugsnagKeplerNative() :
        TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {}

void BugsnagKeplerNative::aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept {
  methodAggregator.addMethod("configure", 1, &BugsnagKeplerNative::configure);
  methodAggregator.addMethod("markLaunchCompleted", 0, &BugsnagKeplerNative::markLaunchCompleted);
  methodAggregator.addMethod("nativeCrash", 0, &BugsnagKeplerNative::nativeCrash);
}

utils::json::JsonContainer BugsnagKeplerNative::configure(utils::json::JsonContainer config) {
  auto bsg_config = std::make_unique<Configuration>();
  bsg_config->storage_dir = std::string("/data/");

  Client *c = new Client(std::move(bsg_config));
  global_client = c;

  install_signal_handlers();

  auto result = utils::json::JsonContainer::createJsonObject();
  result.insert("app", createStaticApp());

  return result;
}

void BugsnagKeplerNative::markLaunchCompleted() {
  auto client = this->bugsnag;
  if (client != nullptr) {
    client->markLaunchCompleted();
  }
}

// Temporary native crash that can be used for testing:

static void __attribute__((used)) somefakefunc(void) {}

void BugsnagKeplerNative::nativeCrash() {
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}

}
