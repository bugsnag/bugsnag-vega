#include "Kepler/turbomodule/TMLog.h"

#include "BugsnagKeplerNative.h"
#include "utils/signal_handler.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
BugsnagKeplerNative::BugsnagKeplerNative() :
        TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {}

void BugsnagKeplerNative::aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept {
    methodAggregator.addMethod("configure", 1, &BugsnagKeplerNative::configure);
    methodAggregator.addMethod("readTextFile", 1, &BugsnagKeplerNative::readTextFile);
    methodAggregator.addMethod("writeTextFile", 2, &BugsnagKeplerNative::writeTextFile);
    methodAggregator.addMethod("nativeCrash", 0, &BugsnagKeplerNative::nativeCrash);
}

std::string BugsnagKeplerNative::readTextFile(std::string filename) {
    std::string result;

    char buffer[1024];
    auto fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        return nullptr;
    }

    ssize_t data_read;

    while ((data_read = read(fd, buffer, sizeof(buffer))) > 0) {
        result.append(std::string(buffer, (size_t)data_read));
    }

    return result;
}

bool BugsnagKeplerNative::writeTextFile(std::string filename, std::string content) {
    auto fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        return false;
    }

    auto bytes = write(fd, content.c_str(), content.length());

    close(fd);
    return content.length() == bytes;
}

utils::json::JsonContainer BugsnagKeplerNative::configure(utils::json::JsonContainer config) {
    auto bsg_config = std::make_unique<Configuration>();
    bsg_config->storage_dir = std::string("/data/");

    Client *c = new Client(std::move(bsg_config));
    global_client = c;

    install_signal_handlers();

    auto result = utils::json::JsonContainer::createJsonObject();

    return result;
}

// Temporary native crash that can be used for testing:

static void __attribute__((used)) somefakefunc(void) {}

void BugsnagKeplerNative::nativeCrash() {
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}

}
