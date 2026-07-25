---
title: Constants
description: Public constants, flags, and symbolic values used by json-c.
---

# Constants

json-c keeps public constants focused on a small, readable surface.

## Current constant groups

- JSON value kinds
- serializer modes
- parser and helper error codes

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

## Notes

The API does not currently export version macros, parser flags, or validation-policy toggles. Some error codes are reserved for future expansion or non-parser helpers.
