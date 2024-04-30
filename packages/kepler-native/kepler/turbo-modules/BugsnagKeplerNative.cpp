#include "Kepler/turbomodule/TMLog.h"

#include "BugsnagKeplerNative.h"
#include "utils/bsg_signal_handler.h"

#include <chrono>
#include <filesystem>
#include <fstream>
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
  this->device_id = this->generate_uuid();
}

void BugsnagKeplerNative::aggregateMethods(
    TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
        &methodAggregator) const noexcept {
  methodAggregator.addMethod("configure", 1, &BugsnagKeplerNative::configure);
  methodAggregator.addMethod("markLaunchCompleted", 0,
                             &BugsnagKeplerNative::mark_launch_completed);
  methodAggregator.addMethod("getDeviceID", 0,
                             &BugsnagKeplerNative::get_device_id);
  methodAggregator.addMethod("setDeviceID", 1,
                             &BugsnagKeplerNative::set_device_id);
  methodAggregator.addMethod("generateUUID", 0,
                             &BugsnagKeplerNative::generate_uuid);
  methodAggregator.addMethod("leaveBreadcrumb", 1,
                             &BugsnagKeplerNative::leave_breadcrumb);
  methodAggregator.addMethod("setMetadata", 1,
                             &BugsnagKeplerNative::set_metadata);
  methodAggregator.addMethod("clearMetadata", 0,
                             &BugsnagKeplerNative::clear_metadata);
  methodAggregator.addMethod("setFeatures", 1,
                             &BugsnagKeplerNative::set_features);
  methodAggregator.addMethod("clearFeatures", 0,
                             &BugsnagKeplerNative::clear_features);
  methodAggregator.addMethod("nativeCrash", 0,
                             &BugsnagKeplerNative::native_crash);
}

utils::json::JsonContainer
BugsnagKeplerNative::configure(utils::json::JsonContainer config) {
  auto bsg_config = std::make_unique<Configuration>();
  bsg_config->storage_dir = std::string("/data/");

  this->bugsnag = new Client(std::move(bsg_config));
  global_client = this->bugsnag;

  install_signal_handlers();

  auto result = utils::json::JsonContainer::createJsonObject();
  result.insert("app", createStaticApp());

  return result;
}

void BugsnagKeplerNative::mark_launch_completed() {
  if (this->bugsnag != nullptr) {
    this->bugsnag->mark_launch_completed();
  }
}

std::string BugsnagKeplerNative::generate_uuid() {
  std::random_device rng;
  const char *available_chars = "0123456789abcdef";
  std::uniform_int_distribution<int> dist(0, 15);
  char uuid_buffer[37];

  for (int i = 0; i < 36; ++i) {
    uuid_buffer[i] = available_chars[dist(rng)];
  }
  uuid_buffer[8] = '-';
  uuid_buffer[13] = '-';
  uuid_buffer[18] = '-';
  uuid_buffer[23] = '-';
  return std::string(uuid_buffer);
}

std::string BugsnagKeplerNative::get_device_id() { return this->device_id; }

void BugsnagKeplerNative::set_device_id(std::string device_id) {
  this->device_id = device_id;
}

void BugsnagKeplerNative::leave_breadcrumb(utils::json::JsonContainer crumb) {
  auto type = crumb["type"].getValue(0);
  bsg_breadcrumb_type casted_type = static_cast<bsg_breadcrumb_type>(type);

  std::string empty = "";
  auto msg = crumb["message"].getValue(empty);
  auto metadata = crumb["metadata"].getValue(empty);

  auto time_now = std::chrono::system_clock::now();
  time_t timestamp = std::chrono::system_clock::to_time_t(time_now);
  auto crumb_time = crumb["timestamp"].getValue(0);
  if (crumb_time != 0) {
    timestamp = static_cast<time_t>(crumb_time);
  }

  this->bugsnag->leave_breadcrumb(casted_type, msg, metadata, timestamp);
}

void BugsnagKeplerNative::set_metadata(std::string metadata_str) {
  this->bugsnag->set_metadata(metadata_str);
}

void BugsnagKeplerNative::clear_metadata() { this->bugsnag->clear_metadata(); }

void BugsnagKeplerNative::set_features(std::string features_str) {
  this->bugsnag->set_features(features_str);
}

void BugsnagKeplerNative::clear_features() { this->bugsnag->clear_features(); }

// Temporary native crash that can be used for testing:

static void __attribute__((used)) somefakefunc(void) {}

void BugsnagKeplerNative::native_crash() {
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}
} // namespace bugsnag
