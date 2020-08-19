from conans import ConanFile
from conans import tools
from conans import AutoToolsBuildEnvironment
import shutil
import re

def _get_version():
    with open('include/version.hpp', 'r') as f:
        for line in f:
            line = line.rstrip() # remove trailing whitespace such as '\n'
            if (line.startswith("#define MCP_LIBRARY_VERSION")):
                version = re.findall(r'"([^"]*)"', line)[0]
                return version

class McpServerConanFile(ConanFile):
    name = "mcpserver"
    version = _get_version()
    settings = "os", "compiler", "build_type", "arch"
    generators = "make"
    author = "André Luiz Clinio (andre.clinio@gmail.com)"
    description = "Micro compiled webserver for C++"
    topics = ("web server", "C++ 11", "http")
    url = "https://github.com/andreclinio/mcpserver.git"
    license = "Open Source"
    exports_sources = ["*.mk", "makefile", "src/*", "include/*"]


    def build(self):
        self.output.info("version read from source version.hpp: " + _get_version())
        env_build = AutoToolsBuildEnvironment(self)
        env_build.make()

    def package(self):
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
        
