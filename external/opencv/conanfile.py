from conans import ConanFile, tools
from conans.errors import ConanInvalidConfiguration

import os


class OpenCvConan(ConanFile):
    name = "opencv"
    version = "4.5.1"
    settings = "os", "arch"

    _url_list = {
        ("Linux", "x86_64"):
        "https://ion-archives.s3-us-west-2.amazonaws.com/genesis-runtime/OpenCV-4.5.1-x86_64-gcc75.sh",
        ("Linux", "armv8"):
        "https://ion-archives.s3-us-west-2.amazonaws.com/genesis-runtime/OpenCV-4.5.1-aarch64-gcc75.sh",
    }

    def _get_url(self) -> str:
        return self._url_list.get(
            (str(self.settings.os), str(self.settings.arch)), "")

    def validate(self):
        if not self._get_url():
            raise ConanInvalidConfiguration(
                'Binary does not exist for these settings')

    def build(self):
        tools.download(self._get_url(), "temp.sh")
        os.mkdir(os.path.join(self.build_folder, "install"))
        self.run("sh temp.sh --skip-license --prefix=install")

    def package(self):
        self.copy("*", src="install")

    def package_info(self):
        self.cpp_info.libs = [
            "opencv_calib3d", "opencv_core", "opencv_dnn", "opencv_features2d",
            "opencv_flann", "opencv_gapi", "opencv_highgui",
            "opencv_imgcodecs", "opencv_imgproc", "opencv_ml",
            "opencv_objdetect", "opencv_photo", "opencv_stitching",
            "opencv_video", "opencv_videoio"
        ]
        self.cpp_info.includedirs = ["include/opencv4"]
