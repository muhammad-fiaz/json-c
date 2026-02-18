/*
 * SPDX-License-Identifier: MIT
 */
#include "jsonc/jsonc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct jsonc_value {
    jsonc_type type;
    union {
        int boolean_value;
        long integer_value;
        double number_value;
        char *string_value;
        struct {
            jsonc_value **items;
            size_t size;
            size_t capacity;
        } array;
        struct {
            jsonc_pair *items;
            size_t size;
            size_t capacity;
        } object;
    } as;
};

typedef struct jsonc_parser {
    const char *text;
    size_t length;
    size_t index;
    size_t line;
    size_t column;
    char path[128];
    jsonc_error *error;
} jsonc_parser;

static void *jsonc_default_malloc(size_t size, void *user_data) {
    (void)user_data;
    return malloc(size);
}

static void jsonc_default_free(void *ptr, void *user_data) {
    (void)user_data;
    free(ptr);
}

static const jsonc_allocator jsonc_default_allocator = {
    jsonc_default_malloc,
    jsonc_default_free,
    NULL
};

static void jsonc_set_error(jsonc_parser *parser, int code, const char *message) {
    if (parser == NULL || parser->error == NULL) {
        return;
    }

    parser->error->code = code;
    strncpy(parser->error->message, message, sizeof(parser->error->message) - 1);
    parser->error->message[sizeof(parser->error->message) - 1] = '\0';
    parser->error->line = parser->line;
    parser->error->column = parser->column;
    parser->error->byte_offset = parser->index;
    strncpy(parser->error->path, parser->path, sizeof(parser->error->path) - 1);
    parser->error->path[sizeof(parser->error->path) - 1] = '\0';
}

static jsonc_value *jsonc_alloc_value(void) {
    jsonc_value *value;
    value = (jsonc_value *)malloc(sizeof(jsonc_value));
    if (value == NULL) {
        return NULL;
    }
    memset(value, 0, sizeof(jsonc_value));
    return value;
}

static jsonc_value *jsonc_new_value(jsonc_type type) {
    jsonc_value *value;
    value = jsonc_alloc_value();
    if (value == NULL) {
        return NULL;
    }
    value->type = type;
    return value;
}

jsonc_value *jsonc_value_new_null(void) {
    return jsonc_new_value(JSONC_TYPE_NULL);
}

jsonc_value *jsonc_value_new_bool(int boolean_value) {
    jsonc_value *value;
    value = jsonc_new_value(JSONC_TYPE_BOOLEAN);
    if (value == NULL) {
        return NULL;
    }
    value->as.boolean_value = boolean_value ? 1 : 0;
    return value;
}

jsonc_value *jsonc_value_new_string(const char *text) {
    jsonc_value *value;
    size_t length;
    char *copy;

    if (text == NULL) {
        return NULL;
    }

    length = strlen(text);
    copy = (char *)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, text, length + 1);

    value = jsonc_new_value(JSONC_TYPE_STRING);
    if (value == NULL) {
        free(copy);
        return NULL;
    }

    value->as.string_value = copy;
    return value;
}

static jsonc_value *jsonc_value_new_integer(long integer_value) {
    jsonc_value *value;
    value = jsonc_new_value(JSONC_TYPE_INTEGER);
    if (value == NULL) {
        return NULL;
    }
    value->as.integer_value = integer_value;
    return value;
}

static jsonc_value *jsonc_value_new_number(double number_value) {
    jsonc_value *value;
    value = jsonc_new_value(JSONC_TYPE_NUMBER);
    if (value == NULL) {
        return NULL;
    }
    value->as.number_value = number_value;
    return value;
}

static jsonc_value *jsonc_value_new_array(void) {
    return jsonc_new_value(JSONC_TYPE_ARRAY);
}

static jsonc_value *jsonc_value_new_object(void) {
    return jsonc_new_value(JSONC_TYPE_OBJECT);
}

jsonc_type jsonc_value_type(const jsonc_value *value) {
    if (value == NULL) {
        return JSONC_TYPE_NULL;
    }
    return value->type;
}

int jsonc_value_get_bool(const jsonc_value *value) {
    if (value == NULL || value->type != JSONC_TYPE_BOOLEAN) {
        return 0;
    }
    return value->as.boolean_value;
}

int jsonc_value_set_bool(jsonc_value *value, int boolean_value) {
    if (value == NULL || value->type != JSONC_TYPE_BOOLEAN) {
        return -1;
    }
    value->as.boolean_value = boolean_value ? 1 : 0;
    return 0;
}

const char *jsonc_value_get_string(const jsonc_value *value) {
    if (value == NULL || value->type != JSONC_TYPE_STRING) {
        return NULL;
    }
    return value->as.string_value;
}

static void jsonc_value_destroy_internal(jsonc_value *value) {
    size_t i;

    if (value == NULL) {
        return;
    }

    if (value->type == JSONC_TYPE_STRING) {
        free(value->as.string_value);
    } else if (value->type == JSONC_TYPE_ARRAY) {
        for (i = 0; i < value->as.array.size; ++i) {
            jsonc_value_destroy_internal(value->as.array.items[i]);
        }
        free(value->as.array.items);
    } else if (value->type == JSONC_TYPE_OBJECT) {
        for (i = 0; i < value->as.object.size; ++i) {
            free(value->as.object.items[i].key);
            jsonc_value_destroy_internal(value->as.object.items[i].value);
        }
        free(value->as.object.items);
    }

    free(value);
}

void jsonc_value_destroy(jsonc_value *value) {
    jsonc_value_destroy_internal(value);
}

void jsonc_free_string(char *text, const jsonc_allocator *allocator) {
    if (text == NULL) {
        return;
    }
    (void)allocator;
    free(text);
}

static void jsonc_parser_init(jsonc_parser *parser, const char *text, jsonc_error *error) {
    parser->text = text;
    parser->length = strlen(text);
    parser->index = 0;
    parser->line = 1;
    parser->column = 1;
    parser->error = error;
    parser->path[0] = '$';
    parser->path[1] = '\0';
    if (error != NULL) {
        error->code = JSONC_ERROR_NONE;
        error->message[0] = '\0';
        error->line = 1;
        error->column = 1;
        error->byte_offset = 0;
        error->path[0] = '$';
        error->path[1] = '\0';
    }
}

static int jsonc_parser_peek(jsonc_parser *parser) {
    if (parser->index >= parser->length) {
        return EOF;
    }
    return (unsigned char)parser->text[parser->index];
}

static int jsonc_parser_next(jsonc_parser *parser) {
    int ch;
    ch = jsonc_parser_peek(parser);
    if (ch == EOF) {
        return EOF;
    }
    parser->index += 1;
    if (ch == '\n') {
        parser->line += 1;
        parser->column = 1;
    } else {
        parser->column += 1;
    }
    return ch;
}

static void jsonc_parser_skip_ws(jsonc_parser *parser) {
    int ch;
    while ((ch = jsonc_parser_peek(parser)) != EOF && isspace(ch)) {
        jsonc_parser_next(parser);
    }
}

static int jsonc_hex_value(int ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    if (ch >= 'a' && ch <= 'f') {
        return 10 + (ch - 'a');
    }
    if (ch >= 'A' && ch <= 'F') {
        return 10 + (ch - 'A');
    }
    return -1;
}

static int jsonc_utf8_append(char **buffer, size_t *length, size_t *capacity, unsigned long codepoint) {
    unsigned char bytes[4];
    size_t count;
    size_t i;
    char *grown;

    if (codepoint <= 0x7F) {
        bytes[0] = (unsigned char)codepoint;
        count = 1;
    } else if (codepoint <= 0x7FF) {
        bytes[0] = (unsigned char)(0xC0 | (codepoint >> 6));
        bytes[1] = (unsigned char)(0x80 | (codepoint & 0x3F));
        count = 2;
    } else if (codepoint <= 0xFFFF) {
        bytes[0] = (unsigned char)(0xE0 | (codepoint >> 12));
        bytes[1] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        bytes[2] = (unsigned char)(0x80 | (codepoint & 0x3F));
        count = 3;
    } else {
        bytes[0] = (unsigned char)(0xF0 | (codepoint >> 18));
        bytes[1] = (unsigned char)(0x80 | ((codepoint >> 12) & 0x3F));
        bytes[2] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        bytes[3] = (unsigned char)(0x80 | (codepoint & 0x3F));
        count = 4;
    }

    if (*length + count + 1 > *capacity) {
        size_t new_capacity;
        new_capacity = *capacity * 2;
        if (new_capacity < *length + count + 1) {
            new_capacity = *length + count + 1;
        }
        grown = (char *)realloc(*buffer, new_capacity);
        if (grown == NULL) {
            return -1;
        }
        *buffer = grown;
        *capacity = new_capacity;
    }

    for (i = 0; i < count; ++i) {
        (*buffer)[*length + i] = (char)bytes[i];
    }
    *length += count;
    (*buffer)[*length] = '\0';
    return 0;
}

static int jsonc_parse_unicode_escape(jsonc_parser *parser, unsigned long *codepoint) {
    int i;
    int ch;
    unsigned long value;

    value = 0;
    for (i = 0; i < 4; ++i) {
        ch = jsonc_parser_next(parser);
        if (ch == EOF) {
            jsonc_set_error(parser, JSONC_ERROR_UNEXPECTED_EOF, "Unexpected end of input in unicode escape");
            return -1;
        }
        ch = jsonc_hex_value(ch);
        if (ch < 0) {
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid unicode escape");
            return -1;
        }
        value = (value << 4) | (unsigned long)ch;
    }
    *codepoint = value;
    return 0;
}

static int jsonc_string_append_char(char **buffer, size_t *length, size_t *capacity, char ch) {
    char *grown;
    if (*length + 2 > *capacity) {
        size_t new_capacity;
        new_capacity = *capacity * 2;
        if (new_capacity < *length + 2) {
            new_capacity = *length + 2;
        }
        grown = (char *)realloc(*buffer, new_capacity);
        if (grown == NULL) {
            return -1;
        }
        *buffer = grown;
        *capacity = new_capacity;
    }
    (*buffer)[*length] = ch;
    *length += 1;
    (*buffer)[*length] = '\0';
    return 0;
}

static jsonc_value *jsonc_parse_value(jsonc_parser *parser);

static jsonc_value *jsonc_parse_string_value(jsonc_parser *parser) {
    char *buffer;
    size_t length;
    size_t capacity;
    int ch;

    buffer = (char *)malloc(32);
    if (buffer == NULL) {
        jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
        return NULL;
    }
    length = 0;
    capacity = 32;
    buffer[0] = '\0';

    if (jsonc_parser_next(parser) != '"') {
        free(buffer);
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected string opening quote");
        return NULL;
    }

    while ((ch = jsonc_parser_next(parser)) != EOF) {
        if (ch == '"') {
            jsonc_value *value;
            value = jsonc_new_value(JSONC_TYPE_STRING);
            if (value == NULL) {
                free(buffer);
                jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                return NULL;
            }
            value->as.string_value = buffer;
            return value;
        }
        if (ch == '\\') {
            int esc;
            esc = jsonc_parser_next(parser);
            if (esc == EOF) {
                free(buffer);
                jsonc_set_error(parser, JSONC_ERROR_UNEXPECTED_EOF, "Unexpected end of input in string escape");
                return NULL;
            }
            if (esc == '"' || esc == '\\' || esc == '/') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, (char)esc) != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 'b') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, '\b') != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 'f') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, '\f') != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 'n') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, '\n') != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 'r') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, '\r') != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 't') {
                if (jsonc_string_append_char(&buffer, &length, &capacity, '\t') != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else if (esc == 'u') {
                unsigned long codepoint;
                if (jsonc_parse_unicode_escape(parser, &codepoint) != 0) {
                    free(buffer);
                    return NULL;
                }
                if (jsonc_utf8_append(&buffer, &length, &capacity, codepoint) != 0) {
                    free(buffer);
                    jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                    return NULL;
                }
            } else {
                free(buffer);
                jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid string escape");
                return NULL;
            }
        } else {
            if ((unsigned char)ch < 0x20) {
                free(buffer);
                jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Unescaped control character in string");
                return NULL;
            }
            if (jsonc_string_append_char(&buffer, &length, &capacity, (char)ch) != 0) {
                free(buffer);
                jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                return NULL;
            }
        }
    }

    free(buffer);
    jsonc_set_error(parser, JSONC_ERROR_UNEXPECTED_EOF, "Unexpected end of input in string");
    return NULL;
}

static jsonc_value *jsonc_parse_number_value(jsonc_parser *parser) {
    size_t start;
    size_t end;
    char buffer[128];
    char *tail;
    double number;
    long integer_value;
    int is_integer;

    start = parser->index;
    if (jsonc_parser_peek(parser) == '-') {
        jsonc_parser_next(parser);
    }
    if (!isdigit(jsonc_parser_peek(parser))) {
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid number");
        return NULL;
    }
    if (jsonc_parser_peek(parser) == '0') {
        jsonc_parser_next(parser);
    } else {
        while (isdigit(jsonc_parser_peek(parser))) {
            jsonc_parser_next(parser);
        }
    }
    is_integer = 1;
    if (jsonc_parser_peek(parser) == '.') {
        is_integer = 0;
        jsonc_parser_next(parser);
        if (!isdigit(jsonc_parser_peek(parser))) {
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid number fraction");
            return NULL;
        }
        while (isdigit(jsonc_parser_peek(parser))) {
            jsonc_parser_next(parser);
        }
    }
    if (jsonc_parser_peek(parser) == 'e' || jsonc_parser_peek(parser) == 'E') {
        is_integer = 0;
        jsonc_parser_next(parser);
        if (jsonc_parser_peek(parser) == '+' || jsonc_parser_peek(parser) == '-') {
            jsonc_parser_next(parser);
        }
        if (!isdigit(jsonc_parser_peek(parser))) {
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid number exponent");
            return NULL;
        }
        while (isdigit(jsonc_parser_peek(parser))) {
            jsonc_parser_next(parser);
        }
    }

    end = parser->index;
    if (end - start >= sizeof(buffer)) {
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Number literal too long");
        return NULL;
    }
    memcpy(buffer, parser->text + start, end - start);
    buffer[end - start] = '\0';

    if (is_integer) {
        integer_value = strtol(buffer, &tail, 10);
        if (tail == buffer || *tail != '\0') {
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid integer");
            return NULL;
        }
        return jsonc_value_new_integer(integer_value);
    }

    number = strtod(buffer, &tail);
    if (tail == buffer || *tail != '\0') {
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid number");
        return NULL;
    }
    return jsonc_value_new_number(number);
}

static int jsonc_path_push(jsonc_parser *parser, const char *segment) {
    size_t current;
    size_t needed;
    current = strlen(parser->path);
    needed = current + strlen(segment) + 1;
    if (needed >= sizeof(parser->path)) {
        return -1;
    }
    strcat(parser->path, segment);
    return 0;
}

static void jsonc_path_pop(jsonc_parser *parser, size_t saved_length) {
    parser->path[saved_length] = '\0';
}

static jsonc_value *jsonc_parse_array_value(jsonc_parser *parser) {
    jsonc_value *value;
    size_t index;
    int ch;
    size_t path_length;
    char segment[32];

    value = jsonc_value_new_array();
    if (value == NULL) {
        jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
        return NULL;
    }

    if (jsonc_parser_next(parser) != '[') {
        jsonc_value_destroy(value);
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected array opening bracket");
        return NULL;
    }
    jsonc_parser_skip_ws(parser);
    if (jsonc_parser_peek(parser) == ']') {
        jsonc_parser_next(parser);
        return value;
    }

    index = 0;
    while (1) {
        jsonc_value *item;
        jsonc_parser_skip_ws(parser);
        path_length = strlen(parser->path);
        sprintf(segment, "[%lu]", (unsigned long)index);
        if (jsonc_path_push(parser, segment) != 0) {
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "JSON path too deep");
            return NULL;
        }
        item = jsonc_parse_value(parser);
        jsonc_path_pop(parser, path_length);
        if (item == NULL) {
            jsonc_value_destroy(value);
            return NULL;
        }
        if (value->as.array.size + 1 > value->as.array.capacity) {
            size_t new_capacity;
            jsonc_value **grown;
            new_capacity = value->as.array.capacity == 0 ? 4 : value->as.array.capacity * 2;
            grown = (jsonc_value **)realloc(value->as.array.items, new_capacity * sizeof(jsonc_value *));
            if (grown == NULL) {
                jsonc_value_destroy(item);
                jsonc_value_destroy(value);
                jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                return NULL;
            }
            value->as.array.items = grown;
            value->as.array.capacity = new_capacity;
        }
        value->as.array.items[value->as.array.size] = item;
        value->as.array.size += 1;
        jsonc_parser_skip_ws(parser);
        ch = jsonc_parser_next(parser);
        if (ch == ']') {
            return value;
        }
        if (ch != ',') {
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected comma or closing bracket in array");
            return NULL;
        }
        index += 1;
    }
}

static jsonc_value *jsonc_parse_object_value(jsonc_parser *parser) {
    jsonc_value *value;
    int ch;
    size_t path_length;

    value = jsonc_value_new_object();
    if (value == NULL) {
        jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
        return NULL;
    }

    if (jsonc_parser_next(parser) != '{') {
        jsonc_value_destroy(value);
        jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected object opening brace");
        return NULL;
    }
    jsonc_parser_skip_ws(parser);
    if (jsonc_parser_peek(parser) == '}') {
        jsonc_parser_next(parser);
        return value;
    }

    while (1) {
        jsonc_value *key_value;
        jsonc_value *member_value;
        const char *key;
        size_t key_length;
        char segment[128];
        size_t i;

        jsonc_parser_skip_ws(parser);
        key_value = jsonc_parse_string_value(parser);
        if (key_value == NULL) {
            jsonc_value_destroy(value);
            return NULL;
        }
        key = key_value->as.string_value;
        key_length = strlen(key);
        jsonc_parser_skip_ws(parser);
        ch = jsonc_parser_next(parser);
        if (ch != ':') {
            jsonc_value_destroy(key_value);
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected colon after object key");
            return NULL;
        }
        path_length = strlen(parser->path);
        if (key_length + 2 >= sizeof(segment)) {
            jsonc_value_destroy(key_value);
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Object key too long for path");
            return NULL;
        }
        segment[0] = '.';
        for (i = 0; i < key_length; ++i) {
            segment[i + 1] = key[i];
        }
        segment[key_length + 1] = '\0';
        if (jsonc_path_push(parser, segment) != 0) {
            jsonc_value_destroy(key_value);
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "JSON path too deep");
            return NULL;
        }
        member_value = jsonc_parse_value(parser);
        jsonc_path_pop(parser, path_length);
        if (member_value == NULL) {
            jsonc_value_destroy(key_value);
            jsonc_value_destroy(value);
            return NULL;
        }
        if (value->as.object.size + 1 > value->as.object.capacity) {
            size_t new_capacity;
            jsonc_pair *grown;
            new_capacity = value->as.object.capacity == 0 ? 4 : value->as.object.capacity * 2;
            grown = (jsonc_pair *)realloc(value->as.object.items, new_capacity * sizeof(jsonc_pair));
            if (grown == NULL) {
                jsonc_value_destroy(key_value);
                jsonc_value_destroy(member_value);
                jsonc_value_destroy(value);
                jsonc_set_error(parser, JSONC_ERROR_NO_MEMORY, "Out of memory");
                return NULL;
            }
            value->as.object.items = grown;
            value->as.object.capacity = new_capacity;
        }
        value->as.object.items[value->as.object.size].key = key_value->as.string_value;
        value->as.object.items[value->as.object.size].value = member_value;
        value->as.object.size += 1;
        key_value->as.string_value = NULL;
        jsonc_value_destroy(key_value);
        jsonc_parser_skip_ws(parser);
        ch = jsonc_parser_next(parser);
        if (ch == '}') {
            return value;
        }
        if (ch != ',') {
            jsonc_value_destroy(value);
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Expected comma or closing brace in object");
            return NULL;
        }
    }
}

static jsonc_value *jsonc_parse_literal(jsonc_parser *parser, const char *literal, int literal_kind) {
    size_t i;
    size_t length;
    jsonc_value *value;

    length = strlen(literal);
    for (i = 0; i < length; ++i) {
        if (jsonc_parser_next(parser) != literal[i]) {
            jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Invalid literal");
            return NULL;
        }
    }
    if (literal_kind == 0) {
        return jsonc_value_new_null();
    }
    if (literal_kind == 1) {
        return jsonc_value_new_bool(1);
    }
    if (literal_kind == 2) {
        return jsonc_value_new_bool(0);
    }
    value = NULL;
    return value;
}

static jsonc_value *jsonc_parse_value(jsonc_parser *parser) {
    int ch;
    jsonc_parser_skip_ws(parser);
    ch = jsonc_parser_peek(parser);
    if (ch == EOF) {
        jsonc_set_error(parser, JSONC_ERROR_UNEXPECTED_EOF, "Unexpected end of input");
        return NULL;
    }
    if (ch == '"') {
        return jsonc_parse_string_value(parser);
    }
    if (ch == '{') {
        return jsonc_parse_object_value(parser);
    }
    if (ch == '[') {
        return jsonc_parse_array_value(parser);
    }
    if (ch == 't') {
        return jsonc_parse_literal(parser, "true", 1);
    }
    if (ch == 'f') {
        return jsonc_parse_literal(parser, "false", 2);
    }
    if (ch == 'n') {
        return jsonc_parse_literal(parser, "null", 0);
    }
    if (ch == '-' || isdigit(ch)) {
        return jsonc_parse_number_value(parser);
    }
    jsonc_set_error(parser, JSONC_ERROR_INVALID_SYNTAX, "Unexpected token");
    return NULL;
}

jsonc_value *jsonc_parse_string(const char *text, jsonc_error *error) {
    jsonc_parser parser;
    jsonc_value *value;

    if (text == NULL) {
        if (error != NULL) {
            error->code = JSONC_ERROR_INVALID_SYNTAX;
            strncpy(error->message, "Input text is NULL", sizeof(error->message) - 1);
            error->message[sizeof(error->message) - 1] = '\0';
            error->line = 1;
            error->column = 1;
            error->byte_offset = 0;
            error->path[0] = '$';
            error->path[1] = '\0';
        }
        return NULL;
    }

    jsonc_parser_init(&parser, text, error);
    value = jsonc_parse_value(&parser);
    if (value == NULL) {
        return NULL;
    }
    jsonc_parser_skip_ws(&parser);
    if (jsonc_parser_peek(&parser) != EOF) {
        jsonc_value_destroy(value);
        jsonc_set_error(&parser, JSONC_ERROR_INVALID_SYNTAX, "Trailing data after JSON document");
        return NULL;
    }
    return value;
}

jsonc_value *jsonc_object_get(const jsonc_value *value, const char *key) {
    size_t i;
    if (value == NULL || value->type != JSONC_TYPE_OBJECT || key == NULL) {
        return NULL;
    }
    for (i = 0; i < value->as.object.size; ++i) {
        if (strcmp(value->as.object.items[i].key, key) == 0) {
            return value->as.object.items[i].value;
        }
    }
    return NULL;
}

int jsonc_object_set_bool(jsonc_value *value, const char *key, int boolean_value) {
    jsonc_value *existing;
    jsonc_value *new_value;
    size_t key_length;
    char *key_copy;
    jsonc_pair *grown;
    size_t new_capacity;

    if (value == NULL || value->type != JSONC_TYPE_OBJECT || key == NULL) {
        return -1;
    }

    existing = jsonc_object_get(value, key);
    if (existing != NULL) {
        if (existing->type != JSONC_TYPE_BOOLEAN) {
            return -1;
        }
        existing->as.boolean_value = boolean_value ? 1 : 0;
        return 0;
    }

    key_length = strlen(key);
    key_copy = (char *)malloc(key_length + 1);
    if (key_copy == NULL) {
        return -1;
    }
    memcpy(key_copy, key, key_length + 1);

    new_value = jsonc_value_new_bool(boolean_value);
    if (new_value == NULL) {
        free(key_copy);
        return -1;
    }

    if (value->as.object.size + 1 > value->as.object.capacity) {
        new_capacity = value->as.object.capacity == 0 ? 4 : value->as.object.capacity * 2;
        grown = (jsonc_pair *)realloc(value->as.object.items, new_capacity * sizeof(jsonc_pair));
        if (grown == NULL) {
            free(key_copy);
            jsonc_value_destroy(new_value);
            return -1;
        }
        value->as.object.items = grown;
        value->as.object.capacity = new_capacity;
    }

    value->as.object.items[value->as.object.size].key = key_copy;
    value->as.object.items[value->as.object.size].value = new_value;
    value->as.object.size += 1;
    return 0;
}

static char *jsonc_string_buffer_reserve(char *buffer, size_t *capacity, size_t needed) {
    char *grown;
    size_t new_capacity;

    if (needed <= *capacity) {
        return buffer;
    }
    new_capacity = *capacity * 2;
    if (new_capacity < needed) {
        new_capacity = needed;
    }
    grown = (char *)realloc(buffer, new_capacity);
    if (grown == NULL) {
        return NULL;
    }
    *capacity = new_capacity;
    return grown;
}

static int jsonc_stringify_escape(char **buffer, size_t *length, size_t *capacity, const char *text) {
    size_t i;
    int ch;
    char *grown;

    for (i = 0; text[i] != '\0'; ++i) {
        ch = (unsigned char)text[i];
        if (ch == '"' || ch == '\\') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = (char)ch;
        } else if (ch == '\b') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = 'b';
        } else if (ch == '\f') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = 'f';
        } else if (ch == '\n') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = 'n';
        } else if (ch == '\r') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = 'r';
        } else if (ch == '\t') {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = '\\';
            (*buffer)[(*length)++] = 't';
        } else if ((unsigned char)ch < 0x20) {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 7);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            sprintf(*buffer + *length, "\\u%04x", ch);
            *length += 6;
        } else {
            grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 2);
            if (grown == NULL) {
                return -1;
            }
            *buffer = grown;
            (*buffer)[(*length)++] = (char)ch;
        }
    }
    (*buffer)[*length] = '\0';
    return 0;
}

static int jsonc_stringify_internal(const jsonc_value *value, jsonc_format format, char **buffer, size_t *length, size_t *capacity, int depth);

static int jsonc_indent(char **buffer, size_t *length, size_t *capacity, int depth) {
    int i;
    char *grown;
    for (i = 0; i < depth; ++i) {
        grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + 3);
        if (grown == NULL) {
            return -1;
        }
        *buffer = grown;
        (*buffer)[(*length)++] = ' ';
        (*buffer)[(*length)++] = ' ';
    }
    (*buffer)[*length] = '\0';
    return 0;
}

static int jsonc_append_literal(char **buffer, size_t *length, size_t *capacity, const char *literal) {
    size_t len;
    char *grown;
    len = strlen(literal);
    grown = jsonc_string_buffer_reserve(*buffer, capacity, *length + len + 1);
    if (grown == NULL) {
        return -1;
    }
    *buffer = grown;
    memcpy(*buffer + *length, literal, len);
    *length += len;
    (*buffer)[*length] = '\0';
    return 0;
}

static int jsonc_stringify_array(const jsonc_value *value, jsonc_format format, char **buffer, size_t *length, size_t *capacity, int depth) {
    size_t i;
    if (jsonc_append_literal(buffer, length, capacity, "[") != 0) {
        return -1;
    }
    for (i = 0; i < value->as.array.size; ++i) {
        if (i > 0) {
            if (jsonc_append_literal(buffer, length, capacity, ",") != 0) {
                return -1;
            }
        }
        if (format == JSONC_FORMAT_PRETTY) {
            if (jsonc_append_literal(buffer, length, capacity, "\n") != 0) {
                return -1;
            }
            if (jsonc_indent(buffer, length, capacity, depth + 1) != 0) {
                return -1;
            }
        }
        if (jsonc_stringify_internal(value->as.array.items[i], format, buffer, length, capacity, depth + 1) != 0) {
            return -1;
        }
    }
    if (format == JSONC_FORMAT_PRETTY && value->as.array.size > 0) {
        if (jsonc_append_literal(buffer, length, capacity, "\n") != 0) {
            return -1;
        }
        if (jsonc_indent(buffer, length, capacity, depth) != 0) {
            return -1;
        }
    }
    return jsonc_append_literal(buffer, length, capacity, "]");
}

static int jsonc_stringify_object(const jsonc_value *value, jsonc_format format, char **buffer, size_t *length, size_t *capacity, int depth) {
    size_t i;
    if (jsonc_append_literal(buffer, length, capacity, "{") != 0) {
        return -1;
    }
    for (i = 0; i < value->as.object.size; ++i) {
        if (i > 0) {
            if (jsonc_append_literal(buffer, length, capacity, ",") != 0) {
                return -1;
            }
        }
        if (format == JSONC_FORMAT_PRETTY) {
            if (jsonc_append_literal(buffer, length, capacity, "\n") != 0) {
                return -1;
            }
            if (jsonc_indent(buffer, length, capacity, depth + 1) != 0) {
                return -1;
            }
        }
        if (jsonc_append_literal(buffer, length, capacity, "\"") != 0) {
            return -1;
        }
        if (jsonc_stringify_escape(buffer, length, capacity, value->as.object.items[i].key) != 0) {
            return -1;
        }
        if (jsonc_append_literal(buffer, length, capacity, format == JSONC_FORMAT_PRETTY ? "\": " : "\":") != 0) {
            return -1;
        }
        if (jsonc_stringify_internal(value->as.object.items[i].value, format, buffer, length, capacity, depth + 1) != 0) {
            return -1;
        }
    }
    if (format == JSONC_FORMAT_PRETTY && value->as.object.size > 0) {
        if (jsonc_append_literal(buffer, length, capacity, "\n") != 0) {
            return -1;
        }
        if (jsonc_indent(buffer, length, capacity, depth) != 0) {
            return -1;
        }
    }
    return jsonc_append_literal(buffer, length, capacity, "}");
}

static int jsonc_stringify_internal(const jsonc_value *value, jsonc_format format, char **buffer, size_t *length, size_t *capacity, int depth) {
    char number_buffer[64];
    switch (jsonc_value_type(value)) {
        case JSONC_TYPE_NULL:
            return jsonc_append_literal(buffer, length, capacity, "null");
        case JSONC_TYPE_BOOLEAN:
            return jsonc_append_literal(buffer, length, capacity, value->as.boolean_value ? "true" : "false");
        case JSONC_TYPE_INTEGER:
            sprintf(number_buffer, "%ld", value->as.integer_value);
            return jsonc_append_literal(buffer, length, capacity, number_buffer);
        case JSONC_TYPE_NUMBER:
            sprintf(number_buffer, "%.17g", value->as.number_value);
            return jsonc_append_literal(buffer, length, capacity, number_buffer);
        case JSONC_TYPE_STRING:
            if (jsonc_append_literal(buffer, length, capacity, "\"") != 0) {
                return -1;
            }
            if (jsonc_stringify_escape(buffer, length, capacity, value->as.string_value) != 0) {
                return -1;
            }
            return jsonc_append_literal(buffer, length, capacity, "\"");
        case JSONC_TYPE_ARRAY:
            return jsonc_stringify_array(value, format, buffer, length, capacity, depth);
        case JSONC_TYPE_OBJECT:
            return jsonc_stringify_object(value, format, buffer, length, capacity, depth);
        default:
            return -1;
    }
}

char *jsonc_stringify(const jsonc_value *value, jsonc_format format, const jsonc_allocator *allocator) {
    char *buffer;
    size_t length;
    size_t capacity;
    char *grown;

    if (value == NULL) {
        return NULL;
    }

    (void)allocator;

    capacity = 64;
    buffer = (char *)jsonc_default_allocator.malloc_fn(capacity, NULL);
    if (buffer == NULL) {
        return NULL;
    }
    length = 0;
    buffer[0] = '\0';

    if (jsonc_stringify_internal(value, format, &buffer, &length, &capacity, 0) != 0) {
        jsonc_default_allocator.free_fn(buffer, NULL);
        return NULL;
    }

    grown = jsonc_string_buffer_reserve(buffer, &capacity, length + 1);
    if (grown == NULL) {
        jsonc_default_allocator.free_fn(buffer, NULL);
        return NULL;
    }
    buffer = grown;
    buffer[length] = '\0';
    return buffer;
}
