#include "turbo-modules/BugsnagFileIO.h"
#include "turbo-modules/BugsnagKeplerNative.h"

#include <Kepler/turbomodule/KeplerTurboModuleRegistration.h>

extern "C" {
__attribute__((visibility("default"))) void
autoLinkKeplerTurboModulesV1() noexcept {
  KEPLER_REGISTER_TURBO_MODULE(bugsnag, BugsnagKeplerNative);
  KEPLER_REGISTER_TURBO_MODULE(bugsnag, BugsnagFileIO);
}
}
