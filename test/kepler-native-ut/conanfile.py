from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

required_conan_version = ">=2.0.0"

class BugsnagNativeUTConan(ConanFile):
    # Optional metadata
    name = "bugsnagkeplernativeut"
    version = "1.0.0"
    description = "Add a description here"

    topics = ("list", "of", "topics")

    # Binary configuration
    settings = "os", "arch", "build_type"

    options = {
        "shared": [True],
        "fPIC": [True],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
    }

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS"] = True
        tc.generate()
        cmake = CMakeDeps(self)
        cmake.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "BugsnagNativeUT")
        self.cpp_info.set_property("cmake_target_name", "BugsnagNativeUT::BugsnagNativeUT")
        self.cpp_info.libs = ["BugsnagNativeUT"]
