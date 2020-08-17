from conans import ConanFile
from conans import tools

class MacServerConan(ConanFile):
    name = "mcpserver"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "make"
    author = "André Luiz Clinio (andre.clinio@gmail.com)"
    description = "Micro compiled webserver for C++"
    topics = ("web server", "C++ 11", "http")
    url = "https://github.com/andreclinio/mcpserver.git"
    license = "Open Source"

    def source(self):
        command = " ".join(["git", "clone", self.url, "."])
        self.run(command)
        self._delete_post_cloned()
        
    def build(self):
        self.run(" ".join(["git", "clone", self.url]))
        with tools.chdir("mcpserver"):
            self._delete_post_cloned()
            self.run(" ".join([self._make_program, "build"]))

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

    def _delete_post_cloned(self):
        delete_dirs = [".git", ".vscode", ".github", "attic", "samples"]
        for dir in delete_dirs:            
            tools.rmdir(dir)
