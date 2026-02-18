<div align="center">
	<p>
		<a href="https://muhammad-fiaz.github.io/json-c/"><img src="https://img.shields.io/badge/docs-muhammad--fiaz.github.io-blue" alt="Documentation"></a>
		<a href="https://github.com/muhammad-fiaz/json-c/actions/workflows/ci.yml"><img src="https://github.com/muhammad-fiaz/json-c/actions/workflows/ci.yml/badge.svg" alt="CI"></a>
		<a href="https://github.com/muhammad-fiaz/json-c/actions/workflows/deploy.yml"><img src="https://github.com/muhammad-fiaz/json-c/actions/workflows/deploy.yml/badge.svg" alt="Docs Deploy"></a>
		<a href="https://github.com/muhammad-fiaz/json-c"><img src="https://img.shields.io/github/stars/muhammad-fiaz/json-c" alt="GitHub stars"></a>
		<a href="https://github.com/muhammad-fiaz/json-c/issues"><img src="https://img.shields.io/github/issues/muhammad-fiaz/json-c" alt="GitHub issues"></a>
		<a href="https://github.com/muhammad-fiaz/json-c/pulls"><img src="https://img.shields.io/github/issues-pr/muhammad-fiaz/json-c" alt="GitHub pull requests"></a>
		<a href="https://github.com/muhammad-fiaz/json-c"><img src="https://img.shields.io/github/last-commit/muhammad-fiaz/json-c" alt="GitHub last commit"></a>
		<a href="https://github.com/muhammad-fiaz/json-c/blob/main/LICENSE"><img src="https://img.shields.io/github/license/muhammad-fiaz/json-c" alt="License"></a>
		<img src="https://img.shields.io/badge/platforms-linux%20%7C%20windows%20%7C%20macos-blue" alt="Supported Platforms">
		<a href="https://pay.muhammadfiaz.com"><img src="https://img.shields.io/badge/Sponsor-pay.muhammadfiaz.com-ff69b4?style=flat&logo=heart" alt="Sponsor"></a>
	</p>

	<p><em>A lightweight, modular, dependency-free JSON library for ISO C.</em></p>

	<p>
		<b><a href="https://muhammad-fiaz.github.io/json-c/">Documentation</a> |
		<a href="https://muhammad-fiaz.github.io/json-c/api/parser">API Reference</a> |
		<a href="https://muhammad-fiaz.github.io/json-c/guide/getting-started">Quick Start</a> |
		<a href="CONTRIBUTING.md">Contributing</a></b>
	</p>
</div>

`json-c` is a modern C JSON library with a small public surface, predictable allocation behavior, and a focus on portability. The codebase is being developed as a modular foundation for parsing, DOM editing, serialization, validation, and future streaming integrations.

> [!TIP]
> If you use json-c in a project, consider starring the repository and reading the documentation before wiring it into production code.

> [!NOTE]
> **Project status:** this repository is actively evolving. The initial goal is a clean, well-documented, and production-focused C foundation rather than a copy of any existing JSON library.

## Links

- Website: [muhammadfiaz.com](https://muhammadfiaz.com)
- GitHub: [muhammad-fiaz](https://github.com/muhammad-fiaz)
- LinkedIn: [Muhammad Fiaz](https://www.linkedin.com/in/muhammad-fiaz-)
- X: [@muhammadfiaz_](https://x.com/muhammadfiaz_)

## Highlights

- ISO C public API with explicit ownership rules
- DOM and serialization layers designed for reuse
- UTF-8 aware parsing and escaping
- Portable CMake build with package files
- VitePress documentation with SEO metadata and GitHub Pages hosting

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

## License

MIT License - see [LICENSE](LICENSE) for details.
