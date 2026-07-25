---
title: Validation
description: Syntax, UTF-8, depth, duplicate-key, and size validation policies.
---

# Validation

json-c treats validation as a first-class concern by surfacing syntax errors, escape problems, and exact input locations.

## Validation checks

- Syntax validation for malformed JSON
- UTF-8 conversion for `\uXXXX` escapes
- Unescaped control-character rejection inside strings
- Trailing-data rejection after a complete document
- Detailed error location reporting

## Error reporting

Every parse failure should provide:

- a stable error code
- a human-readable message
- a byte offset
- a line and column
- the current JSON path when available

## Recommended policy

- treat external input as untrusted
- check the returned error object before acting on a failure
- keep file handling and schema checks in application code
- reject or log malformed input instead of trying to continue

## What is not configurable yet

- strict versus relaxed parse modes
- duplicate-key policy toggles
- document-size limits through the public API
- caller-selectable parser flags
