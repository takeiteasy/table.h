# table.h

> [!WARNING]
> table.h relies on C11 features (_Generic) and clang/gcc extensions ([blocks](https://clang.llvm.org/docs/LanguageExtensions.html#blocks) + [typeof](https://gcc.gnu.org/onlinedocs/gcc/Typeof.html)). Work in progress but functional. See the [build](#build) section below.

Lua inspired table data type for C. Seemlessly use strings, integers or even pointers as keys by abusing C11 _Generic. It is built off [imap](https://github.com/billziss-gh/imap) (MIT) by [Bill Zissimopoulos](https://github.com/billziss-gh).

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

### Example

```c
#define TABLE_IMPLEMENTATION
#include "table.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct dumb {
    int i;
    float f;
};

int main(int argc, const char *argv[]) {
    table_t table = table();
    // add entries (note: string keys + values are duped + freed automatically)
    // str -> str, str -> ptr, int -> int
    struct dumb dumb = {1, 2.f}; 
    table_set(&table, "test1", "hello");
    table_set(&table, "test2", &dumb);
    table_set(&table, 100, 200);
    // assert the table has the keys
    assert(table_has(&table, "test1") &&
           table_has(&table, "test2") &&
           table_has(&table, 100));
    // remove an entry
    table_del(&table, 100);
    // assert its gone
    assert(!table_has(&table, 100));
    int dummy;
    assert(!table_get(&table, 100, &dummy));
    // get the string and struct from the table
    const char *hello = NULL;
    assert(table_get(&table, "test1", &hello));
    assert(!strncmp("hello", hello, 5));
    struct dumb *ptr = NULL;
    assert(table_get(&table, "test2", &ptr));
    assert(ptr && ptr->i == dumb.i && ptr->f == dumb.f);
    // loop through each table value (unordered), the key argument
    // will be null unless the entry uses a string key
    table_each(&table, NULL, ^(table_t *table, const char *key, table_entry_t *entry, void *userdata) {
        if (key)
            printf("Key: %s, Value: %llu\n", key, entry->value);
        else
            printf("Value: %llu\n", entry->value);
    });
    // loop through table string keys
    table_keys(&table, NULL, ^(table_t *table, const char *key, void *userdata) {
        printf("Key: %s\n", key);
    });
    // free up all the memory
    table_free(&table);
    return 0;
}
```

## Build

> [!NOTE]
> table.h requires clang>=15 or gcc>=10.4

Since this library relies on the clang/gcc apple blocks extension, you may need to add `-fblocks` to the build command. If you're running Linux you may also need to install [blocks runtime](https://mackyle.github.io/blocksruntime/) and add `-lBlocksRuntime` as well. Other than that you may need to specify `-std=c11`.

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
