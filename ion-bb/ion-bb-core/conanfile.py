from conans import ConanFile


class IonBbCoreConan(ConanFile):
    name = "ion-bb-core"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Core BB"
    options = {"bb_depends": [True, False], "rt_depends": [True, False]}
    default_options = {"bb_depends": True, "rt_depends": True}
    exports_sources = "*"

    def requirements(self):
        if self.options.bb_depends:
            self.requires("ion-core/0.2.0")
        if self.options.rt_depends:
            self.requires("halide/[10.0.x]")
            self.requires("cpp-httplib/0.7.18")

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.hpp", dst="include")
        self.copy("*.cpp", dst="src")
        self.copy("*.cc", dst="src")
        self.copy("*.cmake")

    def package_info(self):
        self.cpp_info.srcdirs = ["src"]
