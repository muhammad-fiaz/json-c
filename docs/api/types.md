---
title: Types
description: Public types, enums, and handles exposed by json-c.
---

# Types

This page defines the core public types exposed by json-c.

## Core handles

- `jsonc_value`: opaque editable JSON node handle
- `jsonc_error`: parse error details
- `jsonc_allocator`: caller-supplied allocation callbacks used by serialization APIs
- `jsonc_format`: serializer mode selection

## Type model

json-c represents the standard JSON value set:

- null
- boolean
- integer
- number
- string
- array
- object

## Primitive value helpers

The public header exposes helpers for the current node access patterns:

- `jsonc_value_get_bool` and `jsonc_value_set_bool`
- `jsonc_value_get_string`
- `jsonc_object_get`
- `jsonc_object_set_bool`
- `jsonc_value_new_null`
- `jsonc_value_new_bool`
- `jsonc_value_new_string`

Arrays and objects are part of the DOM model, but only the helpers above are exported today.

## Return conventions

- functions that create values return a newly allocated owner handle
- lookup functions return borrowed pointers into the tree
- setter functions return `0` on success and non-zero on failure
- getter functions return `NULL` or a neutral value when the input is missing or of the wrong shape

## Ownership

Public values are caller-owned handles. Functions that create values transfer ownership to the caller, and destroy functions release the full subtree rooted at that value.
