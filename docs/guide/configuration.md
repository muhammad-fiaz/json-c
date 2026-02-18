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

The serializer accepts a formatting mode and an optional allocator.

```c
char *text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
```

Use compact mode when you need smaller output and pretty mode when the result is meant for humans.

## Allocator configuration

If you need custom memory behavior, provide a `jsonc_allocator` with matching `malloc_fn` and `free_fn` callbacks.

```c
jsonc_allocator allocator;
allocator.malloc_fn = custom_malloc;
allocator.free_fn = custom_free;
allocator.user_data = state;
```

## Validation configuration

The library reports invalid input through `jsonc_error`. Keep your validation policy strict for external input and use relaxed handling only for trusted internal data.

## Best practice

- parse once
- validate early
- mutate in memory
- serialize at the edge of the system
- destroy every tree and buffer when done