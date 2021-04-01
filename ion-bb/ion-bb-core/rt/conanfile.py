from conans import ConanFile


class IonBbCoreBbConan(ConanFile):
    name = "ion-bb-core-rt"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Runtime implementation of ion-bb-core"
    generators = "cmake"
    exports_sources = "*"
    requires = "halide/[8.0.x]", "cpp-httplib/0.7.18"

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.cpp", dst="src")

    def package_info(self):
        self.cpp_info.srcdirs = ["src"]
