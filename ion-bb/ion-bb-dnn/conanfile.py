from conans import ConanFile, tools
from conans.errors import ConanException


class IonBbDnnConan(ConanFile):
    name = "ion-bb-dnn"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "DNN BB"
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"
    rt_requires = "halide/[10.0.x]", "cpp-httplib/0.7.18", "nlohmann_json/3.9.1", "opencv/4.5.1", "openssl/1.1.1k", "onnx/1.8.1"
