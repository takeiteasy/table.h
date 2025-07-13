#define TABLE_IMPLEMENTATION
#include "table.h"
#include <stdio.h>

typedef struct test {
    int dummyA;
    float dummyB;
} dummy_t;

int main(int argc, const char *argv[]) {
    table_t table = table();

    dummy_t poo = {1, 2.f}; 
    table_set(&table, "test1", "poopoo");
    table_set(&table, "test2", &poo);
    table_set(&table, 100, 200);

    if (!table_has(&table, "test1") ||
        !table_has(&table, "test2") ||
        !table_has(&table, 100))
        return 1;

    table_del(&table, 100);
    if (table_has(&table, 100))
        return 1;
    int dummy = 0;
    if (table_get(&table, 100, &dummy))
        return 1;

    const char *poopoo = NULL;
    if (!table_get(&table, "test1", &poopoo))
        return 1;
    dummy_t *poo_ptr = NULL;
    if (!table_get(&table, "test2", &poo_ptr))
        return 1;
    
    table_each(&table, NULL, ^(table_t *table, const char *key, table_entry_t *entry, void *userdata) {
        if (key)
            printf("Key: %s, Value: %llu\n", key, entry->value);
        else
            printf("Value: %llu\n", entry->value);
    });

    table_keys(&table, NULL, ^(table_t *table, const char *key, void *userdata) {
        printf("Key: %s\n", key);
    });

    table_free(&table);
    return 0;
}
