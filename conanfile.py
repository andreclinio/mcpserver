from conans import ConanFile
from conans import tools
from conans import AutoToolsBuildEnvironment
import shutil


class MacServerConan(ConanFile):
    name = "mcpserver"
    version = "0.0.2"
    settings = "os", "compiler", "build_type", "arch"
    generators = "make"
    author = "André Luiz Clinio (andre.clinio@gmail.com)"
    description = "Micro compiled webserver for C++"
    topics = ("web server", "C++ 11", "http")
    url = "https://github.com/andreclinio/mcpserver.git"
    license = "Open Source"
    exports_sources = ["*.mk", "makefile", "library/makefile", "library/src/*", "library/include/*"]


    def build(self):
        self._git_clone()
        with tools.chdir("mcpserver"):
            env_build = AutoToolsBuildEnvironment(self)
            env_build.make()
        shutil.move("mcpserver/library/lib", "lib")
        shutil.move("mcpserver/library/include", "include")
        shutil.rmtree("mcpserver")


    def package(self):
        self.copy("*.hpp", dst="include", src="library/include")
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
        