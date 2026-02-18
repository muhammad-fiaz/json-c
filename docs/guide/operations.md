---
title: Operations
description: Supported json-c operations, return values, and ownership rules.
---

# Operations

json-c focuses on a small set of operations that are available today. Anything not listed here should be treated as application logic or a future extension.

## Supported operations

| Operation | API | Input | Output | Notes |
| --- | --- | --- | --- | --- |
| Parse JSON text | `jsonc_parse_string` | UTF-8 text, optional error object | Owned `jsonc_value *` or `NULL` | Populates diagnostics on failure |
| Serialize JSON | `jsonc_stringify` | Value, format, optional allocator | Owned string or `NULL` | Use `jsonc_free_string` to release |
| Destroy value tree | `jsonc_value_destroy` | Owned value | None | Frees the full subtree |
| Inspect type | `jsonc_value_type` | Value | `jsonc_type` | Returns `JSONC_TYPE_NULL` for `NULL` input |
| Read boolean | `jsonc_value_get_bool` | Boolean node | Integer boolean | Returns non-zero for true |
| Write boolean | `jsonc_value_set_bool` | Boolean node, integer boolean | Success code | Returns non-zero on type mismatch |
| Read string | `jsonc_value_get_string` | String node | `const char *` | Pointer remains owned by the tree |
| Lookup member | `jsonc_object_get` | Object node, key | Member node or `NULL` | Borrowed pointer |
| Set boolean member | `jsonc_object_set_bool` | Object node, key, integer boolean | Success code | Adds or replaces a member |
| Create null | `jsonc_value_new_null` | None | Owned `jsonc_value *` | Caller owns the result |
| Create boolean | `jsonc_value_new_bool` | Integer boolean | Owned `jsonc_value *` | Caller owns the result |
| Create string | `jsonc_value_new_string` | UTF-8 text | Owned `jsonc_value *` | Caller owns the result |

## What applications must handle

- opening and closing files
- reading file contents into memory
- writing serialized text back to disk
- merging documents
- array insertion and deletion beyond current exported helpers
- object member removal beyond current exported helpers
- custom business rules and schema checks

## Return patterns

- pointer return values are either owned handles or borrowed pointers, depending on the function
- integer return values use `0` for success and non-zero for failure in setter functions
- lookup helpers return `NULL` when the key or node is missing

## Practical guidance

Prefer the smallest operation that matches your goal. Parse once, inspect or mutate in memory, serialize once, and destroy the tree when finished.