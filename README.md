# table.h

Lua inspired table data type for C. Seemlessly use strings, integers or even pointers as keys by abusing C11 _Generic.

> [!NOTE]
> Built off [billziss-gh/imap](https://github.com/billziss-gh/imap)

```c
table_t table_new(void); // default hashfn (murmur) + capacity (8)
table_t table_make(table_hash hashfn, uint32_t capacity, uint64_t seed);
void table_free(table_t *table);
void table_each(table_t *table, int(*callback)(table_pair_t *pair, void*), void *userdata);
#define table_get(TABLE, KEY, VALUE) ...
#define table_set(TABLE, KEY, VALUE) ...
#define table_has(TABLE, KEY) ...
#define table_del(TABLE, KEY) ... 
```

### example

```c
#define TABLE_IMPLEMENTATION
#include "table.h"
#include <assert.h>

int main(int argc, const char *argv[]) {
    table_t table = table_new(); // create new table
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

### TODO

- [ ] Strings as values
- [ ] Shrink capacity on delete
- [ ] Documentation + examples

## License

```
MIT License

Copyright (c) 2023 Bill Zissimopoulos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

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
