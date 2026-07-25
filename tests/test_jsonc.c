/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <assert.h>
#include <stdio.h>
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

static void test_parse_jsonl(void) {
    jsonc_error error;
    jsonc_value *lines;
    jsonc_value *first;
    jsonc_value *second;
    const char *name;

    lines = jsonc_parse_jsonl_string("{\"ok\":true}\n\n{\"name\":\"json-c\"}\n", &error);
    assert(lines != NULL);
    assert(error.code == JSONC_ERROR_NONE);
    assert(jsonc_value_type(lines) == JSONC_TYPE_ARRAY);
    assert(jsonc_array_size(lines) == 2);

    first = jsonc_array_get(lines, 0);
    assert(first != NULL);
    assert(jsonc_value_type(first) == JSONC_TYPE_OBJECT);
    assert(jsonc_value_get_bool(jsonc_object_get(first, "ok")) == 1);

    second = jsonc_array_get(lines, 1);
    assert(second != NULL);
    assert(jsonc_value_type(second) == JSONC_TYPE_OBJECT);
    name = jsonc_value_get_string(jsonc_object_get(second, "name"));
    assert(name != NULL);
    assert(strcmp(name, "json-c") == 0);

    jsonc_value_destroy(lines);
}

static void test_mutation_helpers(void) {
    jsonc_value *object;
    jsonc_value *array;
    jsonc_value *item;

    object = jsonc_value_new_object();
    assert(object != NULL);

    assert(jsonc_object_set_string(object, "name", "json-c") == 0);
    assert(jsonc_object_set_string(object, "name", "json-c-core") == 0);
    assert(jsonc_object_set_integer(object, "count", 3) == 0);
    assert(jsonc_object_set_number(object, "ratio", 1.5) == 0);
    assert(jsonc_object_set_null(object, "missing") == 0);
    assert(jsonc_object_set_bool(object, "ok", 1) == 0);
    assert(jsonc_object_get(object, "name") != NULL);
    assert(strcmp(jsonc_value_get_string(jsonc_object_get(object, "name")), "json-c-core") == 0);
    assert(jsonc_value_get_integer(jsonc_object_get(object, "count")) == 3);
    assert(jsonc_value_get_number(jsonc_object_get(object, "ratio")) > 1.49);
    assert(jsonc_object_remove(object, "missing") == 0);

    array = jsonc_value_new_array();
    assert(array != NULL);
    assert(jsonc_array_append_value(array, jsonc_value_new_string("one")) == 0);
    assert(jsonc_array_append_value(array, jsonc_value_new_bool(1)) == 0);
    assert(jsonc_array_append_value(array, jsonc_value_new_integer(2)) == 0);
    assert(jsonc_array_size(array) == 3);

    item = jsonc_array_get(array, 0);
    assert(item != NULL);
    assert(strcmp(jsonc_value_get_string(item), "one") == 0);

    assert(jsonc_array_remove(array, 1) == 0);
    assert(jsonc_array_size(array) == 2);

    jsonc_value_destroy(object);
    jsonc_value_destroy(array);
}

static void test_file_helpers(void) {
    const char *json_path = "jsonc_test_input.json";
    const char *jsonl_path = "jsonc_test_input.jsonl";
    const char *output_path = "jsonc_test_output.json";
    FILE *file;
    jsonc_error error;
    jsonc_value *value;
    jsonc_value *lines;

    file = fopen(json_path, "wb");
    assert(file != NULL);
    fputs("{\"enabled\":true,\"name\":\"json-c\"}", file);
    fclose(file);

    file = fopen(jsonl_path, "wb");
    assert(file != NULL);
    fputs("{\"id\":1}\n{\"id\":2}\n", file);
    fclose(file);

    value = jsonc_parse_file(json_path, &error);
    assert(value != NULL);
    assert(error.code == JSONC_ERROR_NONE);
    assert(jsonc_value_get_bool(jsonc_object_get(value, "enabled")) == 1);
    assert(jsonc_write_file(output_path, value, JSONC_FORMAT_PRETTY, &error) == 0);
    jsonc_value_destroy(value);

    value = jsonc_parse_file(output_path, &error);
    assert(value != NULL);
    assert(jsonc_value_get_bool(jsonc_object_get(value, "enabled")) == 1);
    jsonc_value_destroy(value);

    lines = jsonc_parse_jsonl_file(jsonl_path, &error);
    assert(lines != NULL);
    assert(jsonc_array_size(lines) == 2);
    jsonc_value_destroy(lines);

    remove(json_path);
    remove(jsonl_path);
    remove(output_path);
}

int main(void) {
    test_parse_and_stringify();
    test_parse_jsonl();
    test_mutation_helpers();
    test_file_helpers();
    return 0;
}
