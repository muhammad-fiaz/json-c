/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <stdio.h>

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
    if (text == NULL) {
        jsonc_value_destroy(value);
        return 1;
    }

    puts(text);
    jsonc_free_string(text, NULL);
    jsonc_value_destroy(value);
    return 0;
}
