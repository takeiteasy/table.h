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

#define _T(T) \
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
    _Generic((int(*)[_T(V)])NULL, \
        int(*)[T_INT]: _table_int_to_int, \
        int(*)[T_STR]: _table_str_to_int, \
        default: _table_void_to_int \
    )((T), (V))

 #define table_set(T, A, B) \
    _Generic((int(*)[_T(A)][_T(B)])NULL, \
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

#define table_get(T, K, V)

int main(int argc, const char *argv[]) {
    table_t table = table();
    dummy_t poo = {1, 1.f}; 
    table_set(&table, "test1", "poopoo");
    table_set(&table, "test2", &poo);
    table_set(&table, 100, 200);
    table_free(&table);
    return 0;
}
