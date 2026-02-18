<div align="center">

[![Documentation](https://img.shields.io/badge/docs-muhammad--fiaz.github.io-blue)](https://muhammad-fiaz.github.io/json-c/)
[![CI](https://github.com/muhammad-fiaz/json-c/actions/workflows/ci.yml/badge.svg)](https://github.com/muhammad-fiaz/json-c/actions/workflows/ci.yml)
[![Docs Deploy](https://github.com/muhammad-fiaz/json-c/actions/workflows/deploy.yml/badge.svg)](https://github.com/muhammad-fiaz/json-c/actions/workflows/deploy.yml)
[![License](https://img.shields.io/github/license/muhammad-fiaz/json-c)](LICENSE)
[![Platforms](https://img.shields.io/badge/platforms-linux%20%7C%20windows%20%7C%20macos-blue)]()

**A lightweight, modular, dependency-free JSON library for ISO C.**

[Documentation](https://muhammad-fiaz.github.io/json-c/) · [API Reference](https://muhammad-fiaz.github.io/json-c/api/) · [Quick Start](https://muhammad-fiaz.github.io/json-c/guide/getting-started) · [Contributing](CONTRIBUTING.md) · [Security](SECURITY.md)

</div>

json-c is a lightweight, super fast, modular, dependency-free JSON library for C/C++. It has a small public surface, predictable allocation behavior, and a focus on portability. The codebase is being developed as a modular foundation for parsing, DOM editing, serialization, validation, and future streaming integrations.

> [!TIP]
> If you use json-c in a project, consider starring the repository and reading the documentation before wiring it into production code.

> [!NOTE]
> **Project status:** this repository is actively evolving. The initial goal is a clean, well-documented, and production-focused C foundation rather than a copy of any existing JSON library.

## Highlights

- ISO C public API with explicit ownership rules
- DOM and serialization layers designed for reuse
- UTF-8 aware parsing and escaping
- Portable CMake build with package files
- VitePress documentation with SEO metadata and GitHub Pages hosting
- Conan, vcpkg, and xmake support through repository manifests
- Broad C and C++ compiler compatibility across modern toolchains

## Quick Build

```bash
cmake -S . -B build -DJSONC_BUILD_TESTS=ON -DJSONC_BUILD_EXAMPLES=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Quick Example

```c
#include <jsonc/jsonc.h>

int main(void) {
		jsonc_error error;
		jsonc_value *value = jsonc_parse_string("{\"ok\":true}", &error);
		if (value == NULL) {
				return 1;
		}

		char *text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
		if (text != NULL) {
				puts(text);
				jsonc_free_string(text, NULL);
		}

		jsonc_value_destroy(value);
		return 0;
}
```

## Documentation

- [Getting Started](https://muhammad-fiaz.github.io/json-c/guide/getting-started)
- [Installation](https://muhammad-fiaz.github.io/json-c/guide/installation)
- [Architecture](https://muhammad-fiaz.github.io/json-c/guide/architecture)
- [Validation](https://muhammad-fiaz.github.io/json-c/guide/validation)
- [FAQ](https://muhammad-fiaz.github.io/json-c/guide/faq)

## Repository Policies

- [Contributing](CONTRIBUTING.md)
- [Security](SECURITY.md)

## Supported Toolchains

json-c is intended to build cleanly with mainstream C and C++ toolchains, including C89/C99/C11/C17/C23 and C++98/C++03/C++11/C++14/C++17/C++20/C++23 where the compiler and standard library support the chosen language mode.

## Package Managers

- Conan: `conanfile.py`
- vcpkg: `vcpkg.json`
- xmake: `xmake.lua`

## License

MIT License - see [LICENSE](LICENSE) for details.
