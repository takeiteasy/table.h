#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    uint64_t tmp;
    illmap_t map;
    illmap(&map, 0);
    assert(illmap_set(&map, 1000, 2000));
    assert(illmap_get(&map, 1000, &tmp));
    assert(tmp == 2000);
    assert(illmap_del(&map, 1000));
    assert(!illmap_get(&map, 1000, NULL));
    illmap_free(&map);
    return 0;
}
