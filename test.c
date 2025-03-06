#define TCL_HASHMAP_IMPLEMENTATION
#include "hashmap.h"

#include <stdio.h>
#include <string.h>

typedef hashmap_t(unsigned int, float) map_t;

int main(int argc, char* argv[]) {
    map_t map;
    unsigned int key = 1;
    float value = 1.0f;
    hashmap_init(map, unsigned int, float);

    hashmap_set(map, key, value);
    key++; value++;
    hashmap_set(map, key, value);
    key++; value++;
    hashmap_set(map, key, value);

    size_t i;
    unsigned int caca;
    float zizi;
    hashmap_foreach(map, caca, zizi, i) {
        printf("%d %f\n", caca, zizi);
    }

    hashmap_delete(map);

    return 0;
}