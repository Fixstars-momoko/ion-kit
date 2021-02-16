from conans import ConanFile, CMake


class IonCoreConan(ConanFile):
    name = "ion-core"
    version = "0.1.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "The library to build and compile pipelines"
    settings = "os", "compiler", "build_type", "arch"
    options = {"run_test": [True, False]}
    default_options = {"run_test": False}
    generators = "cmake"
    requires = "halide/[8.0.x]", "nlohmann_json/3.9.1"
    exports_sources = "*"

    _cmake = None

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions["HALIDE_ROOT"] = self.deps_cpp_info[
            "halide"].rootpath
        self._cmake.definitions["ION_BUILD_DOC"] = "OFF"
        self._cmake.definitions[
            "ION_BUILD_TEST"] = "ON" if self.options.run_test else "OFF"
        self._cmake.definitions["ION_BUNDLE_HALIDE"] = "OFF"
        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        if self.options.run_test:
            cmake.test(output_on_failure=True)

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["ion-core"]
