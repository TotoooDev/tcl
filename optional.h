/**
 * a cool, type safe, optional library.
 * almost everything is a macro in here, because of the "dynamic typing" of the library.
 * define TCL_DEFINE_OPTIONALS exactly ONCE in a source file if you want to use the pre-defined optional types.
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

#ifndef TCL_OPTIONAL_H_
#define TCL_OPTIONAL_H_

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/**
 * creates a new optional type.
 * you should use it like this: `typedef optional_t(foo_t) foo_optional` where `foo_t` is the type you want to create an optional type of.
 */
#define optional_t(_type) struct { _type value; bool set; }

/**
 * initializes an optional.
 */
#define optional_init(_optional) (_optional).set = false

/**
 * returns the value of the optional if it was set.
 * if the value was not set with `optional_set`, this aborts the execution of the program.
 */
#define optional_get(_optional) (assert((_optional).set), (_optional).value)

/**
 * sets the value of the optional.
 */
#define optional_set(_optional, _value) do { (_optional).value = _value; (_optional).set = true; } while(0)

/**
 * resets the value of the optional.
 */
#define optional_reset(_optional) (_optional).set = false

/**
 * returns whether or not the value of the optional was set.
 */
#define optional_has_value(_optional) (_optional).set

// ==============================================
// OPTIONAL DEFINITIONS
// ==============================================

typedef optional_t(unsigned int) unsigned_int_optional_t;
typedef optional_t(int) int_optional_t;
typedef optional_t(uint8_t) uint8_optional_t;
typedef optional_t(uint16_t) uint16_optional_t;
typedef optional_t(uint32_t) uint32_optional_t;
typedef optional_t(uint64_t) uint64_optional_t;
typedef optional_t(int8_t) int8_optional_t;
typedef optional_t(int16_t) int16_optional_t;
typedef optional_t(int32_t) int32_optional_t;
typedef optional_t(int64_t) int64_optional_t;
typedef optional_t(float) float_optional_t;
typedef optional_t(double) double_optional_t;
typedef optional_t(char) char_optional_t;
typedef optional_t(void*) void_optional_t;

unsigned_int_optional_t unsigned_int_optional_new();
int_optional_t int_optional_new();
uint8_optional_t uint8_optional_new();
uint16_optional_t uint16_optional_new();
uint32_optional_t uint32_optional_new();
uint64_optional_t uint64_optional_new();
int8_optional_t int8_optional_new();
int16_optional_t int16_optional_new();
int32_optional_t int32_optional_new();
int64_optional_t int64_optional_new();
float_optional_t float_optional_new();
double_optional_t double_optional_new();
char_optional_t char_optional_new();
void_optional_t void_optional_new();

#ifdef TCL_DEFINE_OPTIONALS

unsigned_int_optional_t unsigned_int_optional_new() { unsigned_int_optional_t o; optional_init(o); return o; }
int_optional_t int_optional_new() { int_optional_t o; optional_init(o); return o; }
uint8_optional_t uint8_optional_new() { uint8_optional_t o; optional_init(o); return o; }
uint16_optional_t uint16_optional_new() { uint16_optional_t o; optional_init(o); return o; }
uint32_optional_t uint32_optional_new() { uint32_optional_t o; optional_init(o); return o; }
uint64_optional_t uint64_optional_new() { uint64_optional_t o; optional_init(o); return o; }
int8_optional_t int8_optional_new() { int8_optional_t o; optional_init(o); return o; }
int16_optional_t int16_optional_new() { int16_optional_t o; optional_init(o); return o; }
int32_optional_t int32_optional_new() { int32_optional_t o; optional_init(o); return o; }
int64_optional_t int64_optional_new() { int64_optional_t o; optional_init(o); return o; }
float_optional_t float_optional_new() { float_optional_t o; optional_init(o); return o; }
double_optional_t double_optional_new() { double_optional_t o; optional_init(o); return o; }
char_optional_t char_optional_new() { char_optional_t o; optional_init(o); return o; }
void_optional_t void_optional_new() { void_optional_t o; optional_init(o); return o; }

#endif

#endif
