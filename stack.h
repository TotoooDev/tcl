/**
 * a cool, type safe, stack library.
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

#ifndef TCL_STACK_H_
#define TCL_STACK_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * creates a new stack type.
 * usually, you would use it like this: `typedef stack_t(foo_t) stack_foo_t;` where `foo_t` is the type you want to create a stack of.
 */
#define stack_t(_type) struct { _type* buffer; size_t elem_size; size_t length; size_t capacity; }

/**
 * initializes a new stack of type `_type`.
 */
#define stack_init(_stack, _type) do { \
    (_stack).buffer = (_type*)malloc(sizeof(_type) * 5); \
    (_stack).elem_size = sizeof(_type); \
    (_stack).length = 0; \
    (_stack).capacity = 5; \
} while (0)

/**
 * deletes a stack.
 * all references to `_stack` are invalidated after this call!
 */
#define stack_delete(_stack) free((_stack).buffer)

/**
 * pushes a new value into the stack.
 */
#define stack_push(_stack, _elem) do { \
    if ((_stack).length >= (_stack).capacity) { \
        (_stack).capacity += (_stack).capacity / 2 + 1; \
        (_stack).buffer = realloc((_stack).buffer, (_stack).elem_size * (_stack).capacity); \
    }\
    (_stack).buffer[(_stack).length] = _elem; \
    (_stack).length++; \
} while (0)

/**
 * pops and returns the top value from the stack.
 */
#define stack_pop(_stack) (_stack).buffer[--(_stack).length]

/**
 * returns the top value of the stack without popping it.
 */
#define stack_top(_stack) (_stack).buffer[(_stack).length - 1]

/**
 * returns the bottom value of the stack.
 */
#define stack_bottom(_stack) (_stack).buffer[0]

/**
 * returns whether the stack is empty or not.
 */
#define stack_is_empty(_stack) (_stack).length == 0

/**
 * reserves memory for `_new_size` elements.
 * does nothing if `_new_size` is smaller than the stack capacity.
 */
#define stack_reserve(_stack, _new_size) if ((_new_size) >= (_stack).capacity) { (_stack).buffer = realloc((_stack).buffer, (_stack).elem_size * (_new_size); (_stack).capacity = new_size }

/**
 * reallocates the stack buffer to be as small as possible.
 */
#define stack_shrink(_stack) (_stack).buffer = realloc((_stack).buffer, (_stack).capacity * (_stack).elem_size)

/**
 * iterates through all the elements in the stack, bottom to top.
 */
#define stack_foreach(_stack, _elem, _i) for ((_i) = 0, (_elem) = (_stack).buffer[0]; (_i) < (_stack).length; (_i)++, (_elem) = (_stack).buffer[(_i)])

/**
 * clears the stack.
 */
#define stack_clear(_stack) do { \
    (_stack).buffer = realloc((_stack).buffer, (_stack).elem_size); \
    (_stack).length = 0; \
    (_stack).capacity = 1; \
while (0)

#endif
