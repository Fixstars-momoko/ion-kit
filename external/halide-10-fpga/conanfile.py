from conans import ConanFile


class HalideConan(ConanFile):
    name = "halide"
    version = "10.0.0"
    exports_sources = "*"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = ["Halide"]
        self.user_info.enable_fpga_backend = True
