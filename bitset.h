/**
 * a cool, length-based string library.
 * when including the file, you must define TCL_BITSET_IMPLEMENTATION exactly ONCE in a source file.
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

#ifndef TCL_BITSET_H_
#define TCL_BITSET_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct bitset_t {
    uint8_t* bits;
    size_t num_uint8;
    size_t num_bits;
} bitset_t;

/**
 * creates a new bitset of `num_bits` bits.
 */
bitset_t bitset_new(size_t num_bits);

/**
 * deletes a bitset.
 * all references to this bitset are invalidated after this call.
 */
void bitset_delete(bitset_t bitset);

/**
 * returns whether or not bit `n` is set to 1.
 */
bool bitset_get(bitset_t bitset, size_t n);

/**
 * sets bit `n` to 1.
 */
void bitset_set(bitset_t bitset, size_t n);

/**
 * sets bit `n` to 0.
 */
void bitset_reset(bitset_t bitset, size_t n);

/**
 * flips bit `n` (0 <=> 1).
 */
void bitset_flip(bitset_t bitset, size_t n);

/**
 * inverts a bitset.
 */
void bitset_not(bitset_t bitset);

/**
 * performs an `and` operation on `a` with `b`.
 */
void bitset_and(bitset_t a, bitset_t b);

/**
 * performs an `or` operation on `a` with `b`.
 */
void bitset_or(bitset_t a, bitset_t b);

/**
 * performs an `exclusive or` operation on `a` with `b`.
 */
void bitset_xor(bitset_t a, bitset_t b);

/**
 * returns a pointer to a null terminated string that represents the bitset in a human-readable format.
 * you NEED to free the returned pointer yourself after using it!
 */
char* bitset_c_str(bitset_t bitset);

#ifdef TCL_BITSET_IMPLEMENTATION

#define TCL_CEILING(x, y) (((x) + (y) - 1) / (y))
#define TCL_MAX(x, y) (x) > (y) ? (x) : (y)

bitset_t bitset_new(size_t num_bits) {
    bitset_t bitset;
    bitset.num_bits = num_bits;
    bitset.num_uint8 = TCL_CEILING(num_bits, sizeof(uint8_t));
    bitset.bits = (uint8_t*)malloc(sizeof(uint8_t) * bitset.num_uint8);
    return bitset;
}

void bitset_delete(bitset_t bitset) {
    free(bitset.bits);
}

bool bitset_get(bitset_t bitset, size_t n) {
    assert(n >= 0 && n < bitset.num_bits);
    return (bitset.bits[n / 8] >> n % 8) & 1 == 1;
}

void bitset_set(bitset_t bitset, size_t n) {
    assert(n >= 0 && n < bitset.num_bits);
    bitset.bits[n / 8] |= 1 << (n % 8);
}

void bitset_reset(bitset_t bitset, size_t n) {
    assert(n >= 0 && n < bitset.num_bits);
    bitset.bits[n / 8] &= ~(1 << (n % 8));
}

void bitset_flip(bitset_t bitset, size_t n) {
    assert(n >= 0 && n < bitset.num_bits);
    bitset.bits[n / 8] ^= 1 << (n % 8);
}

void bitset_not(bitset_t bitset) {
    for (size_t i = 0; i < bitset.num_uint8; i++) {
        bitset.bits[i] = ~bitset.bits[i];
    }
}

void bitset_and(bitset_t a, bitset_t b) {
    size_t max = TCL_MAX(a.num_uint8, b.num_uint8);
    for (size_t i = 0; i < max; i++) {
        a.bits[i] &= b.bits[i];
    }
}

void bitset_or(bitset_t a, bitset_t b) {
    size_t max = TCL_MAX(a.num_uint8, b.num_uint8);
    for (size_t i = 0; i < max; i++) {
        a.bits[i] |= b.bits[i];
    }
}

void bitset_xor(bitset_t a, bitset_t b) {
    size_t max = TCL_MAX(a.num_uint8, b.num_uint8);
    for (size_t i = 0; i < max; i++) {
        a.bits[i] ^= b.bits[i];
    }
}

char* bitset_c_str(bitset_t bitset) {
    size_t length = bitset.num_bits + 3;
    char* str = (char*)malloc(sizeof(char) * length);
    str[0] = '0';
    str[1] = 'b';

    for (size_t i = 0; i < bitset.num_bits; i++) {
        if (bitset_get(bitset, i)) {
            str[i + 2] = '1';
        }
        else {
            str[i + 2] = '0';
        }
    }

    str[length - 1] = '\0';
    return str;
}

#endif

#endif
