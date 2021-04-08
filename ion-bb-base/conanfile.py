from conans import ConanFile, CMake, tools
import shutil, os


class IonBbBase(object):
    options = {"enable_bb": [True, False], "enable_rt": [True, False]}
    default_options = {"enable_bb": True, "enable_rt": True}
    generators = "cmake"
    cmake_definitions = {}
    bb_requires = {}

    def _get_list(self, name):
        l = getattr(self, name, None)
        if not l:
            return ()
        if isinstance(l, (tuple, list)):
            return tuple(l)
        else:
            return l,

    def requirements(self):
        if self.options.enable_bb:
            for name in self._get_list("bb_requires"):
                self.requires(name)
        if self.options.enable_rt:
            for name in self._get_list("rt_requires"):
                self.requires(name)

    def build(self):
        shutil.copy(
            os.path.join(self.python_requires["ion-bb-base"].path,
                         "CMakeLists.txt"), self.source_folder)
        cmake = CMake(self)
        cmake.definitions["BUILD_BB"] = self.options.enable_bb
        cmake.definitions["BUILD_RT"] = self.options.enable_rt
        cmake.definitions["BB_NAME"] = self.name
        if hasattr(self, "cmake_definitions") and isinstance(
                self.cmake_definitions, dict):
            cmake.definitions.update(self.cmake_definitions)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", src="include")
        self.copy("*.hpp", src="include")
        self.copy("*.so")
        self.copy("*.a")
        self.copy("*.dll")
        self.copy("*.lib")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)


class IonBbBaseConan(ConanFile):
    name = "ion-bb-base"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Base definition for ion-bb"
    exports = "CMakeLists.txt"
