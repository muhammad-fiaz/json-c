/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <stdio.h>

int main(void) {
    jsonc_error error;
    jsonc_value *lines;
    char *text;

    lines = jsonc_parse_jsonl_string("{\"id\":1}\n{\"id\":2}\n", &error);
    if (lines == NULL) {
        fprintf(stderr, "parse error: %s\n", error.message);
        return 1;
    }

    text = jsonc_stringify(lines, JSONC_FORMAT_PRETTY, NULL);
    if (text != NULL) {
        puts(text);
        jsonc_free_string(text, NULL);
    }

    jsonc_value_destroy(lines);
    return 0;
}
