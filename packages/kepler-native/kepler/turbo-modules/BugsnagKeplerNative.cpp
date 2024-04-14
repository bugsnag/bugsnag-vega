#include "Kepler/turbomodule/TMLog.h"

#include "BugsnagKeplerNative.h"
#include "utils/signal_handler.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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

BugsnagKeplerNative::BugsnagKeplerNative()
    : TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {
  this->deviceID = this->generateUUID();
}

void BugsnagKeplerNative::aggregateMethods(
    TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
        &methodAggregator) const noexcept {
  methodAggregator.addMethod("configure", 1, &BugsnagKeplerNative::configure);
  methodAggregator.addMethod("markLaunchCompleted", 0,
                             &BugsnagKeplerNative::markLaunchCompleted);
  methodAggregator.addMethod("getDeviceID", 0,
                             &BugsnagKeplerNative::getDeviceID);
  methodAggregator.addMethod("setDeviceID", 1,
                             &BugsnagKeplerNative::setDeviceID);
  methodAggregator.addMethod("generateUUID", 0,
                             &BugsnagKeplerNative::generateUUID);
  methodAggregator.addMethod("nativeCrash", 0,
                             &BugsnagKeplerNative::nativeCrash);
}

utils::json::JsonContainer
BugsnagKeplerNative::configure(utils::json::JsonContainer config) {
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

std::string BugsnagKeplerNative::generateUUID() {
  std::random_device rng;
  const char *availableChars = "0123456789abcdef";
  std::uniform_int_distribution<int> dist(0, 15);
  char uuidBuffer[37];

  for (int i = 0; i < 36; ++i) {
    uuidBuffer[i] = availableChars[dist(rng)];
  }
  uuidBuffer[8] = '-';
  uuidBuffer[13] = '-';
  uuidBuffer[18] = '-';
  uuidBuffer[23] = '-';
  return std::string(uuidBuffer);
}

std::string BugsnagKeplerNative::getDeviceID() { return this->deviceID; }

void BugsnagKeplerNative::setDeviceID(std::string deviceID) {
  this->deviceID = deviceID;
}

// Temporary native crash that can be used for testing:

static void __attribute__((used)) somefakefunc(void) {}

void BugsnagKeplerNative::nativeCrash() {
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}
} // namespace bugsnag
