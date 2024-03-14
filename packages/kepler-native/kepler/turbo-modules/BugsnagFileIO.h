#ifndef BUGSNAG_FILE_IO_H
#define BUGSNAG_FILE_IO_H

#include "Kepler/turbomodule/KeplerTurboModule.h"

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
    class BugsnagFileIO: public TM_API_NAMESPACE::KeplerTurboModule {
      public:
        BugsnagFileIO();

        void aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept;

        utils::json::JsonContainer readTextFile(std::string filename);
        TM_API_NAMESPACE::ArrayBuffer readFile(std::string filename);
        utils::json::JsonContainer writeTextFile(std::string filename, std::string content);
        utils::json::JsonContainer listDirectory(std::string dir);
        bool mkdir(std::string path);
        bool deleteFile(std::string path);
        std::string sha1(std::string data);
    };
}

#endif // BUGSNAG_FILE_IO_H
