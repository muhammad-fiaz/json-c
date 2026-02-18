---
title: Errors
description: Error codes and diagnostics returned by json-c.
---

# Errors

json-c error objects are meant to be useful in logs, telemetry, and user-facing diagnostics.

## Fields

- `code`: numeric `jsonc_error_code`
- `message`: human-readable description
- `line`: 1-based line number
- `column`: 1-based column number
- `byte_offset`: 0-based byte offset into the original input
- `path`: JSON path when available

## Error codes

- `JSONC_ERROR_NONE`
- `JSONC_ERROR_INVALID_SYNTAX`
- `JSONC_ERROR_UNEXPECTED_EOF`
- `JSONC_ERROR_UNSUPPORTED`
- `JSONC_ERROR_DEPTH_LIMIT`
- `JSONC_ERROR_NO_MEMORY`
- `JSONC_ERROR_TYPE_MISMATCH`
- `JSONC_ERROR_NOT_FOUND`

## Failure model

Public functions should fail explicitly rather than silently dropping malformed input or partially mutating a document.
