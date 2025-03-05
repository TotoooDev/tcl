/**
 * a cool, type safe, hashmap library.
 * when including the file, you must define TCL_HASHMAP_IMPLEMENTATION exactly ONCE in a source file.
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

#ifndef TCL_HASHMAP_H_
#define TCL_HASHMAP_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef size_t(*hashmap_hash_func_t)(uint8_t* buffer, size_t size);
typedef void(*hashmap_iterator_t)(void* key, void* value, void* user_data);

/**
 * sets the hashing function used to hash stuff in hashmaps.
 * you should call this before everything else or you may run into trouble.
 */
void hashmap_set_hash_func(hashmap_hash_func_t func);

/**
 * sets the hashing function back to the default one (djb2, see http://www.cse.yorku.ca/~oz/hash.html).
 */
void hashmap_set_hash_func_default();

/**
 * returns the current hashing function.
 */
hashmap_hash_func_t hashmap_get_hash_func();

/**
 * constructs a pair type from two other types.
 */
#define pair_t(key_type, value_type) struct { key_type key; value_type value; size_t hash; }

/**
 * constructs a hashmap type that associates `key_type` to `value_type`.
 * usually, you would use it like this: `typedef hashmap_t(key_t, value_t) hashmap_key_value_t;`
 * do not access the fields yourself! you'd be better off using the provided functions.
 */
#define hashmap_t(key_type, value_type) struct { \
    pair_t(key_type, value_type)* pairs; \
    size_t length; \
    size_t capacity; \
    size_t pair_size; \
    size_t key_size; \
    size_t value_size; \
    size_t key_offset; \
    size_t value_offset; \
    size_t hash_offset; \
}

/**
 * deletes a hashmap.
 * all references to the hashmap are invalidated after this call.
 */
#define hashmap_delete(hashmap) free((hashmap).pairs)

/**
 * associates `key` to `value`.
 * if `key` already exists in the hashmap, it updates its value.
 */
#define hashmap_set(hashmap, key, value) tclhashmap_set(&hashmap, &key, &value)

/**
 * returns a pointer to the value associated with `key`.
 * if `key` does not exist in the hashmap, it returns `NULL`.
 */
#define hashmap_get(hashmap, key) tclhashmap_get(&hashmap, &key)

/**
 * removes `key` and its associated value from the hashmap.
 * does nothing if `key` is not found.
 */
#define hashmap_erase(hashmap, key) tclhashmap_erase(&hashmap, &key)

/**
 * returns whether `key` exists in the hashmap or not.
 */
#define hashmap_exists(hashmap, key) tclhashmap_exists(&hashmap, &key)

/**
 * calls the `iterator` for each key in the hashmap.
 */
#define hashmap_iterate(hashmap, iterator, user_data) tclhashmap_iterate(&hashmap, iterator, user_data)

/**
 * initializes a hashmap with `key_type` associated to `value_type`.
 */
#define hashmap_init(hashmap, key_type, value_type) do { \
    (hashmap).pairs = malloc(sizeof(pair_t(key_type, value_type)) * 5); \
    (hashmap).length = 0; \
    (hashmap).capacity = 5; \
    (hashmap).pair_size = sizeof(pair_t(key_type, value_type)); \
    (hashmap).key_size = sizeof(key_type); \
    (hashmap).value_size = sizeof(value_type); \
    (hashmap).key_offset = offsetof(pair_t(key_type, value_type), key); \
    (hashmap).value_offset = offsetof(pair_t(key_type, value_type), value); \
    (hashmap).hash_offset = offsetof(pair_t(key_type, value_type), hash); \
} while (0)

// you're not really supposed to use these directly

void tclhashmap_set(void* _hashmap, void* key, void* value);
void tclhashmap_erase(void* _hashmap, void* key);
void* tclhashmap_get(void* _hashmap, void* key);
bool tclhashmap_exists(void* _hashmap, void* key);
void tclhashmap_iterate(void* _hashmap, hashmap_iterator_t func, void* user_data);

// ===========================
// IMPLEMENTATION
// ===========================

#ifdef TCL_HASHMAP_IMPLEMENTATION

/**
 * the default hahsing function.
 * from http://www.cse.yorku.ca/~oz/hash.html
 */
size_t tclhashmap_hash_djb2(uint8_t* buffer, size_t size) {
    size_t hash = 5381;

    for (size_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash + *(char*)(buffer + i));
    }

    return hash;
}

/**
 * the hashing function to use.
 */
static hashmap_hash_func_t tclhashmap_hash_func = tclhashmap_hash_djb2;

/**
 * a handy macro that defines a hashmap with `uint8_t*` types (we use `uint8_t` here because it is always one byte wide).
 * it's used at the begining of every hashmap function so it can use the hashmap without knowing the types of the fields in advance.
 */
#define TCLHASHMAP_BEGIN_FUNC() \
    struct _pair_t { uint8_t* key; uint8_t* value; size_t hash; }; \
    struct _hashmap_t { \
        struct _pair_t* pairs; \
        size_t length; \
        size_t capacity; \
        size_t pair_size; \
        size_t key_size; \
        size_t value_size; \
        size_t key_offset; \
        size_t value_offset; \
        size_t hash_offset; \
    }; \
    struct _hashmap_t* hashmap = _hashmap; \

/**
 * use this to get the `i`-th pair of the `_pair_t` array.
 * you can't use `hashmap->pairs[i]` because the size of `_pair_t` is not the same as `pair_t(foo_t, bar_t)`! you would end up at a completly wrong memory location.
 * 
 * 
 * instead, we cast `hashmap->pairs` to a pointer of `uint8_t`, which we know is always one byte wide.
 * then, when can add the size of a pair times the index we want to increment the pointer to the actual location of the i-th pair.
 * cast all of this back to a `_pair_t*` and we're good to go.
 */
#define TCLHASHMAP_GET_PAIR(hashmap, i) (struct _pair_t*)((uint8_t*)(hashmap)->pairs + (hashmap)->pair_size * (i));

/**
 * returns a pointer to the key of the pair.
 * the compiler may want to pad `key` to a certain alignment even though it is the first field.
 * `pair->key` may work, but because of stuff that i do not understand about padding in memory and everything, i prefer adding a key offset just to be sure.
 */
#define TCLHASHMAP_GET_KEY(hashmap, pair) ((uint8_t*)(pair) + (hashmap)->key_offset)

/**
 * returns a pointer to the value of the pair.
 * `pair->value` will not work because of the reasons explained in `TCLHASHMAP_GET_PAIR`.
 */
#define TCLHASHMAP_GET_VALUE(hashmap, pair) ((uint8_t*)(pair) + (hashmap)->value_offset)

/**
 * returns a pointer to the hash of the pair.
 * `pair->hash` will not work because of the reasons explained in `TCLHASHMAP_GET_PAIR`.
 */
#define TCLHASHMAP_GET_HASH(hashmap, pair) ((uint8_t*)(pair) + (hashmap)->hash_offset)

void hashmap_set_hash_func(hashmap_hash_func_t hash_func) {
    tclhashmap_hash_func = hash_func;
}

void hashmap_set_hash_func_default() {
    tclhashmap_hash_func = tclhashmap_hash_djb2;
}

hashmap_hash_func_t hashmap_get_hash_func() {
    return tclhashmap_hash_func;
}

void tclhashmap_set(void* _hashmap, void* key, void* value) {
    TCLHASHMAP_BEGIN_FUNC();

    size_t hash = tclhashmap_hash_func(key, hashmap->key_size);
    for (size_t i = 0; i < hashmap->length; i++) {
        struct _pair_t* current_pair = TCLHASHMAP_GET_PAIR(hashmap, i);
        size_t* current_hash = (size_t*)TCLHASHMAP_GET_HASH(hashmap, current_pair);
        if (*current_hash == hash) {
            memcpy(&current_pair->value, value, hashmap->value_size);
            return;
        }
    }

    if (hashmap->capacity <= hashmap->length) {
        hashmap->capacity += hashmap->capacity / 2;
        hashmap->pairs = realloc(hashmap->pairs, hashmap->pair_size * hashmap->capacity);
    }

    struct _pair_t* dest_pair = TCLHASHMAP_GET_PAIR(hashmap, hashmap->length);
    void* dest_key = TCLHASHMAP_GET_KEY(hashmap, dest_pair);
    void* dest_value = TCLHASHMAP_GET_VALUE(hashmap, dest_pair);
    void* dest_hash = TCLHASHMAP_GET_HASH(hashmap, dest_pair);

    memcpy(dest_key, key, hashmap->key_size);
    memcpy(dest_value, value, hashmap->value_size);
    memcpy(dest_hash, &hash, sizeof(size_t));

    hashmap->length++;
}

void tclhashmap_erase(void* _hashmap, void* key) {
    TCLHASHMAP_BEGIN_FUNC();

    size_t hash = tclhashmap_hash_func(key, hashmap->key_size);
    for (size_t i = 0; i < hashmap->length; i++) {
        struct _pair_t* current_pair = TCLHASHMAP_GET_PAIR(hashmap, i);
        size_t* current_hash = (size_t*)TCLHASHMAP_GET_HASH(hashmap, current_pair);
        if (*current_hash == hash) {
            struct _pair_t* pair_after = TCLHASHMAP_GET_PAIR(hashmap, i + 1);
            memmove(current_pair, pair_after, hashmap->pair_size * hashmap->length - i);
            hashmap->length--;
            return;
        }
    }
}

void* tclhashmap_get(void* _hashmap, void* key) {
    TCLHASHMAP_BEGIN_FUNC();

    size_t hash = tclhashmap_hash_func(key, hashmap->key_size);
    for (size_t i = 0; i < hashmap->length; i++) {
        struct _pair_t* current_pair = TCLHASHMAP_GET_PAIR(hashmap, i);
        size_t* current_hash = (size_t*)TCLHASHMAP_GET_HASH(hashmap, current_pair);
        if (*current_hash == hash) {
            void* value = TCLHASHMAP_GET_VALUE(hashmap, current_pair);
            return (void*)value;
        }
    }

    return NULL;
}

bool tclhashmap_exists(void* _hashmap, void* key) {
    TCLHASHMAP_BEGIN_FUNC();

    size_t hash = tclhashmap_hash_func(key, hashmap->key_size);
    for (size_t i = 0; i < hashmap->length; i++) {
        struct _pair_t* current_pair = TCLHASHMAP_GET_PAIR(hashmap, i);
        size_t* current_hash = (size_t*)TCLHASHMAP_GET_HASH(hashmap, current_pair);
        if (*current_hash == hash) {
            return true;
        }
    }

    return false;
}

void tclhashmap_iterate(void* _hashmap, hashmap_iterator_t func, void* user_data) {
    TCLHASHMAP_BEGIN_FUNC();

    for (size_t i = 0; i < hashmap->length; i++) {
        struct _pair_t* current_pair = TCLHASHMAP_GET_PAIR(hashmap, i);
        void* key = TCLHASHMAP_GET_KEY(hashmap, current_pair);
        void* value = TCLHASHMAP_GET_VALUE(hashmap, current_pair);
        func(key, value, user_data);
    }
}

#endif

#endif
