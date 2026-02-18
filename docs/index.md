---
layout: home

hero:
  name: json-c
  text: Production-grade JSON for ISO C
  tagline: Lightweight, modular, dependency-free, and designed for long-term maintainability.
  actions:
    - theme: brand
      text: Get Started
      link: /guide/getting-started
    - theme: alt
      text: API Reference
      link: /api/parser
    - theme: alt
      text: View on GitHub
      link: https://github.com/muhammad-fiaz/json-c

features:
  - icon: ⚡
    title: Fast parsing
    details: Focused on low-allocation parsing and predictable performance for embedded and server workloads.
  - icon: 🧩
    title: Modular design
    details: Clear separation between parser, DOM, serializer, validation, memory, and utility layers.
  - icon: 🛡️
    title: Defensive by default
    details: Explicit error reporting, depth limits, allocation checks, and UTF-8 validation hooks.
  - icon: 📦
    title: Portable C
    details: Written for mainstream ISO C toolchains without compiler-specific extensions.
  - icon: 🔁
    title: Streaming-ready
    details: Designed for incremental parsing, SAX-style integration, and large document processing.
  - icon: 📚
    title: Thoroughly documented
    details: Doxygen-friendly API docs, configuration guidance, file workflows, and Markdown guides for users, integrators, and contributors.
---

## What json-c is

json-c is a C JSON library aimed at production software that needs a small binary footprint, stable APIs, and a clean maintenance surface.

It is intended for:

- embedded systems
- CLI tools
- desktop applications
- mobile applications
- web services
- game engines
- systems software

## Design goals

- Keep the public API explicit and easy to audit.
- Avoid hidden global state.
- Support both compact and pretty serialization.
- Provide strong diagnostics with line, column, byte offset, and JSON path context.
- Keep memory management under caller control.

## Quick example

```c
#include <jsonc/jsonc.h>

int main(void) {
    jsonc_error error;
    jsonc_value *value;
    char *text;

    value = jsonc_parse_string("{\"name\":\"json-c\"}", &error);
    if (value == NULL) {
        return 1;
    }

    text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
    if (text != NULL) {
        /* write text to stdout or a file */
        jsonc_free_string(text, NULL);
    }

    jsonc_value_destroy(value);
    return 0;
}
```

## Status

This repository is being reorganized around a C-first architecture, VitePress documentation, GitHub Pages publishing, and a portable build workflow.
