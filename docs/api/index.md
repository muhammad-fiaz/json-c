---
title: API Overview
description: Complete overview of the json-c public API, data model, parameters, and return values.
---

# API Overview

json-c exposes a compact, explicit C API. The library is designed around a few core concepts:

- parse UTF-8 JSON text into a DOM tree
- inspect and mutate values in memory
- serialize values back to JSON text
- destroy values and release owned buffers
- report detailed errors when parsing or validation fails

## Public surface

The exported header currently provides these function groups:

- parsing and serialization
- value lifecycle management
- value inspection helpers
- object lookup and mutation helpers
- value factory functions for null, boolean, and string nodes

## Core types

- `jsonc_value`: opaque JSON node handle
- `jsonc_type`: node kind enumeration
- `jsonc_format`: serializer output mode
- `jsonc_error`: parse and validation diagnostics
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

The public header currently exports a small but complete set of basic operations:

- parse text into a value tree
- inspect type, boolean, and string nodes
- look up object members
- update boolean values in place
- create null, boolean, and string values
- serialize values and free serialized output

If you need array indexing, object removal, file opening, or file closing, handle that in your application code or add the missing API surface before documenting it.

## Typical flow

1. Read JSON text from a file, network response, or string buffer in your application.
2. Parse the text with `jsonc_parse_string`.
3. Inspect or mutate the resulting DOM tree.
4. Serialize the value with `jsonc_stringify`.
5. Release the tree with `jsonc_value_destroy` and free any serialized buffer.

## File handling note

json-c does not open or close files for you. File loading and flushing are handled by the application using normal C or C++ file APIs, then the resulting text is passed to json-c for parsing or serialization.

## Related pages

- [Types](./types)
- [Parser](./parser)
- [Serializer](./export)
- [Errors](./errors)
- [Constants](./constants)