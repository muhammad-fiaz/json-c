---
title: Serializer
description: Serialization and formatting API reference for json-c.
---

# Serializer

The serializer converts a DOM tree back into UTF-8 JSON text.

## Output modes

- compact output for transport
- pretty output for readability

## Function contract

```c
char *jsonc_stringify(const jsonc_value *value, jsonc_format format, const jsonc_allocator *allocator);
```

- `value`: the JSON tree to serialize
- `format`: `JSONC_FORMAT_COMPACT` or `JSONC_FORMAT_PRETTY`
- `allocator`: reserved for API symmetry; the current implementation uses the library default allocator
- return value: newly allocated NUL-terminated UTF-8 string or `NULL` on failure

## Freeing output

Use `jsonc_free_string` to release buffers returned by `jsonc_stringify`.

```c
char *text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
if (text != NULL) {
	jsonc_free_string(text, NULL);
}
```

## Typical usage

```c
char *text;

text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
```

## Ownership

Serialization functions that return text allocate the result with the library's current allocator path. The caller releases the buffer with `jsonc_free_string`.
