#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>
#include <stdio.h>

typedef struct test {
    int dummyA;
    float dummyB;
} dummy_t;

enum _table_type {
    T_INT,
    T_STR,
    T_VOID
};

#define _T_TYPE(T) \
    _Generic((T), \
        char: T_INT, \
        short: T_INT, \
        int: T_INT, \
        long: T_INT, \
        long long: T_INT, \
        unsigned char: T_INT, \
        unsigned short: T_INT, \
        unsigned int: T_INT, \
        unsigned long: T_INT, \
        unsigned long long: T_INT, \
        char*: T_STR, \
        const char*: T_STR, \
        unsigned char*: T_STR, \
        const unsigned char*: T_STR, \
        void*: T_VOID, \
        default: T_VOID)

#define _T_COERCE(T, V) \
    _Generic((int(*)[_T_TYPE(V)])NULL, \
        int(*)[T_INT]: _table_int_to_int, \
        int(*)[T_STR]: _table_str_to_int, \
        default: _table_void_to_int \
    )((T), (V))

 #define table_set(T, A, B) \
    _Generic((int(*)[_T_TYPE(A)][_T_TYPE(B)])NULL, \
        int(*)[T_INT][T_INT]:   _table_set_int, \
        int(*)[T_INT][T_STR]:   _table_set_int, \
        int(*)[T_INT][T_VOID]:  _table_set_int, \
        int(*)[T_STR][T_INT]:   _table_set_str, \
        int(*)[T_STR][T_STR]:   _table_set_str, \
        int(*)[T_STR][T_VOID]:  _table_set_str, \
        int(*)[T_VOID][T_INT]:  _table_set_void, \
        int(*)[T_VOID][T_STR]:  _table_set_void, \
        int(*)[T_VOID][T_VOID]: _table_set_void \
    )((T), (A), _T_COERCE((T), (B)))

#define table_get(T, K, V) \
    (^(table_t *_t, typeof(K) _k, typeof(V) _v) { \
        printf("table_get: %d\n", _k); \
        uint64_t _out = 0; \
        imap_get(&_t->map, (uint64_t)_k, &_out); \
        table_entry_t *_entry = (table_entry_t*)_out; \
        if (!_entry) \
            return false; \
        *_v = (typeof(*_v))_entry->value; \
        return true; \
    })((T), (K), (V))

extern int imap_get(imap_t *map, uint64_t key, uint64_t *val);

int main(int argc, const char *argv[]) {
    table_t table = table();
    dummy_t poo = {1, 1.f}; 
    table_set(&table, "test1", "poopoo");
    table_set(&table, "test2", &poo);
    table_set(&table, 100, 200);

    int v = 0;
    if (!table_get(&table, 100, &v))
        return 1;
    const char *poopoo = NULL;
    if (!table_get(&table, "test1", &poopoo))
        return 1; // todo

    // test1: 10363722223966044716
    // test2: 3161135047519575325
    
    // 5133844608

    uint64_t key1 = _table_murmur("test1", 5, 0);
    uint64_t key2 = _table_murmur("test2", 5, 0);
    uint64_t key3 = 0, key4 = 0;
    int a = imap_get(&table.map, key1, &key3);
    int b = imap_get(&table.map, key2, &key4);

    table_entry_t *entry1 = (table_entry_t*)key3;
    const char *str1 = (const char*)entry1->value;
    table_entry_t *entry2 = (table_entry_t*)key4;
    dummy_t *poo_ptr = (dummy_t*)entry2->value;

    table_free(&table);
    return 0;
}
