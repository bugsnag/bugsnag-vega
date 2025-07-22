cmake_minimum_required(VERSION 3.15)

project(BugsnagNativeUT
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

if (CMAKE_SYSTEM_NAME STREQUAL Android)
  add_compile_definitions(ANDROID)
endif()

set(HEADERS
  kepler/turbo-modules/generated/BugsnagNativeUTSpec.h
  kepler/turbo-modules/doctest.h
  kepler/turbo-modules/BugsnagNativeUT.h
)

set(SOURCES
  kepler/turbo-modules/generated/BugsnagNativeUTSpec.cpp
  kepler/turbo-modules/BugsnagNativeUT.cpp
  kepler/AutoLinkInit.cpp
)

set(NATIVE_DIR ${CMAKE_SOURCE_DIR}/../../packages/kepler-native)
set(KEPLER_NATIVE_H ${NATIVE_DIR}/kepler/turbo-modules/BugsnagFileIO.h)
set(KEPLER_NATIVE_S ${NATIVE_DIR}/kepler/turbo-modules/BugsnagFileIO.cpp)

find_package(turbomoduleAPI CONFIG REQUIRED)

add_library(BugsnagNativeUT SHARED ${HEADERS} ${SOURCES} ${KEPLER_NATIVE_H} ${KEPLER_NATIVE_S})

set(LIBRARIES turbomoduleAPI::turbomoduleAPI)

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    list(APPEND LIBRARIES
        android log)
endif()
target_include_directories(BugsnagNativeUT PRIVATE ${NATIVE_DIR})
target_link_libraries(BugsnagNativeUT PRIVATE ${LIBRARIES})

install(TARGETS BugsnagNativeUT)
