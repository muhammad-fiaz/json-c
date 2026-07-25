/*
 * SPDX-License-Identifier: MIT
 *
 * json-c public API.
 */
#ifndef JSONC_JSONC_H
#define JSONC_JSONC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum jsonc_type {
    JSONC_TYPE_NULL = 0,
    JSONC_TYPE_BOOLEAN = 1,
    JSONC_TYPE_INTEGER = 2,
    JSONC_TYPE_NUMBER = 3,
    JSONC_TYPE_STRING = 4,
    JSONC_TYPE_ARRAY = 5,
    JSONC_TYPE_OBJECT = 6
} jsonc_type;

typedef enum jsonc_format {
    JSONC_FORMAT_COMPACT = 0,
    JSONC_FORMAT_PRETTY = 1
} jsonc_format;

typedef enum jsonc_error_code {
    JSONC_ERROR_NONE = 0,
    JSONC_ERROR_INVALID_SYNTAX = 1,
    JSONC_ERROR_UNEXPECTED_EOF = 2,
    JSONC_ERROR_UNSUPPORTED = 3,
    JSONC_ERROR_DEPTH_LIMIT = 4,
    JSONC_ERROR_NO_MEMORY = 5,
    JSONC_ERROR_TYPE_MISMATCH = 6,
    JSONC_ERROR_NOT_FOUND = 7
} jsonc_error_code;

typedef void *(*jsonc_malloc_fn)(size_t size, void *user_data);
typedef void (*jsonc_free_fn)(void *ptr, void *user_data);

typedef struct jsonc_allocator {
    jsonc_malloc_fn malloc_fn;
    jsonc_free_fn free_fn;
    void *user_data;
} jsonc_allocator;

typedef struct jsonc_error {
    int code;
    char message[256];
    size_t line;
    size_t column;
    size_t byte_offset;
    char path[128];
} jsonc_error;

typedef struct jsonc_value jsonc_value;

typedef struct jsonc_pair {
    char *key;
    jsonc_value *value;
} jsonc_pair;

/**
 * Parse JSON text into a new DOM value.
 *
 * @param text UTF-8 encoded JSON document.
 * @param error Optional error output.
 * @return Newly allocated value on success, or NULL on failure.
 */
jsonc_value *jsonc_parse_string(const char *text, jsonc_error *error);

/**
 * Serialize a JSON value into a newly allocated UTF-8 string.
 *
 * @param value JSON value to serialize.
 * @param format Compact or pretty output.
 * @param allocator Optional allocator to use for the returned buffer.
 * @return Newly allocated NUL-terminated string, or NULL on failure.
 */
char *jsonc_stringify(const jsonc_value *value, jsonc_format format, const jsonc_allocator *allocator);

/**
 * Parse JSON Lines text into an array of JSON values.
 *
 * @param text UTF-8 encoded JSON Lines document.
 * @param error Optional error output.
 * @return Newly allocated array value on success, or NULL on failure.
 */
jsonc_value *jsonc_parse_jsonl_string(const char *text, jsonc_error *error);

/**
 * Parse a JSON document from a file path.
 *
 * @param path UTF-8 file path.
 * @param error Optional error output.
 * @return Newly allocated value on success, or NULL on failure.
 */
jsonc_value *jsonc_parse_file(const char *path, jsonc_error *error);

/**
 * Parse a JSON Lines document from a file path.
 *
 * @param path UTF-8 file path.
 * @param error Optional error output.
 * @return Newly allocated array value on success, or NULL on failure.
 */
jsonc_value *jsonc_parse_jsonl_file(const char *path, jsonc_error *error);

/**
 * Write a JSON value to a file path.
 *
 * @param path UTF-8 file path.
 * @param value JSON value to write.
 * @param format Compact or pretty output.
 * @param error Optional error output.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_write_file(const char *path, const jsonc_value *value, jsonc_format format, jsonc_error *error);

/**
 * Return the number of elements in an array value.
 *
 * @param value Array node.
 * @return Element count, or 0 for NULL or non-array input.
 */
size_t jsonc_array_size(const jsonc_value *value);

/**
 * Return an array element by index.
 *
 * @param value Array node.
 * @param index Zero-based element index.
 * @return Borrowed element pointer, or NULL when out of range.
 */
jsonc_value *jsonc_array_get(const jsonc_value *value, size_t index);

/**
 * Append a JSON value to an array.
 *
 * @param value Array node.
 * @param item Owned value to append.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_array_append_value(jsonc_value *value, jsonc_value *item);

/**
 * Remove an array element by index.
 *
 * @param value Array node.
 * @param index Zero-based element index.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_array_remove(jsonc_value *value, size_t index);

/**
 * Free a string returned by jsonc_stringify.
 *
 * @param text String to release.
 * @param allocator Optional allocator matching the one used for allocation.
 */
void jsonc_free_string(char *text, const jsonc_allocator *allocator);

/**
 * Destroy a JSON value and all of its children.
 *
 * @param value Value to destroy.
 */
void jsonc_value_destroy(jsonc_value *value);

/**
 * Return the type of a JSON value.
 *
 * @param value Value to inspect.
 * @return Value type, or JSONC_TYPE_NULL for NULL input.
 */
jsonc_type jsonc_value_type(const jsonc_value *value);

/**
 * Return a boolean value.
 *
 * @param value Boolean node.
 * @return Non-zero for true, zero for false.
 */
int jsonc_value_get_bool(const jsonc_value *value);

/**
 * Update a boolean value in place.
 *
 * @param value Boolean node to update.
 * @param boolean_value New boolean value.
 * @return 0 on success, non-zero on type mismatch.
 */
int jsonc_value_set_bool(jsonc_value *value, int boolean_value);

/**
 * Return an integer value.
 *
 * @param value Integer node.
 * @return Integer payload, or 0 for NULL or non-integer input.
 */
long jsonc_value_get_integer(const jsonc_value *value);

/**
 * Return a number value.
 *
 * @param value Number node.
 * @return Floating-point payload, or 0.0 for NULL or non-number input.
 */
double jsonc_value_get_number(const jsonc_value *value);

/**
 * Return a string value.
 *
 * @param value String node.
 * @return Owned string pointer or NULL.
 */
const char *jsonc_value_get_string(const jsonc_value *value);

/**
 * Look up a member in an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @return Matching member or NULL.
 */
jsonc_value *jsonc_object_get(const jsonc_value *value, const char *key);

/**
 * Add or replace a boolean member on an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @param boolean_value Boolean payload.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_set_bool(jsonc_value *value, const char *key, int boolean_value);

/**
 * Add or replace a null member on an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_set_null(jsonc_value *value, const char *key);

/**
 * Add or replace a string member on an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @param text UTF-8 string payload.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_set_string(jsonc_value *value, const char *key, const char *text);

/**
 * Add or replace an integer member on an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @param integer_value Integer payload.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_set_integer(jsonc_value *value, const char *key, long integer_value);

/**
 * Add or replace a number member on an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @param number_value Floating-point payload.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_set_number(jsonc_value *value, const char *key, double number_value);

/**
 * Remove a member from an object.
 *
 * @param value Object node.
 * @param key Member name.
 * @return 0 on success, non-zero on failure.
 */
int jsonc_object_remove(jsonc_value *value, const char *key);

/**
 * Create a null value.
 *
 * @return Newly allocated null value or NULL.
 */
jsonc_value *jsonc_value_new_null(void);

/**
 * Create a boolean value.
 *
 * @param boolean_value Boolean payload.
 * @return Newly allocated boolean value or NULL.
 */
jsonc_value *jsonc_value_new_bool(int boolean_value);

/**
 * Create an integer value.
 *
 * @param integer_value Integer payload.
 * @return Newly allocated integer value or NULL.
 */
jsonc_value *jsonc_value_new_integer(long integer_value);

/**
 * Create a number value.
 *
 * @param number_value Floating-point payload.
 * @return Newly allocated number value or NULL.
 */
jsonc_value *jsonc_value_new_number(double number_value);

/**
 * Create an array value.
 *
 * @return Newly allocated array value or NULL.
 */
jsonc_value *jsonc_value_new_array(void);

/**
 * Create an object value.
 *
 * @return Newly allocated object value or NULL.
 */
jsonc_value *jsonc_value_new_object(void);

/**
 * Create a string value.
 *
 * @param text UTF-8 string.
 * @return Newly allocated string value or NULL.
 */
jsonc_value *jsonc_value_new_string(const char *text);

#ifdef __cplusplus
}
#endif

#endif
