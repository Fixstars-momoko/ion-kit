from conans import ConanFile, CMake, tools
from conans.errors import ConanException
import shutil, os


class IonBbBase(object):
    settings = "os", "compiler", "build_type", "arch"
    options = {"enable_bb": [True, False], "enable_rt": [True, False]}
    default_options = {"enable_bb": True, "enable_rt": True}
    generators = "cmake"

    def _add_requires(self, name):
        reqs = getattr(self, name, None)
        if not reqs:
            return
        if not isinstance(reqs, (tuple, list)):
            reqs = reqs,
        for req in reqs:
            if isinstance(req, tuple):
                override = private = False
                ref = req[0]
                for elem in req[1:]:
                    if elem == "override":
                        override = True
                    elif elem == "private":
                        private = True
                    else:
                        raise ConanException("Unknown requirement config %s" % elem)
                self.requires(ref, private=private, override=override)
            else:
                self.requires(req)

    def requirements(self):
        if self.options.enable_bb:
            self._add_requires("bb_requires")
        if self.options.enable_rt:
            self._add_requires("rt_requires")

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
