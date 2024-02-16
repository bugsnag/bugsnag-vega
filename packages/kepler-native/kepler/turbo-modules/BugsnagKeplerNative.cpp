#include "BugsnagKeplerNative.h"
#include <string.h>

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
    BugsnagKeplerNative::BugsnagKeplerNative() :
            TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {}

    void BugsnagKeplerNative::aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept {
        methodAggregator.addMethod("configure", 1, &BugsnagKeplerNative::configure);
    }

    utils::json::JsonContainer BugsnagKeplerNative::configure(utils::json::JsonContainer config) {
        std::string apiKey = config["apiKey"].getValue<std::string>();
        std::string type = config["type"].getValue<std::string>();
        std::string appVersion = config["appVersion"].getValue<std::string>();
        double launchDurationMillis = config["launchDurationMillis"].getValue<double>();

        auto result = utils::json::JsonContainer::createJsonObject();

        return result;
    }
}
