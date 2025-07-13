# table.h

> [!WARNING]
> Work in progress

Lua inspired table data type for C. Seemlessly use strings, integers or even pointers as keys by abusing C11 _Generic.

### API

```c
table_t table(void);
void table_free(table_t *table);
bool table_set(table_t, KEY, VALUE);
bool table_get(table_t, KEY, &VALUE);
bool table_has(table_t, KEY);
bool table_del(table_t, KEY);
// void(*^callback)(table_t *table, const char *key, table_entry_t *entry, void *userdata);
void table_each(table_t, CALLBACK, USERDATA);
// void(*^callback)(table_t *table, const char *key, void *userdata);
void table_keys(table_t, CALLBACK, USERDATA);
```

### example

```c
#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>

int main(int argc, const char *argv[]) {
    table_t table = table(); // create new table
    table_set(&table, "test", 100); // set key "test" to 100
    // keys can be:
    //  - (const)? (unsigned)? char* (hashed to uint64_t)
    //  - Any singed/unsigned int type (char, short, int, long, long long)
    //  - void* (pointer address converted to uint64_t)
    // values can be:
    //  - Any singed/unsigned int type (char, short, int, long, long long)
    //  - void* (pointer address converted to uint64_t)
    // NOTE: void* can be any pointer, be careful if passing a reference to a stack allocation
    int a, b;
    table_get(&table, "test", &a); // get key "test"
    assert(a == 100); // is it 100? yes
    table_set(&table, a, 200);  // set the key of a (100) to 200
    table_get(&table, 100, &b); // get key of 100
    assert(b == 200); // is it 200? yes
    table_free(&table); // clean up
    return 0;
}
```

## License

```
 Copyright (C) 2024  George Watson

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
