---
title: C and C++
description: Using json-c from ISO C and C++ projects with the same public API.
---

# C and C++

json-c is written in ISO C and exposes a plain C API that can be used directly from both C and C++ projects.

## What the library supports

- Parsing JSON text into a tree of values
- Building JSON objects and arrays programmatically
- Reading and updating object members
- Serializing compact or pretty-printed JSON
- Reporting parse and validation errors
- Working with caller-owned memory and custom allocators

## What is exported today

json-c exports a focused set of operations for parsing, serialization, value lifecycle management, scalar inspection, array access, object lookup, and object mutation. See the [Operations guide](./operations) for the current function matrix.

Arrays and objects are first-class DOM nodes, and the public helper set now covers the common edit and inspection paths.

## C usage

Include the public header and use the API directly:

```c
#include <jsonc/jsonc.h>

int main(void) {
    jsonc_error error;
    jsonc_value *value = jsonc_parse_string("{\"name\":\"json-c\"}", &error);
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

## C++ usage

Use the same header from C++ and, if you prefer, wrap it in `extern "C"` when integrating with a larger codebase:

```cpp
extern "C" {
#include <jsonc/jsonc.h>
}

#include <iostream>

int main() {
    jsonc_error error{};
    jsonc_value *value = jsonc_parse_string("{\"enabled\":true}", &error);
    if (value == NULL) {
        std::cerr << error.message << '\n';
        return 1;
    }

    jsonc_value *enabled = jsonc_object_get(value, "enabled");
    if (enabled != NULL) {
        jsonc_value_set_bool(enabled, 0);
    }

    char *text = jsonc_stringify(value, JSONC_FORMAT_COMPACT, NULL);
    if (text != NULL) {
        std::cout << text << '\n';
        jsonc_free_string(text, NULL);
    }

    jsonc_value_destroy(value);
    return 0;
}
```

## Syntax and object model

The library uses a straightforward object model:

- JSON objects hold named members
- JSON arrays hold ordered items
- Strings are UTF-8 text values
- Numbers preserve JSON number syntax through parse and stringify
- Booleans and null map to dedicated value kinds

Common object operations include:

- parse a document
- retrieve a member by name
- set or replace boolean, integer, number, string, or null members
- read array length and item pointers
- serialize the result back to text
- destroy the full tree when finished

## Practical examples

### Read a field

```c
jsonc_value *name = jsonc_object_get(value, "name");
if (name != NULL) {
    const char *text = jsonc_value_get_string(name);
    puts(text);
}
```

### Update a field

```c
jsonc_object_set_bool(value, "enabled", 1);
```

### Read numbers

```c
jsonc_value *count = jsonc_object_get(value, "count");
if (count != NULL) {
    long integer_value = jsonc_value_get_integer(count);
    double number_value = jsonc_value_get_number(count);
    (void)integer_value;
    (void)number_value;
}
```

### Read and modify a string

```c
jsonc_value *name = jsonc_object_get(value, "name");
if (name != NULL) {
    const char *text = jsonc_value_get_string(name);
    if (text != NULL) {
        puts(text);
    }
}
```

## Supported workflow

json-c is intended for the full document lifecycle:

1. Parse input from memory.
2. Validate structure and content.
3. Inspect or mutate values.
4. Serialize output.
5. Release the tree and any allocated strings.

## Next references

- [Getting Started](./getting-started)
- [Validation](./validation)
- [Examples](../examples/)
- [Operations](./operations)
- [API Types](../api/types)
- [Parser API](../api/parser)
- [Serializer API](../api/export)