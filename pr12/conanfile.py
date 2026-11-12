# This file is managed by Conan, contents will be overwritten.
# To keep your changes, remove these comment lines, but the plugin won't be able to modify your requirements

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain

class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()



    def requirements(self):
        # Solo descarga las dependencias si el sistema NO es macOS
        if self.settings.os != "Macos":
            self.requires("freeglut/3.4.0")
            self.requires("opengl/system")
            self.requires("glu/system")
            self.requires("opengl-registry/cci.20220929")
            self.requires("khrplatform/cci.20200529")
