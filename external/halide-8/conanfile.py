from conans import ConanFile, tools
from conans.errors import ConanInvalidConfiguration


class HalideConan(ConanFile):
    name = "halide"
    version = "8.0.0"
    settings = "os", "compiler", "arch"

    _url_list = {
        ("Windows", "x86"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-win-32-distro-800-65c26cba6a3eca2d08a0bccf113ca28746012cc3.zip",
        ("Windows", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-win-64-distro-800-65c26cba6a3eca2d08a0bccf113ca28746012cc3.zip",
        ("Macos", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-mac-64-800-65c26cba6a3eca2d08a0bccf113ca28746012cc3.tgz",
        ("Linux", "x86"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-linux-32-gcc53-800-65c26cba6a3eca2d08a0bccf113ca28746012cc3.tgz",
        ("Linux", "x86_64"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-linux-64-gcc53-800-65c26cba6a3eca2d08a0bccf113ca28746012cc3.tgz",
        ("Linux", "armv7hf"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-arm32-linux-32-trunk-65c26cba6a3eca2d08a0bccf113ca28746012cc3.tgz",
        ("Linux", "armv8"):
        "https://github.com/halide/Halide/releases/download/v8.0.0/halide-arm64-linux-64-trunk-65c26cba6a3eca2d08a0bccf113ca28746012cc3.tgz",
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
