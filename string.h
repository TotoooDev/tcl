/**
 * a cool, length-based string library.
 * when including the file, you must define TCL_STRING_IMPLEMENTATION exactly ONCE in a source file.
 * 
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org/>
 */

#ifndef TCL_STRING_H_
#define TCL_STRING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * the string type.
 * `buffer` is the chracter buffer of the string.
 * `length` is the length of the string.
 * `length` may not actually correspond to the length retrieved by using `strlen` on the buffer!
 */
typedef struct string_t {
    char* buffer;
    size_t length;
} string_t;

/**
 * creates a new string from a buffer and its length.
 * you need to call `string_delete` on the returned string.
 */
string_t string_new(const char* buffer, size_t length);

/**
 * creates a new string from a null-terminated string (c-style).
 * you need to call `string_delete` on the returned string.
 */
string_t string_new_from_cstr(const char* str);

/**
 * duplicates a string.
 * you need to call `string_delete` on the returned string.
 */
string_t string_duplicate(string_t string);

/**
 * deletes a string. this frees the string's buffer.
 * any pointer to the buffer will be invalidated.
 */
void string_delete(string_t string);

/**
 * returns the character at a certain index in the string.
 */
char string_get(string_t string, size_t index);

/**
 * returns the first character of the string.
 */
char string_front(string_t string);

/**
 * return the last character of the string.
 */
char string_back(string_t string);

/**
 * return whether the string is empty or not.
 */
bool string_is_empty(string_t string);

/**
 * compares two strings.
 */
bool string_is_equal(string_t a, string_t b);

/**
 * returns whether `b` is contained in `a` or not.
 */
bool string_contains(string_t a, string_t b);

/**
 * appends `b` to `a`.
 */
void string_append(string_t* a, string_t b);

/**
 * appends `c` to `a`.
 */
void string_append_char(string_t* a, char c);

/**
 * erases `b` from `a`. this does nothing if `b` is not contained in `a`.
 */
void string_erase(string_t* a, string_t b);

/**
 * creates a substring of `string` from a certain index of a certain length.
 * you need to call `string_delete` on the returned string.
 */
string_t string_substr(string_t string, size_t index, size_t length);

/**
 * returns the index of `b` inside of `a`.
 * return -1 if `b` is not contained in `a`
 */
int64_t string_find(string_t a, string_t b);

/**
 * returns the index of `c` inside of `a`.
 * return -1 if `c` is not contained in `a`
 */
int64_t string_find_char(string_t a, char c);

/**
 * returns the index of the last iteration of `b` inside of `a`.
 * return -1 if `b` is not contained in `a`
 */
int64_t string_find_last_of(string_t a, string_t b);

/**
 * returns the index of the last iteration of `c` inside of `a`.
 * return -1 if `c` is not contained in `a`
 */
int64_t string_find_char_last_of(string_t a, char c);

/**
 * returns the null-terminated version of a string.
 * you need to free the returned value youself.
 */
char* string_c_str(string_t string);

// ===========================
// IMPLEMENTATION
// ===========================

#ifdef TCL_STRING_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <assert.h>

string_t string_new(const char* buffer, size_t length) {
    string_t string;
    string.length = length;
    string.buffer = (char*)malloc(sizeof(char) * string.length);
    memcpy(string.buffer, buffer, string.length);
    return string;
}

string_t string_new_from_cstr(const char* str) {
    string_t string;
    string.length = strlen(str);
    string.buffer = (char*)malloc(sizeof(char));
    strcpy(string.buffer, str);
    return string;
}

string_t string_duplicate(string_t string) {
    return string_new(string.buffer, string.length);
}

void string_delete(string_t string) {
    free(string.buffer);
}

char string_get(string_t string, size_t index) {
    assert(index > 0 && index < string.length);
    return string.buffer[index];
}

char string_front(string_t string) {
    return string.buffer[0];
}

char string_back(string_t string) {
    return string.buffer[string.length - 1];
}

bool string_is_empty(string_t string) {
    return string.length == 0;
}

bool string_is_equal(string_t a, string_t b) {
    if (a.length != b.length) {
        return false;
    }

    for (size_t i = 0; i < a.length; i++) {
        if (a.buffer[i] != b.buffer[i]) {
            return false;
        }
    }

    return true;
}

bool string_contains(string_t a, string_t b) {
    return string_find(a, b) != -1;
}

void string_append(string_t* a, string_t b) {
    a->buffer = (char*)realloc(a->buffer, sizeof(char) * (a->length + b.length));
    memcpy(a->buffer + a->length, b.buffer, b.length);
    a->length += b.length;
}

void string_append_char(string_t* a, char c) {
    a->buffer = (char*)realloc(a->buffer, sizeof(char) * a->length);
    a->buffer[a->length] = c;
    a->length++;
}

void string_erase(string_t* a, string_t b) {
    int64_t index = string_find(*a, b);
    if (index == -1) {
        return;
    }

    char* b_start = a->buffer + index;
    char* b_end = a->buffer + index + b.length;
    memmove(b_start, b_end, a->length - index - b.length);

    a->length -= b.length;
}

string_t string_substr(string_t string, size_t index, size_t length) {
    return string_new(string.buffer + index, length);
}

int64_t string_find(string_t a, string_t b) {
    if (a.length < b.length) {
        return -1;
    }

    for (size_t i = 0; i < a.length; i++) {
        size_t index_a = i;
        size_t index_b;
        for (index_b = 0; index_b < b.length; index_b++) {
            if (a.buffer[index_a] != b.buffer[index_b]) {
                break;
            }
            index_a++;
        }

        if (index_b == b.length) {
            return i;
        }
    }

    return -1;
}

int64_t string_find_char(string_t a, char c) {
    for (size_t i = 0; i < a.length; i++) {
        if (a.buffer[i] == c) {
            return i;
        }
    }

    return -1;
}

int64_t string_find_last_of(string_t a, string_t b) {
    if (a.length < b.length) {
        return -1;
    }

    for (size_t i = a.length - 1; i <= 0; i--) {
        size_t index_a = i;
        size_t index_b;
        for (index_b = b.length - 1; index_b <= 0; index_b--) {
            if (a.buffer[index_a] != b.buffer[index_b]) {
                break;
            }
            index_a--;
        }

        if (index_b == b.length) {
            return index_a;
        }
    }

    return -1;
}

int64_t string_find_char(string_t a, char c) {
    for (size_t i = a.length - 1; i <= 0; i--) {
        if (a.buffer[i] == c) {
            return i;
        }
    }

    return -1;
}

char* string_c_str(string_t string) {
    char* c_str = (char*)malloc(sizeof(char) * (string.length + 1));
    memcpy(c_str, string.buffer, string.length);
    c_str[string.length] = '\0';
    return c_str;
}

#endif

#endif
