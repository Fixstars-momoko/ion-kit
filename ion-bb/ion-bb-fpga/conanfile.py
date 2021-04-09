from conans import ConanFile, CMake
from conans.errors import ConanException


class IonBbFpgaConan(ConanFile):
    name = "ion-bb-fpga"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "FPGA BB"
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"

    def build(self):
        if self.options.enable_bb and getattr(self.deps_user_info["halide"],
                                              "enable_fpga_backend",
                                              None) != "True":
            raise ConanException("This BB requires FPGA backend")
        super().build()
