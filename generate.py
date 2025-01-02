#!/usr/bin/env python3

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

for n in ["get", "set"]:
    print(f"#define table_{n}(TABLE, KEY, VAL) \\")
    print("\t_Generic((KEY), \\")
    lines = []
    for kk, kv in key_types.items():
        lines.append(f"\t\t{kk}: _Generic((VAL), \\\n" + ", \\\n".join([f"\t\t\t{vk}: __table_{n}_{kv}_{vv}" for vk, vv in val_types.items()]))
    print("), \\\n".join(lines) + ") \\")
    print("\t)(KEY,VALUE)\n")

for n in ["has", "del"]:
    print(f"#define table_{n}(TABLE, KEY) \\")
    print("\t_Generic((KEY), \\")
    print(", \\\n".join([f"\t\t{kk}: __table_{n}_{kv}" for kk, kv in key_types.items()]) + ") \\")
    print("\t)(KEY)\n")

for n in ["get", "set"]:
    for kk, kv in key_types.items():
        for vk, vv in val_types.items():
            fn = f"__table_{n}_{kv}_{vv}"
            if not fn in functions:
                functions.append(fn)
                print(f"int {fn}(table_t *table, {kk} key, {vk if n == 'set' else vk + '*'} val);")
    print()

for n in ["has", "del"]:
    for kk, kv in key_types.items():
        fn = f"__table_{n}_{kv}"
        if not fn in functions:
            functions.append(fn)
            print(f"int __table_{n}_{kv}(table_t *table, {kk} key);")
    print()

def print_key(kk):
    if kk in str_types:
        print("\tuint64_t k = table->hashfn((void*)key, strlen((const char*)key), table->seed);")
    else:
        print("\tuint64_t k = (uint64_t)key;")

for n in ["get", "set"]:
    for kk, kv in key_types.items():
        for vk, vv in val_types.items():
            fn = f"__table_{n}_{kv}_{vv}"
            if not fn in functions:
                continue
            print(f"int {fn}(table_t *table, {kk} key, {vk if n == 'set' else vk + '*'} val) " + "{")
            print_key(kk)
            if n == "get":
                print(f"\t{vk} tmp_out = {'NULL' if vk in other_types else '0' };");
                print("\tint result = 1;")
                print("\tif (!int_map_has(&table->vmap, k)) {")
                print("\t\tresult = 0;")
                print("\t\tgoto BAIL;")
                print("\t}")
                print("\tuint64_t tmp;")
                print("\tint_map_get(&table->vmap, k, &tmp);");
                print(f"\ttmp_out = ({vk})tmp;")
                print("BAIL:")
                print("\tif (val)")
                print("\t\t*val = tmp_out;")
                print("\treturn result;")
            else:
                if kk in str_types:
                    print("\tif (!int_map_has(&table->vmap, k)) {")
                    print("\t\tconst char *dup = strdup((const char*)key);")
                    print("\t\tint_map_set(&table->kmap, k, (uint64_t)dup);")
                    print("\t}")
                print("\treturn int_map_set(&table->vmap, k, (uint64_t)val);")
            print("}\n")
            
for n in ["has", "del"]:
    for kk, kv in key_types.items():
        fn = f"__table_{n}_{kv}"
        if not fn in functions:
            continue
        print(f"int {fn}(table_t *table, {kk} key) " + "{")
        print_key(kk)
        if n == "has":
            print("\treturn int_map_has(&table->vmap, k);")
        else:
            print("\tif (!int_map_has(&table->vmap, k))")
            print("\t\treturn 0;")
            print("\tif (!int_map_del(&table->vmap, k))")
            print("\t\treturn 0;")
            print("\tuint64_t tmp;")
            print("\tif (!int_map_get(&table->kmap, k, &tmp))")
            print("\t\treturn 0;")
            print("\tfree((void*)tmp);")
            print("\treturn int_map_del(&table->kmap, k);")
        print("}\n")
