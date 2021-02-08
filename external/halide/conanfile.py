from conans import ConanFile, tools
from conans.errors import ConanInvalidConfiguration


class HalideConan(ConanFile):
    name = "halide"
    version = "10.0.0"
    settings = "os", "compiler", "arch"

    def _get_url(self) -> str:
        os = None
        arch = None
        if self.settings.os == "Windows":
            os = "windows"
            if self.settings.arch == "x86":
                arch = "x86-32"
            elif self.settings.arch == "x86_64":
                arch = "x86-64"
        elif self.settings.os == "Macos":
            os = "osx"
            arch = "x86-64"
        elif self.settings.os == "Linux":
            os = "linux"
            if self.settings.arch == "x86":
                arch = "x86-32"
            elif self.settings.arch == "x86_64":
                arch = "x86-64"
            elif self.settings.arch == "armv7hf":
                arch = "arm-32"
            elif self.settings.arch == "armv8":
                arch = "arm-64"
        if os is None or arch is None:
            return ""
        else:
            return "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-{}-{}-db901f7f7084025abc3cbb9d17b0f2d3f1745900.{}".format(
                arch, os, 'zip' if os == 'Windows' else 'tar.gz')

    def validate(self):
        if self.settings.compiler == "gcc" and self.settings.compiler.libcxx != "libstdc++11":
            raise ConanInvalidConfiguration(
                'Requires "compiler.libcxx=libstdc++11"')
        if not self._get_url():
            raise ConanInvalidConfiguration(
                'Binary does not exist for these settings')

    def build(self):
        tools.get(self._get_url(), strip_root=True)

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = ["Halide"]
