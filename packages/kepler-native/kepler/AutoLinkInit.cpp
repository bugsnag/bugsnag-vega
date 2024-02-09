/*
* Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* PROPRIETARY/CONFIDENTIAL
* Use is subject to license terms.
*/
#include "turbo-modules/BugsnagKeplerNative.h"

#include <Kepler/turbomodule/KeplerTurboModuleRegistration.h>

extern "C" {
__attribute__((visibility("default"))) void
    autoLinkKeplerTurboModulesV1() noexcept {
        // Currently, the macro argument cannot be namespace-qualified.
        using namespace myApp;
        KEPLER_REGISTER_TURBO_MODULE(BugsnagKeplerNative);
 }
}
