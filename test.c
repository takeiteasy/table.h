#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>
#include <stdio.h>

void handleCharChar(char* a, char *b) {
    printf("handleCharChar\n");
}

void handleCharInt(char* a, int b) {
    printf("handleCharInt\n");
}

void handleIntChar(int a, char *b) {
    printf("handleIntChar\n");
}

void handleIntInt(int a, int b) {
    printf("handleIntInt\n");
}

#define myFunc(a, b)                            \
  _Generic((a),                                 \
         char*: _Generic((b),                   \
                         char*: handleCharChar, \
                           int: handleCharInt), \
           int: _Generic((b),                   \
                         char*: handleIntChar,  \
                           int: handleIntInt)   \
  )(a,b)

#define mk_fptr(a,b) (void(*)(typeof(a), typeof(b)))0
#define handle(a,b) _Generic(mk_fptr(a,b),                      \
                            void(*)(char*,char*): handleCharChar, \
                            void(*)(char*,  int): handleCharInt,  \
                            void(*)(  int,char*): handleIntChar,  \
                            void(*)(  int,  int): handleIntInt    \
                            )(a,b)

int main(int argc, const char *argv[]) {
    char *a  = "test";
    int b = 20;
    
    myFunc(a, a);
    myFunc(a, b);
    myFunc(b, a);
    myFunc(b, b);
    
    handle(a, a);
    handle(a, b);
    handle(b, a);
    handle(b, b);

    uint64_t tmp;
    basic_imap_t map = int_map_new();
    assert(int_map_set(&map, 1000, 2000));
    assert(int_map_get(&map, 1000, &tmp));
    assert(tmp == 2000);
    assert(int_map_del(&map, 1000));
    assert(!int_map_get(&map, 1000, NULL));
    int_map_free(&map);
    
    table_t table = table_new();
    table_free(&table);
    return 0;
}
