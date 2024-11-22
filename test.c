#define ILL_IMPLEMENTATION
#include "ill.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    uint64_t tmp;
    
    illmap_t *map = illmap_def();
    assert(illmap_set(map, 1000, 2000));
    assert(illmap_get(map, 1000, &tmp));
    assert(tmp == 2000);
    assert(illmap_del(map, 1000));
    assert(!illmap_get(map, 1000, NULL));
    illmap_destroy(map);
    
    illdict_t *dict = illdict_def()
    assert(illdict_set(dict, "hello", 15432));
    assert(illdict_get(dict, "hello", &tmp));
    assert(illdict_del(dict, "hello"));
    assert(!illdict_get(dict, "hello", NULL));
    illdict_destroy(dict);
    return 0;
}
