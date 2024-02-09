#ifndef SAMPLETURBOMODULE_H
#define SAMPLETURBOMODULE_H

#include "Kepler/turbomodule/KeplerTurboModule.h"
#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace myApp {
    class BugsnagKeplerNative: public TM_API_NAMESPACE::KeplerTurboModule {
    public:
        BugsnagKeplerNative();

        void aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept;

        int32_t getMajorVersion();
        int32_t getMinorVersion();
        int32_t getPatchVersion();

        std::string getString(std::string arg);
        std::vector<utils::json::JsonContainer> getArray(std::vector<utils::json::JsonContainer> arg);
        TM_API_NAMESPACE::ArrayBuffer getArrayBuffer(TM_API_NAMESPACE::ArrayBuffer arg);
        bool getBool(bool arg);
        utils::json::JsonContainer getConstants();
        void voidFunc();
        double getNumber(double arg);
        utils::json::JsonContainer getObject(utils::json::JsonContainer arg);
        utils::json::JsonContainer getValue(double x, std::string y, utils::json::JsonContainer z);
        void getValueWithCallback(TM_API_NAMESPACE::Callback callback);
        TM_API_NAMESPACE::Promise getValueWithPromise(bool error);
    };
}
#endif  // SAMPLETURBOMODULE_H
