#ifndef SAMPLETURBOMODULE_H
#define SAMPLETURBOMODULE_H

#include "Kepler/turbomodule/KeplerTurboModule.h"
#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace myApp {
    class BugsnagKeplerNative: public TM_API_NAMESPACE::KeplerTurboModule {
    public:
        BugsnagKeplerNative();

        void aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>& methodAggregator) const noexcept;

        void configure(utils::json::JsonContainer config);
    };
}
#endif  // SAMPLETURBOMODULE_H
