/* table.h -- https://github.com/takeiteasy/ill.h

 int lookup library

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

 This library is based off https://github.com/billziss-gh/imap/
 Copyright (c) 2023 Bill Zissimopoulos. All rights reserved. */

#if __STDC_VERSION__ < 201112L // Check for C11 or later
#error "table.h requires C11 minimum"
#endif

#ifndef __TABLE_HEADER
#define __TABLE_HEADER
#ifdef __cplusplus
extern "C" {
#endif

#ifdef TABLE_IMPLEMENTATION
#define ILL_IMPLEMENTATION
#endif
#include "ill.h"

typedef illmap_t table_t;

#define table_new(OLD, CAPACITY) (illmap((OLD), (CAPACITY)))
#define table_def() illmap(NULL, 0)
#define table_set(TABLE, KEY, VALUE) \
    _Generic((KEY), \
        uint64_t: illmap_set, \
        const char*: illdict_set, \
        default: illmap_set \
    )((TABLE), (KEY), (VALUE))
#define table_get(TABLE, KEY, VALUE) \
    _Generic((KEY), \
        uint64_t: illmap_get, \
        const char*: illdict_get, \
        default: illmap_get \
    )((TABLE), (KEY), (VALUE))
#define table_has(TABLE, KEY) \
    _Generic((KEY), \
        uint64_t: illmap_has, \
        const char*: illdict_has, \
        default: illmap_has \
    )((TABLE), (KEY))
#define table_destroy(TABLE) (illmap_destroy((TABLE)))

#ifdef __cplusplus
}
#endif
#endif // __TABLE_HEADER
