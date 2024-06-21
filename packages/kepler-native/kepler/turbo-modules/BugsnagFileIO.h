#ifndef BUGSNAG_FILE_IO_H
#define BUGSNAG_FILE_IO_H

#include "Kepler/turbomodule/KeplerTurboModule.h"

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
class BugsnagFileIO : public TM_API_NAMESPACE::KeplerTurboModule {
public:
  BugsnagFileIO();

  void aggregateMethods(
      TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
          &methodAggregator) const noexcept;

  TM_API_NAMESPACE::JSObject read_text_file(std::string filename);
  TM_API_NAMESPACE::ArrayBuffer read_file(std::string filename);
  TM_API_NAMESPACE::JSObject write_text_file(std::string filename,
                                             std::string content);
  TM_API_NAMESPACE::JSArray list_directory(std::string dir);
  bool mkdir(std::string path);
  bool delete_file(std::string path);
  std::string sha1(std::string data);
};
} // namespace bugsnag

#endif // BUGSNAG_FILE_IO_H
