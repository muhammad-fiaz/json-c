---
title: API Overview
description: Complete overview of the json-c public API, data model, parameters, and return values.
---

# API Overview

json-c exposes a compact, explicit C API designed around a few core concepts:

- parse JSON from a file path
- parse JSON Lines from a file path
- write JSON values to a file path

## Public surface

The exported header provides these function groups:

- parse JSON text and JSON Lines from memory
- parse JSON text and JSON Lines from file paths
- serialize JSON values to text or files
- inspect and mutate booleans, integers, numbers, strings, arrays, and objects
- create null, boolean, integer, number, array, object, and string values

## Core types

- `jsonc_value`: opaque JSON node handle
- `jsonc_type`: node kind enumeration
- `jsonc_format`: serializer output mode
- `jsonc_error`: parse diagnostics
- `jsonc_allocator`: custom allocation callbacks

## How to read the API pages

Each API page documents:

- purpose
- parameters
- return value
- ownership rules
- common usage examples

## Supported JSON model

json-c currently models the standard JSON value set:

- null
- boolean
- integer
- number
- string
- array
- object

## Current operation set

The public header exports a focused but complete set of basic operations:

- parse JSON and JSON Lines text or files
- inspect and update primitive values
- inspect and mutate arrays and objects
- create value nodes directly
- serialize and write values with explicit ownership rules

The current surface includes file-path helpers, array accessors, array append/remove helpers, object lookup and setter/removal helpers, and scalar value constructors.

## Typical flow

1. Read JSON text from a file, network response, or string buffer in your application, or call `jsonc_parse_file` directly.
2. Parse the text with `jsonc_parse_string` or `jsonc_parse_jsonl_string`.
3. Inspect or mutate the resulting DOM tree.
4. Serialize the value with `jsonc_stringify`.
5. Release the tree with `jsonc_value_destroy` and free any serialized buffer.

JSON Lines input is parsed into a JSON array where each non-empty line becomes one element.

## File handling note

json-c still lets your application manage file handles directly, but it also provides convenience wrappers for file-path based parsing and writing when you want them.

## Related pages

- [Types](./types)
- [Parser](./parser)
- [Serializer](./export)
- [Errors](./errors)
- [Constants](./constants)