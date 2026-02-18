---
title: Getting Started
description: Quick start for parsing, editing, and serializing JSON with json-c.
---

# Getting Started

This guide shows the smallest path from raw JSON text to an editable document and back again.

## Include the header

```c
#include <jsonc/jsonc.h>
```

## Parse a document

```c
jsonc_error error;
jsonc_value *value;

value = jsonc_parse_string("{\"ok\":true}", &error);
if (value == NULL) {
    /* inspect error.code, error.message, error.line, error.column */
}
```

## Inspect or edit it

```c
jsonc_value *item;

item = jsonc_object_get(value, "ok");
if (item != NULL) {
    jsonc_value_set_bool(item, 0);
}
```

## Serialize it

```c
char *text;

text = jsonc_stringify(value, JSONC_FORMAT_COMPACT, NULL);
if (text != NULL) {
    jsonc_free_string(text, NULL);
}
```

## Release resources

```c
jsonc_value_destroy(value);
```

## Next steps

- Read the Installation guide.
- Review the Parser and Types pages.
- Check the Validation guide before parsing untrusted input.
