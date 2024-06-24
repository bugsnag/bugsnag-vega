#include "Kepler/turbomodule/TMLog.h"

#include "BugsnagFileIO.h"
#include "external/sha1/sha1.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {

template <typename T>
static T get_js_value(TM_API_NAMESPACE::JSObject &js_object, std::string key,
                      T default_value) {
  T result = js_object.find(key) != js_object.end()
                 ? std::get<T>(js_object[key])
                 : default_value;

  return result;
}

static TM_API_NAMESPACE::JSObject
create_error_result(TM_API_NAMESPACE::JSObject container) {
  container["error"] = true;
  container["message"] = std::string(strerror(errno));
  container["code"] = errno;
  return container;
}

BugsnagFileIO::BugsnagFileIO()
    : TM_API_NAMESPACE::KeplerTurboModule("BugsnagFileIO") {}

void BugsnagFileIO::aggregateMethods(
    TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
        &methodAggregator) const noexcept {
  methodAggregator.addMethod("readTextFile", &BugsnagFileIO::read_text_file);
  methodAggregator.addMethod("readFile", &BugsnagFileIO::read_file);
  methodAggregator.addMethod("writeTextFile", &BugsnagFileIO::write_text_file);
  methodAggregator.addMethod("listDirectory", &BugsnagFileIO::list_directory);
  methodAggregator.addMethod("mkdir", &BugsnagFileIO::mkdir);
  methodAggregator.addMethod("deleteFile", &BugsnagFileIO::delete_file);
  methodAggregator.addMethod("sha1", &BugsnagFileIO::sha1);
}

TM_API_NAMESPACE::JSObject BugsnagFileIO::read_text_file(std::string path) {
  auto result = TM_API_NAMESPACE::JSObject();
  std::ifstream ifs(path);

  if (!ifs.is_open()) {
    return create_error_result(result);
  }

  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  if (ifs.fail()) {
    return create_error_result(result);
  }

  result["content"] = content;
  return result;
}

TM_API_NAMESPACE::ArrayBuffer BugsnagFileIO::read_file(std::string path) {
  auto result = TM_API_NAMESPACE::ArrayBuffer();
  std::ifstream ifs(path, std::ios::binary);

  if (!ifs.is_open()) {
    return result;
  }

  uint8_t buffer[1024];
  while (true) {
    if (!ifs.read(reinterpret_cast<char *>(buffer), sizeof(buffer))) {
      break;
    }

    if (ifs.gcount() == 0) {
      break;
    }

    result.insert(static_cast<uint8_t *>(buffer), ifs.gcount());
  }

  return result;
}

TM_API_NAMESPACE::JSObject BugsnagFileIO::write_text_file(std::string path,
                                                          std::string content) {
  auto result = TM_API_NAMESPACE::JSObject();
  std::ofstream ofs(path);
  if (!ofs.is_open()) {
    return create_error_result(result);
  }

  ofs << content;

  if (ofs.fail()) {
    return create_error_result(result);
  }

  return result;
}

TM_API_NAMESPACE::JSArray BugsnagFileIO::list_directory(std::string dir) {
  auto entries = TM_API_NAMESPACE::JSArray();
  for (auto const &dir_entry : std::filesystem::directory_iterator{dir}) {
    auto json_entry = TM_API_NAMESPACE::JSObject();
    std::string filename = dir_entry.path().filename();
    json_entry["name"] = filename;
    json_entry["isFile"] = dir_entry.is_regular_file();
    json_entry["isDirectory"] = dir_entry.is_directory();

    entries.push_back(json_entry);
  }

  return entries;
}

bool BugsnagFileIO::mkdir(std::string path) {
  std::error_code ec;
  std::filesystem::create_directories(std::filesystem::path{path}, ec);
  return !ec;
}

bool BugsnagFileIO::delete_file(std::string path) {
  std::error_code ec;
  std::filesystem::remove(std::filesystem::path{path}, ec);
  return !ec;
}

std::string BugsnagFileIO::sha1(std::string data) {
  SHA1 digest;
  digest.update(data);
  return digest.final();
}

} // namespace bugsnag
