from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class JsonCRecipe(ConanFile):
    name = "json-c"
    version = "0.1.0"
    license = "MIT"
    url = "https://github.com/muhammad-fiaz/json-c"
    description = "Lightweight, modular, dependency-free JSON library for ISO C and C++"
    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "CMakeLists.txt", "cmake/*", "include/*", "src/*", "examples/*", "tests/*"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        toolchain = CMakeToolchain(self)
        toolchain.variables["JSONC_BUILD_TESTS"] = False
        toolchain.variables["JSONC_BUILD_EXAMPLES"] = False
        toolchain.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["jsonc"]