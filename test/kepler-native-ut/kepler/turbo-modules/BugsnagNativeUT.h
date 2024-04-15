#ifndef BUGSNAG_NATIVE_UT_H
#define BUGSNAG_NATIVE_UT_H

#include "Kepler/turbomodule/KeplerTurboModule.h"
#include "doctest.h"

#include <sstream>

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag
{
    class BugsnagNativeUT : public TM_API_NAMESPACE::KeplerTurboModule
    {
    public:
        BugsnagNativeUT();

        void aggregateMethods(TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule> &methodAggregator) const noexcept;

        void configure();
        int runUnitTests();

    private:
        doctest::Context context;
        std::ostringstream utOutput;
    };
}
#endif // BUGSNAG_NATIVE_UT_H
