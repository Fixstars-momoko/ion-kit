from conans import ConanFile


class IonBbCoreConan(ConanFile):
    name = "ion-bb-core"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Core BB"
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"
    rt_requires = "halide/[10.0.x]", "cpp-httplib/0.7.18"
