from conans import ConanFile, CMake


class IonBbCoreConan(ConanFile):
    name = "ion-bb-core"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Core BB"
    options = {"enable_bb": [True, False], "enable_rt": [True, False]}
    default_options = {"enable_bb": True, "enable_rt": True}
    generators = "cmake"
    exports_sources = "*"

    def requirements(self):
        if self.options.enable_bb:
            self.requires("ion-core/0.2.0")
        if self.options.enable_rt:
            self.requires("halide/[10.0.x]")
            self.requires("cpp-httplib/0.7.18")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_BB"] = self.options.enable_bb
        cmake.definitions["BUILD_RT"] = self.options.enable_rt
        cmake.definitions["BB_NAME"] = self.name
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h")
        self.copy("*.hpp")
        self.copy("*.so")
        self.copy("*.a")
        self.copy("*.dll")
        self.copy("*.lib")

    def package_info(self):
        if self.options.enable_bb:
            self.cpp_info.libs.append(self.name + "-bb")
        if self.options.enable_rt:
            self.cpp_info.libs.append(self.name + "-rt")
