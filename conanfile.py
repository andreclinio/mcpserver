from conans import ConanFile
from conans import tools
from conans import AutoToolsBuildEnvironment
import shutil
import re
import os

def _get_version():
    script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in
    rel_path = "include/version.hpp"
    abs_file_path = os.path.join(script_dir, rel_path)
    with open(abs_file_path, 'r') as f:
        for line in f:
            line = line.rstrip() # remove trailing whitespace such as '\n'
            if (line.startswith("#define MCP_LIBRARY_VERSION")):
                version = re.findall(r'"([^"]*)"', line)[0]
                return version

class McpServerConanFile(ConanFile):
    name = "mcpserver"
    version = "0.0.1-snapshot"
    settings = "os", "compiler", "build_type", "arch"
    generators = "make"
    author = "André Luiz Clinio (andre.clinio@gmail.com)"
    description = "Micro compiled webserver for C++"
    topics = ("web server", "C++ 11", "http")
    url = "https://github.com/andreclinio/mcpserver.git"
    license = "Open Source"
    exports_sources = ["*.mk", "makefile", "src/*", "include/*", "conanfile.py"]


    def build(self):
        self.output.info("Version read from source version.hpp: " + _get_version())
        env_build = AutoToolsBuildEnvironment(self)
        env_build.make()

    def package(self):
        self.copy("conanfile.py", dst=".", src=".")
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["mcpserver"]

    @property
    def _make_program(self):
        if self.settings.compiler == "Visual Studio":
            return "nmake"
        make_program = tools.get_env("CONAN_MAKE_PROGRAM", tools.which("make") or tools.which('mingw32-make'))
        make_program = tools.unix_path(make_program) if tools.os_info.is_windows else make_program
        return make_program

    def _git_clone(self):
        command = " ".join(["git", "clone", self.url])
        self.run(command)
        delete_dirs = [".git", ".vscode", ".github", "attic", "samples"]
        for dir in delete_dirs:            
            tools.rmdir(dir)
        
