#include "turbo-modules/BugsnagKeplerNative.h"
#include "turbo-modules/BugsnagFileIO.h"

#include <Kepler/turbomodule/KeplerTurboModuleRegistration.h>

extern "C" {
__attribute__((visibility("default"))) void autoLinkKeplerTurboModulesV1() noexcept {
    // Currently, the macro argument cannot be namespace-qualified.
    using namespace bugsnag;
    KEPLER_REGISTER_TURBO_MODULE(BugsnagKeplerNative);
    KEPLER_REGISTER_TURBO_MODULE(BugsnagFileIO);
}
}
