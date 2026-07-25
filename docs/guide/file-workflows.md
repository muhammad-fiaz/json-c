---
title: File Workflows
description: Reading and writing JSON files around json-c using C and C++ file APIs.
---

# File Workflows

json-c can work directly from file paths with `jsonc_parse_file`, `jsonc_parse_jsonl_file`, and `jsonc_write_file`. If you prefer manual file handling, you can still read and write text yourself and then use the string-based APIs.

## Read a JSON file in C

```c
#include <stdio.h>
#include <stdlib.h>
#include <jsonc/jsonc.h>

static char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    char *buffer;
    long length;

    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    length = ftell(file);
    if (length < 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    buffer = (char *)malloc((size_t)length + 1U);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    if (fread(buffer, 1, (size_t)length, file) != (size_t)length) {
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}
```

## Parse and write a file in C

```c
jsonc_error error;
jsonc_value *value;
char *json;

value = jsonc_parse_string(text, &error);
if (value == NULL) {
    /* inspect error.message, error.line, error.column, error.byte_offset */
}

json = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
if (json != NULL) {
    FILE *file = fopen("output.json", "wb");
    if (file != NULL) {
        fputs(json, file);
        fclose(file);
    }
    jsonc_free_string(json, NULL);
}

jsonc_value_destroy(value);
```

## Parse and write files with helpers

```c
jsonc_error error;
jsonc_value *value;

value = jsonc_parse_file("input.json", &error);
if (value == NULL) {
    /* inspect error.message, error.line, error.column, error.byte_offset */
}

if (jsonc_write_file("output.json", value, JSONC_FORMAT_PRETTY, &error) != 0) {
    /* inspect error.message */
}

jsonc_value_destroy(value);
```

## Read a JSON file in C++

```cpp
#include <fstream>
#include <sstream>
#include <string>
extern "C" {
#include <jsonc/jsonc.h>
}

std::ifstream input("input.json", std::ios::binary);
std::ostringstream buffer;
buffer << input.rdbuf();
std::string text = buffer.str();
```

## Close files explicitly

When you manage file handles yourself, always close them explicitly. json-c only operates on the in-memory text you provide.