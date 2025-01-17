#!/usr/bin/env python3
import re

int_types = {
    'char': 's8',
    'short': 's16',
    'int': 's32',
    'long': 'sl32',
    'long long': 's64',
    'unsigned char': 'u8',
    'unsigned short': 'u16',
    'unsigned int': 'u32',
    'unsigned long': 'ul32',
    'unsigned long long': 'u64'}

str_types = {'char*': 'c',
             'const char*': 'cc',
             'unsigned char*': 'uc',
             'const unsigned char*': 'cuc'}

other_types = {'void*': 'v'}

key_types = str_types | int_types | other_types
val_types = int_types | other_types

functions = []

with open("table.h", 'r') as f:
    og_lines = [[]]
    idx = 0
    for line in f:
        if re.match(r"^\/\/ [BEGIN|END]+ [HEADER|SOURCE]+$", line):
            og_lines.append([])
            idx += 1
        else:
            og_lines[idx].append(line.rstrip())

with open("table.h", 'w') as f:
    def printf(msg):
        print(msg, file=f)
    
    printf("\n".join(og_lines[0]))
    printf("// BEGIN HEADER\n")
    
    for n in ["get", "set"]:
        printf(f"#define table_{n}(TABLE, KEY, VALUE) \\")
        printf("\t_Generic((KEY), \\")
        lines = []
        for kk, kv in key_types.items():
            lines.append(f"\t\t{kk}: _Generic((VALUE), \\\n" + ", \\\n".join([f"\t\t\t{vk if n == 'set' else vk + '*'}: __table_{n}_{kv}_{vv}" for vk, vv in val_types.items()]))
        printf("), \\\n".join(lines) + ") \\")
        printf("\t(TABLE,KEY,VALUE)\n")

    for n in ["has", "del"]:
        printf(f"#define table_{n}(TABLE, KEY) \\")
        printf("\t_Generic((KEY), \\")
        printf(", \\\n".join([f"\t\t{kk}: __table_{n}_{kv}" for kk, kv in key_types.items()]) + ") \\")
        printf("\t(TABLE,KEY)\n")

    for n in ["get", "set"]:
        for kk, kv in key_types.items():
            for vk, vv in val_types.items():
                fn = f"__table_{n}_{kv}_{vv}"
                if not fn in functions:
                    functions.append(fn)
                    printf(f"int {fn}(table_t *table, {kk} key, {vk if n == 'set' else vk + '*'} val);")
        printf("")

    for n in ["has", "del"]:
        for kk, kv in key_types.items():
            fn = f"__table_{n}_{kv}"
            if not fn in functions:
                functions.append(fn)
                printf(f"int __table_{n}_{kv}(table_t *table, {kk} key);")
        printf("")
    
    printf("// END HEADER")
    printf("\n".join(og_lines[2]))
    printf("// BEGIN SOURCE\n")

    def print_key(kk):
        if kk in str_types:
            printf("\tuint64_t k = table->hashfn((void*)key, strlen((const char*)key), table->seed);")
        else:
            printf("\tuint64_t k = (uint64_t)key;")

    for n in ["get", "set"]:
        for kk, kv in key_types.items():
            for vk, vv in val_types.items():
                fn = f"__table_{n}_{kv}_{vv}"
                if not fn in functions:
                    continue
                printf(f"int {fn}(table_t *table, {kk} key, {vk if n == 'set' else vk + '*'} val) " + "{")
                print_key(kk)
                if n == "get":
                    printf(f"\t{vk} tmp_out = {'NULL' if vk in other_types else '0' };")
                    printf("\tint result = 1;")
                    printf("\tif (!unordered_map_has(&table->vmap, k)) {")
                    printf("\t\tresult = 0;")
                    printf("\t\tgoto BAIL;")
                    printf("\t}")
                    printf("\tuint64_t tmp;")
                    printf("\tif (!(result = unordered_map_get(&table->vmap, k, &tmp)))")
                    printf("\t\tgoto BAIL;")
                    printf(f"\ttmp_out = ({vk})tmp;")
                    printf("BAIL:")
                    printf("\tif (val)")
                    printf("\t\t*val = tmp_out;")
                    printf("\treturn result;")
                else:
                    if kk in str_types:
                        printf("\tif (!unordered_map_has(&table->vmap, k)) {")
                        printf("\t\tconst char *dup = strdup((const char*)key);")
                        printf("\t\tunordered_map_set(&table->kmap, k, (uint64_t)dup);")
                        printf("\t}")
                    printf("\treturn unordered_map_set(&table->vmap, k, (uint64_t)val);")
                printf("}\n")
                
    for n in ["has", "del"]:
        for kk, kv in key_types.items():
            fn = f"__table_{n}_{kv}"
            if not fn in functions:
                continue
            printf(f"int {fn}(table_t *table, {kk} key) " + "{")
            print_key(kk)
            if n == "has":
                printf("\treturn unordered_map_has(&table->vmap, k);")
            else:
                printf("\tif (!unordered_map_has(&table->vmap, k))")
                printf("\t\treturn 0;")
                printf("\tif (!unordered_map_del(&table->vmap, k))")
                printf("\t\treturn 0;")
                printf("\tuint64_t tmp;")
                printf("\tif (!unordered_map_get(&table->kmap, k, &tmp))")
                printf("\t\treturn 0;")
                printf("\tfree((void*)tmp);")
                printf("\treturn unordered_map_del(&table->kmap, k);")
            printf("}\n")
    
    printf("// END SOURCE\n")
    printf("\n".join(og_lines[4]))
