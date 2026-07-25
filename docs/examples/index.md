---
title: Examples
description: Copy-ready examples for parsing, editing, and serializing JSON with json-c.
---

# Examples

This section collects small, compile-tested usage examples that mirror the current public API.

## Current example

The repository ships with [examples/basic.c](../../examples/basic.c), a minimal parse-and-print program that exercises the public header end to end.

```c
#include <stdio.h>
#include <jsonc/jsonc.h>

int main(void) {
	jsonc_error error;
	jsonc_value *value;
	char *text;

	value = jsonc_parse_string("{\"name\":\"json-c\"}", &error);
	if (value == NULL) {
		fprintf(stderr, "parse error: %s\n", error.message);
		return 1;
	}

	text = jsonc_stringify(value, JSONC_FORMAT_PRETTY, NULL);
	if (text != NULL) {
		puts(text);
		jsonc_free_string(text, NULL);
	}

	jsonc_value_destroy(value);
	return 0;
}
```

## Example style

Examples should stay small, self-contained, and ready to copy into a C or C++ project with minimal adjustment.
