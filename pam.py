from build.model import *
from externals.googletest import googletest
from toolchains.pam import *


stdext_path = cxx_project(
    name = "stdext-path",
    incpaths = [("include", {"publish": True})]
)

stdext_path_test = cxx_executable(
    name = "stdext-path-test",
    sources = ["test/test.cpp"],
    dependencies = [stdext_path, googletest],
    features = ["language-c++11"]
)
