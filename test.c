#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    table_t table = table_new();
    table_set(&table, "test", 100);
    int a, b;
    table_get(&table, "test", &a);
    assert(a == 100);
    table_set(&table, a, 200);
    table_get(&table, a, &b);
    assert(b == 200);
    table_free(&table);
    
    uint64_t tmp;
    unordered_map_t map = unordered_map_new();
    assert(unordered_map_set(&map, 1000, 2000));
    assert(unordered_map_get(&map, 1000, &tmp));
    assert(tmp == 2000);
    assert(unordered_map_del(&map, 1000));
    assert(!unordered_map_get(&map, 1000, NULL));
    unordered_map_free(&map);
    return 0;
}
