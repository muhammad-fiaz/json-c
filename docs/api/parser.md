---
title: Parser
description: Parsing API reference for json-c.
---

# Parser

The parser turns UTF-8 JSON text into an editable DOM tree or a streamed event sequence.

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
- allocator: allocation behavior is controlled by the library and the caller-selected allocator in serialization paths

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

On failure, parsing functions should return NULL and populate the supplied error object when one is provided.
