from conans import ConanFile, CMake, tools


class StdextpathConan(ConanFile):
    name = "stdext-path"
    version = "1.0"
    license = "http://github.com/srand/stdext-path/LICENSE"
    url = "http://github.com/srand/stdext-path"
    description = "stdext::path"
    settings = None
    exports_sources = "include/*"

    def package(self):
        self.copy("*.h", dst="include", src="include")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
