#include "BugsnagKeplerNative.h"

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace myApp {
    BugsnagKeplerNative::BugsnagKeplerNative() :
            TM_API_NAMESPACE::KeplerTurboModule("BugsnagKeplerNative") {}

    void BugsnagKeplerNative::aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept {
        methodAggregator.addMethod("getBool", 1, &BugsnagKeplerNative::getBool);
        methodAggregator.addMethod("getConstants", 0, &BugsnagKeplerNative::getConstants);
        methodAggregator.addMethod("getMajorVersion", 0, &BugsnagKeplerNative::getMajorVersion);
        methodAggregator.addMethod("getMinorVersion", 0, &BugsnagKeplerNative::getMinorVersion);
        methodAggregator.addMethod("getPatchVersion", 0, &BugsnagKeplerNative::getPatchVersion);
        methodAggregator.addMethod("voidFunc", 0, &BugsnagKeplerNative::voidFunc);
        methodAggregator.addMethod("getNumber", 1, &BugsnagKeplerNative::getNumber);
        methodAggregator.addMethod("getString", 1, &BugsnagKeplerNative::getString);
        methodAggregator.addMethod("getArray", 1, &BugsnagKeplerNative::getArray);
        methodAggregator.addMethod("getArrayBuffer", 1, &BugsnagKeplerNative::getArrayBuffer);
        methodAggregator.addMethod("getObject", 1, &BugsnagKeplerNative::getObject);
        methodAggregator.addMethod("getValue", 3, &BugsnagKeplerNative::getValue);
        methodAggregator.addMethod("getValueWithCallback", 1,
                                                &BugsnagKeplerNative::getValueWithCallback);
        methodAggregator.addMethod("getValueWithPromise", 1,
                                                &BugsnagKeplerNative::getValueWithPromise);

    }

    std::string BugsnagKeplerNative::getString(std::string arg) {
        return arg;
    }

    std::vector<utils::json::JsonContainer> BugsnagKeplerNative::getArray(std::vector<utils::json::JsonContainer> arg) {
        return arg;
    }

    TM_API_NAMESPACE::ArrayBuffer BugsnagKeplerNative::getArrayBuffer(TM_API_NAMESPACE::ArrayBuffer arg) {
        return arg;
    }

    bool BugsnagKeplerNative::getBool(bool arg) {
        return false;
    }

    utils::json::JsonContainer BugsnagKeplerNative::getConstants() {
        auto result = utils::json::JsonContainer::createJsonObject();
        result.insert("const1", true);
        result.insert("const2", 375);
        result.insert("const3", std::string("something"));
        return result;
    }

    int32_t BugsnagKeplerNative::getMajorVersion() {
        return 1;
    }

    int32_t BugsnagKeplerNative::getMinorVersion() {
        return 0;
    }

    int32_t BugsnagKeplerNative::getPatchVersion() {
        return 0;
    }

    void BugsnagKeplerNative::voidFunc() {
        // Nothing to do
    }

    double BugsnagKeplerNative::getNumber(double arg) {
        return arg;
    }

    utils::json::JsonContainer BugsnagKeplerNative::getObject(utils::json::JsonContainer arg) {
        return arg;
    }

    utils::json::JsonContainer BugsnagKeplerNative::getValue(double x, std::string y, utils::json::JsonContainer z) {
        // Note: return type isn't type-safe.
        auto result = utils::json::JsonContainer::createJsonObject();
        result.insert("x", x);
        result.insert("y", y);
        result.insert("z", z);
        return result;

    }

    void BugsnagKeplerNative::getValueWithCallback(TM_API_NAMESPACE::Callback callback) {
        callback.invoke(std::string{"value from callback!"});
    }

    TM_API_NAMESPACE::Promise BugsnagKeplerNative::getValueWithPromise(bool error) {
        return TM_API_NAMESPACE::Promise([error](std::shared_ptr<TM_API_NAMESPACE::Promise> promise) {
            if (error) {
                promise->reject(std::string{"intentional promise rejection"});
            } else {
                promise->resolve(std::string{"result!"});
            }
        });
    }
}