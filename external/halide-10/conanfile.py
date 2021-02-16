from conans import ConanFile, tools
from conans.errors import ConanInvalidConfiguration


class HalideConan(ConanFile):
    name = "halide"
    version = "10.0.0"
    settings = "os", "compiler", "arch"

    _url_list = {
        ("Windows", "x86"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-x86-32-windows-db901f7f7084025abc3cbb9d17b0f2d3f1745900.zip",
        ("Windows", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-x86-64-windows-db901f7f7084025abc3cbb9d17b0f2d3f1745900.zip",
        ("Macos", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-x86-64-osx-db901f7f7084025abc3cbb9d17b0f2d3f1745900.tar.gz",
        ("Linux", "x86"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-x86-32-linux-db901f7f7084025abc3cbb9d17b0f2d3f1745900.tar.gz",
        ("Linux", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-x86-64-linux-db901f7f7084025abc3cbb9d17b0f2d3f1745900.tar.gz",
        ("Linux", "armv7hf"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-arm-32-linux-db901f7f7084025abc3cbb9d17b0f2d3f1745900.tar.gz",
        ("Linux", "armv8"):
        "https://github.com/halide/Halide/releases/download/v10.0.0/Halide-10.0.0-arm-64-linux-db901f7f7084025abc3cbb9d17b0f2d3f1745900.tar.gz",
    }

    def _get_url(self) -> str:
        return self._url_list.get(
            (str(self.settings.os), str(self.settings.arch)), "")

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
