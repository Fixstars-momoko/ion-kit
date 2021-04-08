from conans import ConanFile, CMake


class IonBbInternalConan(ConanFile):
    name = "ion-bb-internal"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Internal BB"
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"
