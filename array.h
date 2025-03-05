/**
 * a cool, type safe, dynamic array library.
 * almost everything is a macro in here, because of the "dynamic typing" of the library.
 * define TCL_DEFINE_ARRAYS exactly ONCE in a source file if you want to use the pre-defined array types.
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

#ifndef TCL_ARRAY_H_
#define TCL_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/**
 * creates a new array type.
 * you should use it like this: `typedef array_t(foo_t) array_foo_t` where `foo_t` is the type you want to create an array type of.
 */
#define array_t(type) struct { type* buffer; size_t capacity; size_t elem_size; size_t length; bool(*compare_function)(type, type); }

/**
 * deletes `array` and frees all related memory.
 * all pointers to `array` become invalid after this call.
 */
#define array_delete(array) do { if ((array).buffer != NULL) { free((array).buffer); } } while (0)

/**
 * increases the capacity of the array. this does nothing if `new_size` is smaller than the current capacity.
 */
#define array_reserve(array, new_size) if ((new_size) > (array).capacity) { (array).buffer = malloc((new_size) * (array).elem_size); (array).capacity = new_size; }

/**
 * reallocates the array buffer to be as small as possible.
 */
#define array_shrink(array) (array).buffer = realloc((array).buffer, (array).elem_size * (array).length); (array).capacity = (array).length

/**
 * removes the last element in the array.
 */
#define array_pop(array) array_remove_index((array), (array).length - 1)

/**
 * returns the element located at index `index`.
 */
#define array_get(array, index) (assert((index) >= 0 && (index) < (array).length), (array).buffer[(index)])

/**
 * returns the first element in the array.
 */
#define array_front(array) (array).buffer[0]

/**
 * returns the last element in the array.
 */
#define array_back(array) (array).buffer[(array).length - 1]

/**
 * returns whether or not the array is empty.
 */
#define array_is_empty(array) (array).length == 0

/**
 * returns the capacity of the array buffer, that is the maximum number of elements the array can contain before needing to be resized.
 */
#define array_get_capacity(array) (array).capacity

/**
 * returns the number of elements in the array.
 */
#define array_get_length(array) (array).length

/**
 * iterates through all the elements in the array. `elem` is set to the current element, `i` is set to the index of the element.
 */
#define array_foreach(array, elem, i) for (i = 0, elem = (array).buffer[0]; i < (array).length; i++, elem = (array).buffer[i])

/**
 * initializes the array.
 * `type` is the type of the elements, not the type of the array!
 * `func` is the function that compares two elements. you don't need to define it and can simply set it to `NULL`, but you won't be able to use `array_get_index` and `array_remove`.
 */
#define array_init(array, type, func) do { \
    (array).buffer = malloc(sizeof(type) * 5); \
    (array).capacity = 5; \
    (array).elem_size = sizeof(type); \
    (array).length = 0; \
    (array).compare_function = func; \
} while (0)

/**
 * adds an element in the array.
 */
#define array_add(array, elem) do { \
    if ((array).capacity <= (array).length) { \
        (array).capacity += (array).capacity / 2 + 1; \
        (array).buffer = realloc((array).buffer, (array).elem_size * (array).capacity); \
    } \
    (array).buffer[(array).length] = elem; \
    (array).length++; \
} while (0)

/**
 * insert an element at the given index.
 */
#define array_insert(array, elem, index) do { \
    assert((index) > 0 && (index) < (array).length); \
    if ((array).capacity <= (array).length) { \
        (array).capacity += (array).capacity / 2 + 1; \
        (array).buffer = realloc((array).buffer, (array).elem_size * (array).capacity); \
    } \
    memmove(&(array).buffer[(index) + 1], &(array).buffer[(index)], (array).length - (index)); \
    (array).buffer[(index)] = elem; \
    (array).length++; \
} while (0)

/**
 * returns the index of the first occurence of an element in the array.
 * you need to have specified a compare function in `array_init` to use this macro.
 */
#define array_get_index(array, elem, i) do { \
    for (i = 0; i < (array).length; i++) { \
        if ((array).compare_function((array).buffer[i], elem)) { \
            break; \
        } \
    } \
    if (i == (array).length) { \
        i = -1; \
    } \
} while (0)

/**
 * removes the element located at `index` in the array.
 */
#define array_remove_index(array, index) do { \
    assert((index) > 0 && (index) < (array).length); \
    if ((index) < (array).length) { \
        for (size_t i = (index); i < (array).length - 1; i++) { \
            (array).buffer[i] = (array).buffer[i + 1]; \
        } \
        (array).length--; \
    } \
} while (0)

/**
 * removes the first occurence of `elem` in the array.
 */
#define array_remove(array, elem) do { \
    size_t index; \
    array_get_index(array, elem, index); \
    if (index != -1) { \
        array_remove_index(array, index); \
    } \
} while (0)

/**
 * clears the array, making the length 0 and the capacity 1.
 * all contents will be erased!
 */
#define array_clear(array) do { \
    (array).buffer = realloc((array).buffer, (array).elem_size); \
    (array).capacity = 1; \
    (array).length = 0; \
} while (0)

// ==============================================
// ARRAY DEFINITIONS
// ==============================================

// no i'm not writing docs for all of this
// sorry :)

bool unsigned_int_compare(unsigned int a, unsigned int b);
bool int_compare(int a, int b);
bool uint8_compare(uint8_t a, uint8_t b);
bool uint16_compare(uint16_t a, uint16_t b);
bool uint32_compare(uint32_t a, uint32_t b);
bool uint64_compare(uint64_t a, uint64_t b);
bool int8_compare(int8_t a, int8_t b);
bool int16_compare(int16_t a, int16_t b);
bool int32_compare(int32_t a, int32_t b);
bool int64_compare(int64_t a, int64_t b);
bool float_compare(float a, float b);
bool double_compare(double a, double b);
bool char_compare(char a, char b);
bool str_compare(char* a, char* b);
bool void_compare(void* a, void* b);

typedef array_t(unsigned int)   unsigned_int_array_t;
typedef array_t(int)            int_array_t;
typedef array_t(uint8_t)        uint8_array_t;
typedef array_t(uint16_t)       uint16_array_t;
typedef array_t(uint32_t)       uint32_array_t;
typedef array_t(uint64_t)       uint64_array_t;
typedef array_t(int8_t)         int8_array_t;
typedef array_t(int16_t)        int16_array_t;
typedef array_t(int32_t)        int32_array_t;
typedef array_t(int64_t)        int64_array_t;
typedef array_t(float)          float_array_t;
typedef array_t(double)         double_array_t;
typedef array_t(char)           char_array_t;
typedef array_t(char*)          str_array_t;
typedef array_t(void*)          void_array_t;

unsigned_int_array_t   unsigned_int_array_new();
int_array_t            int_array_new();
uint8_array_t          uint8_array_new();
uint16_array_t         uint16_array_new();
uint32_array_t         uint32_array_new();
uint64_array_t         uint64_array_new();
int8_array_t           int8_array_new();
int16_array_t          int16_array_new();
int32_array_t          int32_array_new();
int64_array_t          int64_array_new();
float_array_t          float_array_new();
double_array_t         double_array_new();
char_array_t           char_array_new();
str_array_t            str_array_new();
void_array_t           void_array_new();

#ifdef TCL_DEFINE_ARRAYS

bool unsigned_int_compare(unsigned int a, unsigned int b) { return a == b; }
bool int_compare(int a, int b) { return a == b; }
bool uint8_compare(uint8_t a, uint8_t b) { return a == b; }
bool uint16_compare(uint16_t a, uint16_t b) { return a == b; }
bool uint32_compare(uint32_t a, uint32_t b) { return a == b; }
bool uint64_compare(uint64_t a, uint64_t b) { return a == b; }
bool int8_compare(int8_t a, int8_t b) { return a == b; }
bool int16_compare(int16_t a, int16_t b) { return a == b; }
bool int32_compare(int32_t a, int32_t b) { return a == b; }
bool int64_compare(int64_t a, int64_t b) { return a == b; }
bool float_compare(float a, float b) { return a == b; }
bool double_compare(double a, double b) { return a == b; }
bool char_compare(char a, char b) { return a == b; }
bool str_compare(char* a, char* b) { return strcmp(a, b) == 0; }
bool void_compare(void* a, void* b) { return a == b; }

unsigned_int_array_t   unsigned_int_array_new() { unsigned_int_array_t a; array_init(a, unsigned int, unsigned_int_compare); return a; }
int_array_t            int_array_new() { int_array_t a; array_init(a, int, int_compare); return a; }
uint8_array_t          uint8_array_new()  { uint8_array_t a;  array_init(a, uint8_t,  uint8_compare);  return a; }
uint16_array_t         uint16_array_new() { uint16_array_t a; array_init(a, uint16_t, uint16_compare); return a; }
uint32_array_t         uint32_array_new() { uint32_array_t a; array_init(a, uint32_t, uint32_compare); return a; }
uint64_array_t         uint64_array_new() { uint64_array_t a; array_init(a, uint64_t, uint64_compare); return a; }
int8_array_t           int8_array_new()  { int8_array_t a;  array_init(a, int8_t,  int8_compare);  return a; }
int16_array_t          int16_array_new() { int16_array_t a; array_init(a, int16_t, int16_compare); return a; }
int32_array_t          int32_array_new() { int32_array_t a; array_init(a, int32_t, int32_compare); return a; }
int64_array_t          int64_array_new() { int64_array_t a; array_init(a, int64_t, int64_compare); return a; }
float_array_t          float_array_new()  { float_array_t a;  array_init(a, float, float_compare);   return a; }
double_array_t         double_array_new() { double_array_t a; array_init(a, double, double_compare); return a; }
char_array_t           char_array_new() { char_array_t a; array_init(a, char, char_compare); return a; }
str_array_t            str_array_new() { str_array_t a; array_init(a, char*, str_compare); return a; }
void_array_t           void_array_new() { void_array_t a; array_init(a, void*, void_compare); return a; }

#endif

#endif
