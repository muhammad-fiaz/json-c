---
title: Installation
description: Build and package json-c with CMake, pkg-config, and common package managers.
---

# Installation

json-c is intended to be easy to consume from source trees, package managers, and superbuilds.

## Build from source

```bash
cmake -S . -B build -DJSONC_BUILD_TESTS=ON
cmake --build build
cmake --install build
```

## CMake integration

```cmake
find_package(json-c CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE jsonc::jsonc)
```

## pkg-config

```bash
pkg-config --cflags --libs json-c
```

## Package managers

- vcpkg: planned package layout and port file
- Conan: planned recipe and package metadata
- Homebrew: planned formula support
- MSYS2: planned mingw packaging

## Source layout

- include/jsonc/ for public headers
- src/ for implementation
- tests/ for unit and regression coverage
- examples/ for compile-tested usage samples
