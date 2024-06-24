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
static TM_API_NAMESPACE::JSObject createStaticApp() {
  auto app = TM_API_NAMESPACE::JSObject();

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
  app["binaryArch"] = std::string(binary_arch);

  return app;
}

template <typename T>
static T get_js_value(TM_API_NAMESPACE::JSObject &js_object, std::string key,
                      T default_value) {
  T result = js_object.find(key) != js_object.end()
                 ? std::get<T>(js_object[key])
                 : default_value;

  return result;
}

BugsnagKeplerNative::BugsnagKeplerNative()
    : TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {
  this->device_id = this->generate_uuid();
}

void BugsnagKeplerNative::aggregateMethods(
    TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
        &methodAggregator) const noexcept {
  methodAggregator.addMethod("configure", &BugsnagKeplerNative::configure);
  methodAggregator.addMethod("markLaunchCompleted",
                             &BugsnagKeplerNative::mark_launch_completed);
  methodAggregator.addMethod("getDeviceID",
                             &BugsnagKeplerNative::get_device_id);
  methodAggregator.addMethod("setDeviceID",
                             &BugsnagKeplerNative::set_device_id);
  methodAggregator.addMethod("generateUUID",
                             &BugsnagKeplerNative::generate_uuid);
  methodAggregator.addMethod("leaveBreadcrumb",
                             &BugsnagKeplerNative::leave_breadcrumb);
  methodAggregator.addMethod("setMetadata", &BugsnagKeplerNative::set_metadata);
  methodAggregator.addMethod("clearMetadata",
                             &BugsnagKeplerNative::clear_metadata);
  methodAggregator.addMethod("setFeatures", &BugsnagKeplerNative::set_features);
  methodAggregator.addMethod("clearFeatures",
                             &BugsnagKeplerNative::clear_features);
  methodAggregator.addMethod("setUser", &BugsnagKeplerNative::set_user_data);
  methodAggregator.addMethod("clearUser",
                             &BugsnagKeplerNative::clear_user_data);
  methodAggregator.addMethod("setApp", &BugsnagKeplerNative::set_app_data);
  methodAggregator.addMethod("setDevice", &BugsnagKeplerNative::set_device_data);
}

TM_API_NAMESPACE::JSObject
BugsnagKeplerNative::configure(TM_API_NAMESPACE::JSObject config) {
  auto bsg_config = std::make_unique<Configuration>();

  std::string api_key = get_js_value<std::string>(config, "apikey", "");
  TMWARN("[bugsnag] got api key " + api_key);
  bsg_config->api_key = api_key;

  std::string persistence_dir = get_js_value<std::string>(
      config, "persistenceDirectory", "/data/bugsnag");
  bsg_config->storage_dir = persistence_dir + "/errors";
  TMWARN("[bugsnag] got persistence dir " + persistence_dir);

  if (bsg_config->max_breadcrumbs > BUGSNAG_CRUMBS_MAX) {
    bsg_config->max_breadcrumbs = BUGSNAG_CRUMBS_MAX;
  }

  this->bugsnag = new Client(std::move(bsg_config));
  global_client = this->bugsnag;
  this->bugsnag->set_device_id(this->device_id);

  install_signal_handlers();

  auto result = TM_API_NAMESPACE::JSObject();
  result["app"] = createStaticApp();

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
  if (this->bugsnag == nullptr) {
    return;
  }
  this->bugsnag->set_device_id(device_id);
}

void BugsnagKeplerNative::leave_breadcrumb(TM_API_NAMESPACE::JSObject crumb) {
  if (this->bugsnag == nullptr) {
    return;
  }

  auto type = get_js_value<double>(crumb, "type", 0);
  bsg_breadcrumb_type casted_type = static_cast<bsg_breadcrumb_type>(type);

  auto msg = get_js_value<std::string>(crumb, "message", "");
  auto metadata = get_js_value<std::string>(crumb, "metadata", "");

  auto time_now = std::chrono::system_clock::now();
  time_t timestamp = std::chrono::system_clock::to_time_t(time_now);
  auto crumb_time = get_js_value<double>(crumb, "timestamp", 0);
  if (crumb_time != 0) {
    timestamp = static_cast<time_t>(crumb_time);
  }

  this->bugsnag->leave_breadcrumb(casted_type, msg, metadata, timestamp);
}

void BugsnagKeplerNative::set_metadata(std::string metadata_str) {
  if (this->bugsnag == nullptr) {
    return;
  }

  this->bugsnag->set_metadata(metadata_str);
}

void BugsnagKeplerNative::clear_metadata() {
  if (this->bugsnag == nullptr) {
    return;
  }
  this->bugsnag->clear_metadata();
}

void BugsnagKeplerNative::set_features(std::string features_str) {
  if (this->bugsnag == nullptr) {
    return;
  }

  this->bugsnag->set_features(features_str);
}

void BugsnagKeplerNative::clear_features() {
  if (this->bugsnag == nullptr) {
    return;
  }
  this->bugsnag->clear_features();
}

void BugsnagKeplerNative::set_user_data(TM_API_NAMESPACE::JSObject user_data) {
  if (this->bugsnag == nullptr) {
    return;
  }

  auto id = get_js_value<std::string>(user_data, "id", "");
  auto email = get_js_value<std::string>(user_data, "email", "");
  auto name = get_js_value<std::string>(user_data, "name", "");
  this->bugsnag->set_user_data(id, email, name);
}

void BugsnagKeplerNative::clear_user_data() {
  if (this->bugsnag == nullptr) {
    return;
  }
  this->bugsnag->clear_user_data();
}

void BugsnagKeplerNative::set_app_data(TM_API_NAMESPACE::JSObject app_data) {
  if (this->bugsnag == nullptr) {
    return;
  }

  std::string id = get_js_value<std::string>(app_data, "id", "");
  std::string stage = get_js_value<std::string>(app_data, "releaseStage", "");
  std::string type = get_js_value<std::string>(app_data, "type", "");
  std::string ver = get_js_value<std::string>(app_data, "version", "");
  std::string binary_arch =
      get_js_value<std::string>(app_data, "binaryArch", "");

  this->bugsnag->set_app_data(id, stage, type, ver, binary_arch);
}

void BugsnagKeplerNative::set_device_data(
    TM_API_NAMESPACE::JSObject device_data) {
  if (this->bugsnag == nullptr) {
    return;
  }

  auto manufacturer =
      get_js_value<std::string>(device_data, "manufacturer", "");
  auto model = get_js_value<std::string>(device_data, "model", "");
  auto os_name = get_js_value<std::string>(device_data, "osName", "");
  auto os_version = get_js_value<std::string>(device_data, "osVersion", "");

  auto runtime_versions = get_js_value<TM_API_NAMESPACE::JSObject>(
      device_data, "runtimeVersions", TM_API_NAMESPACE::JSObject());
  auto rn_version =
      get_js_value<std::string>(runtime_versions, "reactNative", "");
  auto js_engine =
      get_js_value<std::string>(runtime_versions, "reactNativeJsEngine", "");

  this->bugsnag->set_device_data(manufacturer, model, os_name, os_version,
                                 rn_version, js_engine);
}
} // namespace bugsnag
