cmake_minimum_required(VERSION 3.15)

project(BugsnagKeplerNative
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
  add_compile_definitions(ANDROID)
endif()

set(HEADERS
  kepler/turbo-modules/utils/bsg_reference_guard.h
  kepler/turbo-modules/utils/bsg_signal_handler.h
  kepler/turbo-modules/utils/bsg_breadcrumb.h
  kepler/turbo-modules/utils/bsg_types.h
  kepler/turbo-modules/utils/bsg_event.h
  kepler/turbo-modules/utils/serializer/bsg_buffered_writer.h
  kepler/turbo-modules/utils/serializer/bsg_event_writer.h
  kepler/turbo-modules/utils/serializer/BSG_KSCrashStringConversion.h
  kepler/turbo-modules/utils/serializer/BSG_KSJSONCodec.h
  kepler/turbo-modules/BugsnagKeplerNative.h
  kepler/turbo-modules/BugsnagSignalSafePtr.h
  kepler/turbo-modules/BugsnagBreadcrumbBuffer.h
)
set(SOURCES
    kepler/turbo-modules/utils/bsg_reference_guard.c
    kepler/turbo-modules/utils/bsg_signal_handler.cpp
    kepler/turbo-modules/utils/bsg_breadcrumb.c
    kepler/turbo-modules/utils/bsg_event.c
    kepler/turbo-modules/utils/serializer/bsg_buffered_writer.c
    kepler/turbo-modules/utils/serializer/bsg_event_writer.c
    kepler/turbo-modules/utils/serializer/BSG_KSCrashStringConversion.c
    kepler/turbo-modules/utils/serializer/BSG_KSJSONCodec.c
    kepler/turbo-modules/BugsnagClient.cpp
    kepler/turbo-modules/BugsnagEvent.cpp
    kepler/turbo-modules/BugsnagFileIO.cpp
    kepler/turbo-modules/BugsnagKeplerNative.cpp
    kepler/turbo-modules/BugsnagBreadcrumbBuffer.cpp
    kepler/AutoLinkInit.cpp
)

find_package(turbomoduleAPI CONFIG REQUIRED)

add_library(BugsnagKeplerNative SHARED ${HEADERS} ${SOURCES})

set(LIBRARIES turbomoduleAPI::turbomoduleAPI JsonContainer::JsonContainer)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    list(APPEND LIBRARIES
        android log)
endif()
target_link_libraries(BugsnagKeplerNative PRIVATE ${LIBRARIES})

install(TARGETS BugsnagKeplerNative)
