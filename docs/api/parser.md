---
title: Parser
description: Parsing API reference for json-c.
---

# Parser

The parser turns UTF-8 JSON text into an editable DOM tree.

## Parse from memory

```c
jsonc_error error;
jsonc_value *value;

value = jsonc_parse_string(text, &error);
```

## Parse options

- input: UTF-8 encoded JSON text
- output: owned `jsonc_value *` tree on success
- error: optional `jsonc_error *` populated on failure
- allocator: parse allocations are currently handled internally by the library

## Error details

When parsing fails, the error object can report:

- numeric error code
- human-readable message
- line
- column
- byte offset
- JSON path when available

## Supported parse results

The parser recognizes:

- objects
- arrays
- strings
- numbers
- booleans
- null

## Error handling

On failure, parsing functions return NULL and populate the supplied error object when one is provided.

Current parser failures typically use these codes:

- `JSONC_ERROR_INVALID_SYNTAX`
- `JSONC_ERROR_UNEXPECTED_EOF`
- `JSONC_ERROR_NO_MEMORY`

The parser also tracks a JSON path while it descends into objects and arrays so diagnostics can point at the failing location.
