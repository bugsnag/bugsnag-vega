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
static utils::json::JsonContainer
create_error_result(utils::json::JsonContainer container) {
  container.insert("error", true);
  container.insert("message", std::string(strerror(errno)));
  container.insert("code", errno);
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

utils::json::JsonContainer BugsnagFileIO::read_text_file(std::string path) {
  auto result = utils::json::JsonContainer::createJsonObject();
  std::ifstream ifs(path);

  if (!ifs.is_open()) {
    return create_error_result(result);
  }

  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
  if (ifs.fail()) {
    return create_error_result(result);
  }

  result.insert("content", content);
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

utils::json::JsonContainer BugsnagFileIO::write_text_file(std::string path,
                                                          std::string content) {
  auto result = utils::json::JsonContainer::createJsonObject();
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

utils::json::JsonContainer BugsnagFileIO::list_directory(std::string dir) {
  auto entries = utils::json::JsonContainer::createJsonArray();
  for (auto const &dir_entry : std::filesystem::directory_iterator{dir}) {
    auto json_entry = utils::json::JsonContainer::createJsonObject();
    std::string filename = dir_entry.path().filename();
    json_entry.insert("name", filename);
    json_entry.insert("isFile", dir_entry.is_regular_file());
    json_entry.insert("isDirectory", dir_entry.is_directory());

    entries.insert(json_entry);
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
