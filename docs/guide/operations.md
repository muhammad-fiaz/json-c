---
title: Operations
description: Supported json-c operations, return values, and ownership rules.
---

# Operations

json-c focuses on a small set of operations that are available today. Anything not listed here should be treated as application logic.

## Supported operations

| Operation | API | Input | Output | Notes |
| --- | --- | --- | --- | --- |
| Parse JSON text | `jsonc_parse_string` | UTF-8 text, optional error object | Owned `jsonc_value *` or `NULL` | Populates diagnostics on failure |
| Parse JSON file | `jsonc_parse_file` | File path, optional error object | Owned `jsonc_value *` or `NULL` | Reads the full file into memory |
| Parse JSON Lines | `jsonc_parse_jsonl_string` | UTF-8 JSONL text, optional error object | Owned array `jsonc_value *` or `NULL` | Blank lines are ignored |
| Parse JSON Lines file | `jsonc_parse_jsonl_file` | File path, optional error object | Owned array `jsonc_value *` or `NULL` | Reads the full file into memory |
| Serialize JSON | `jsonc_stringify` | Value, format, optional allocator placeholder | Owned string or `NULL` | Use `jsonc_free_string` to release |
| Write JSON file | `jsonc_write_file` | File path, value, format, optional error object | Success code | Serializes and writes the result |
| Destroy value tree | `jsonc_value_destroy` | Owned value | None | Frees the full subtree |
| Inspect type | `jsonc_value_type` | Value | `jsonc_type` | Returns `JSONC_TYPE_NULL` for `NULL` input |
| Read boolean | `jsonc_value_get_bool` | Boolean node | Integer boolean | Returns non-zero for true |
| Write boolean | `jsonc_value_set_bool` | Boolean node, integer boolean | Success code | Returns non-zero on type mismatch |
| Read integer | `jsonc_value_get_integer` | Integer node | Integer value | Returns `0` for `NULL` or non-integer input |
| Read number | `jsonc_value_get_number` | Number node | Floating-point value | Returns `0.0` for `NULL` or non-number input |
| Read string | `jsonc_value_get_string` | String node | `const char *` | Pointer remains owned by the tree |
| Read array size | `jsonc_array_size` | Array node | Element count | Returns `0` for `NULL` or non-array input |
| Read array item | `jsonc_array_get` | Array node, index | Borrowed item or `NULL` | Returns `NULL` when out of range |
| Lookup member | `jsonc_object_get` | Object node, key | Member node or `NULL` | Borrowed pointer |
| Set boolean member | `jsonc_object_set_bool` | Object node, key, integer boolean | Success code | Adds or replaces a member |
| Set null member | `jsonc_object_set_null` | Object node, key | Success code | Adds or replaces a member |
| Set string member | `jsonc_object_set_string` | Object node, key, UTF-8 text | Success code | Adds or replaces a member |
| Set integer member | `jsonc_object_set_integer` | Object node, key, integer value | Success code | Adds or replaces a member |
| Set number member | `jsonc_object_set_number` | Object node, key, floating-point value | Success code | Adds or replaces a member |
| Remove member | `jsonc_object_remove` | Object node, key | Success code | Deletes a member if present |
| Append array value | `jsonc_array_append_value` | Array node, owned value | Success code | Appends an owned item |
| Remove array value | `jsonc_array_remove` | Array node, index | Success code | Deletes an element if present |
| Create null | `jsonc_value_new_null` | None | Owned `jsonc_value *` | Caller owns the result |
| Create boolean | `jsonc_value_new_bool` | Integer boolean | Owned `jsonc_value *` | Caller owns the result |
| Create string | `jsonc_value_new_string` | UTF-8 text | Owned `jsonc_value *` | Caller owns the result |
| Create integer | `jsonc_value_new_integer` | Integer value | Owned `jsonc_value *` | Caller owns the result |
| Create number | `jsonc_value_new_number` | Floating-point value | Owned `jsonc_value *` | Caller owns the result |
| Create array | `jsonc_value_new_array` | None | Owned `jsonc_value *` | Caller owns the result |
| Create object | `jsonc_value_new_object` | None | Owned `jsonc_value *` | Caller owns the result |

## What applications must handle

- opening and closing files
- reading file contents into memory
- writing serialized text back to disk
- merging documents
- custom business rules and schema checks

## Return patterns

- pointer return values are either owned handles or borrowed pointers, depending on the function
- integer return values use `0` for success and non-zero for failure in setter functions
- lookup helpers return `NULL` when the key or node is missing

## Practical guidance

Prefer the smallest operation that matches your goal. Parse once, inspect or mutate in memory, serialize once, and destroy the tree when finished.