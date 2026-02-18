---
title: Constants
description: Public constants, flags, and symbolic values used by json-c.
---

# Constants

json-c keeps public constants focused on readable, stable configuration.

## Expected constant groups

- version macros
- error codes
- serializer modes
- parser flags
- validation policies

## Current public constants

- `JSONC_TYPE_NULL`
- `JSONC_TYPE_BOOLEAN`
- `JSONC_TYPE_INTEGER`
- `JSONC_TYPE_NUMBER`
- `JSONC_TYPE_STRING`
- `JSONC_TYPE_ARRAY`
- `JSONC_TYPE_OBJECT`
- `JSONC_FORMAT_COMPACT`
- `JSONC_FORMAT_PRETTY`
- `JSONC_ERROR_NONE`
- `JSONC_ERROR_INVALID_SYNTAX`
- `JSONC_ERROR_UNEXPECTED_EOF`
- `JSONC_ERROR_UNSUPPORTED`
- `JSONC_ERROR_DEPTH_LIMIT`
- `JSONC_ERROR_NO_MEMORY`
- `JSONC_ERROR_TYPE_MISMATCH`
- `JSONC_ERROR_NOT_FOUND`

## Stability note

Constants that become part of the public API should remain stable once released in a 1.0 series.
