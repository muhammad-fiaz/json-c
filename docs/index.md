---
layout: home

hero:
  name: JSON-C
  text: Portable JSON for C and C++
  tagline: Lightweight, modular, dependency-free, and built around explicit ownership, predictable parsing, and clear diagnostics.
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
    title: Parse and serialize
    details: Turn UTF-8 JSON text into a DOM tree, inspect it, change it, and serialize it back to compact or pretty output.
  - icon: 🧩
    title: Small public surface
    details: The API currently focuses on value lifecycles, boolean updates, string inspection, object lookup, and constructors.
  - icon: 🛡️
    title: Clear diagnostics
    details: Parse failures report a code, message, line, column, byte offset, and JSON path when available.
  - icon: 📦
    title: Portable C and C++
    details: The library builds as ISO C and can be included directly from C++ projects.
  - icon: 🔁
    title: Explicit ownership
    details: Returned values and serialized strings follow simple ownership rules that are easy to audit.
  - icon: 📚
    title: Documentation-first
    details: The guides cover getting started, file workflows, API contracts, validation, performance, and contribution notes.
---

## What it supports

- Parsing JSON text from memory with detailed error reporting.
- Inspecting booleans and strings in parsed DOM trees.
- Updating boolean members on JSON objects.
- Creating null, boolean, and string values programmatically.
- Serializing values back to compact or pretty JSON text.
- Releasing values and serialized buffers with explicit ownership rules.

## Quick example

```c
#include <jsonc/jsonc.h>

int main(void) {
    jsonc_error error;
    jsonc_value *value = jsonc_parse_string("{\"enabled\":true,\"name\":\"json-c\"}", &error);
    if (value == NULL) {
        return 1;
    }

    jsonc_value_set_bool(jsonc_object_get(value, "enabled"), 0);

    char *text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
    if (text != NULL) {
        puts(text);
        jsonc_free_string(text, NULL);
    }

    jsonc_value_destroy(value);
    return 0;
}
```

## Where to go next

- Read the [Getting Started](guide/getting-started) guide for the shortest path from text to DOM and back.
- Review the [API Overview](api/) for the current public surface.
- Check the [Examples](examples/) page for copy-ready usage patterns.
