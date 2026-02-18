---
title: Validation
description: Syntax, UTF-8, depth, duplicate-key, and size validation policies.
---

# Validation

json-c treats validation as a first-class concern so applications can decide how strict they want to be before accepting input from untrusted sources.

## Validation checks

- Syntax validation for malformed JSON
- UTF-8 validation for string content
- Maximum nesting depth
- Maximum document size
- Duplicate key detection
- Strict versus relaxed parse modes

## Error reporting

Every parse failure should provide:

- a stable error code
- a human-readable message
- a byte offset
- a line and column
- the current JSON path when available

## Recommended policy

- use strict mode for network input
- use relaxed mode only for trusted internal inputs
- set explicit depth limits for user-controlled documents
- reject duplicate keys when configuration correctness matters
