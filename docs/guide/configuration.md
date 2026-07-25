---
title: Configuration
description: Parser, serializer, allocator, and validation configuration guidance for json-c.
---

# Configuration

json-c keeps configuration explicit. Most behavior is controlled by the values you pass into the API rather than by hidden globals.

## Parser configuration

The current public parser entry point accepts UTF-8 JSON text and an optional error object.

```c
jsonc_error error;
jsonc_value *value = jsonc_parse_string(text, &error);
```

Current configuration is driven by the library implementation and the validation rules it applies internally. In application code, treat parse input as trusted or untrusted and validate accordingly before storing or acting on the result.

## Serializer configuration

The serializer accepts a formatting mode.

```c
char *text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
```

Use compact mode when you need smaller output and pretty mode when the result is meant for humans.

## Allocator configuration

The public API includes a `jsonc_allocator` type and allocator parameter on the serializer for future expansion, but the current implementation uses its built-in allocation path.

## Validation configuration

The library reports invalid input through `jsonc_error`. Keep your validation policy strict for external input and use application-level schema checks when you need business rules beyond syntax.

## Best practice

- parse once
- validate early
- mutate in memory
- serialize at the edge of the system
- destroy every tree and buffer when done