/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <stdio.h>

int main(void) {
    jsonc_error error;
    jsonc_value *value;

    value = jsonc_parse_file("input.json", &error);
    if (value == NULL) {
        fprintf(stderr, "parse error: %s\n", error.message);
        return 1;
    }

    jsonc_object_set_bool(value, "enabled", 1);
    if (jsonc_write_file("output.json", value, JSONC_FORMAT_PRETTY, &error) != 0) {
        fprintf(stderr, "write error: %s\n", error.message);
        jsonc_value_destroy(value);
        return 1;
    }

    jsonc_value_destroy(value);
    return 0;
}
