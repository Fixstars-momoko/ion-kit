from conans import ConanFile, CMake


class IonBbImageProcessingConan(ConanFile):
    name = "ion-bb-image-processing"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Image processing BB"
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"
