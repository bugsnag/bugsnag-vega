#ifndef BUGSNAG_KEPLER_NATIVE_H
#define BUGSNAG_KEPLER_NATIVE_H

#include "Kepler/turbomodule/KeplerTurboModule.h"
#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
    class BugsnagKeplerNative: public TM_API_NAMESPACE::KeplerTurboModule {
    public:
        BugsnagKeplerNative();

        void aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept;

        utils::json::JsonContainer configure(utils::json::JsonContainer config);
    };
}

#endif // BUGSNAG_KEPLER_NATIVE_H
