from conans import ConanFile, tools
from conans.errors import ConanException


class IonBbImageIoConan(ConanFile):
    name = "ion-bb-image-io"
    version = "1.0.0"
    license = "MIT"
    url = "https://github.com/fixstars/ion-kit"
    description = "Image I/O BB"
    options = {
        "enable_realsense": [True, False],
        "enable_runtime": [True, False]
    }
    default_options = {"enable_realsense": False, "enable_runtime": False}
    generators = "cmake"
    exports_sources = "*"
    python_requires = "ion-bb-base/1.0.0"
    python_requires_extend = "ion-bb-base.IonBbBase"
    bb_requires = "ion-core/0.2.0"
    rt_requires = "halide/[10.0.x]", "cpp-httplib/0.7.18", "opencv/4.5.1"

    def init(self):
        self.options.update(super().options)
        self.default_options.update(super().default_options)

    def system_requirements(self):
        if self.options.enable_rt and self.options.enable_realsense:
            repository_url = None
            if tools.os_info.linux_distro == "ubuntu":
                repository_list = {
                    "16.04":
                    "deb https://librealsense.intel.com/Debian/apt-repo xenial main",
                    "18.04":
                    "deb https://librealsense.intel.com/Debian/apt-repo bionic main",
                    "20.04":
                    "deb https://librealsense.intel.com/Debian/apt-repo focal main"
                }
                repository_url = repository_list.get(
                    str(tools.os_info.os_version), None)
            if repository_url is None:
                raise ConanException(
                    "Realsense library is supported on Ubuntu LTS")

            packages = ["librealsense2-utils", "librealsense2-dev"]
            if self.options.enable_runtime:
                packages.append("librealsense2-dkms")

            package_tool = tools.SystemPackageTool(conanfile=self)
            if not all(map(package_tool.installed, packages)):
                # Workaround of add_repository bug
                # package_tool.add_repository(repository_url, "http://keyserver.ubuntu.com/pks/lookup?op=get&search=0xF6E65AC044F831AC80A06380C8B3A55A6F3EFCDE")
                self.run(
                    "sudo apt-key adv --keyserver keys.gnupg.net --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE || sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE"
                )
                self.run(f'sudo add-apt-repository "{repository_url}"')
                package_tool.install_packages(packages)

    def build(self):
        self.cmake_definitions = {
            "ENABLE_REALSENSE": self.options.enable_realsense
        }
        super().build()
