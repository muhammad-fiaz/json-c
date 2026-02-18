/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <assert.h>
#include <string.h>

static void test_parse_and_stringify(void) {
    jsonc_error error;
    jsonc_value *value;
    jsonc_value *member;
    char *text;

    value = jsonc_parse_string("{\"ok\":true,\"name\":\"json-c\"}", &error);
    assert(value != NULL);
    assert(error.code == JSONC_ERROR_NONE);

    member = jsonc_object_get(value, "ok");
    assert(member != NULL);
    assert(jsonc_value_get_bool(member) == 1);
    assert(jsonc_value_set_bool(member, 0) == 0);
    assert(jsonc_value_get_bool(member) == 0);

    text = jsonc_stringify(value, JSONC_FORMAT_COMPACT, NULL);
    assert(text != NULL);
    assert(strstr(text, "\"ok\":false") != NULL);
    jsonc_free_string(text, NULL);

    jsonc_value_destroy(value);
}

int main(void) {
    test_parse_and_stringify();
    return 0;
}
