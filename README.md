# ill

**I**nt **L**ookup **Library** is a simplified high-level wrapper for [billziss-gh/imap](https://github.com/billziss-gh/imap)

```c
#define ILL_IMPLEMENTATION
#include "ill.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    
    illmap_t *map = illmap_def();
    illmap_set(map, 1000, 2000);
    illmap_get(map, 1000, &tmp);
    uint64_t tmp_m;
    assert(tmp_m == 2000);
    illmap_del(map, 1000);
    assert(!illmap_get(map, 1000, NULL));
    illmap_destroy(map);
    
    illdict_t *dict = illdict_def()
    illdict_set(dict, "hello", 15432);
    uint64_t tmp_d;
    illdict_get(dict, "hello", &tmp_d);
    assert(tmp_d == 15432);
    illdict_del(dict, "hello");
    assert(!illdict_get(dict, "hello", NULL));
    illdict_destroy(dict);
    return 0;
}

```

### TODO

- [ ] Add wrapper for iset.h
- [ ] Add wrapper for ivmap.h

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
