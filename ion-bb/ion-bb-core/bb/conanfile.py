from conans import ConanFile


class IonBbCoreBbConan(ConanFile):
    name = "ion-bb-core-bb"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "BB implementation of ion-bb-core"
    generators = "cmake"
    exports_sources = "*"
    requires = "ion-core/0.1.0"

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.cpp", dst="src")

    def package_info(self):
        self.cpp_info.srcdirs = ["src"]
