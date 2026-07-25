---
title: Installation
description: Build and package json-c with CMake, pkg-config, and common package managers.
---

# Installation

json-c is designed to be easy to consume from source trees, package managers, and superbuilds.

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

- vcpkg: manifest support via `vcpkg.json`
- Conan: recipe support via `conanfile.py`
- xmake: build and package support via `xmake.lua`

## Language support

json-c is intended to compile with widely used C and C++ language modes:

- C89 / ANSI C
- C99
- C11
- C17
- C23
- C++98
- C++03
- C++11
- C++14
- C++17
- C++20
- C++23

If you need stricter portability, keep to the subset required by your compiler and target platform.

## Source layout

- include/jsonc/ for public headers
- src/ for implementation
- tests/ for unit and regression coverage
- examples/ for compile-tested usage samples
- docs/ for the VitePress documentation site
