## aishashmap.so

### General
Hash maps are key-value pairs that are managed and kept unique through the usage of a hash function. Both maps are managed using their appropriate functions. So integer hashmaps are created using `int_hashmap_init`
while string hashmaps are created using `str_hashmap_init`. The objects created are struct pointers, with the
struct definition looking like this:
```c
struct hashmap_st {
    size_t size;
    size_t capacity;
    struct hashentry_st *items;
};
```

Each has the following basic methods:
`hashmap_init`: Creates a hashmap. <br>
`hashmap_init_c`: Creates a hashmap with the given capacity. <br>
`hashmap_destroy`: Destroys a hashmap, freeing all components.\* <br>
`hashmap_clear`: Removes all items from the hashmap.\* <br>
`hashmap_put`: Assigns the given value to the given key, creating an entry if key
did not previously exist in the map.<br>
`hashmap_remove`: Removes the entry associated with the given key. <br>
`hashmap_get`: Retrieves the item associated with the given key, placing the value at the
pointer passed by the user.<br>
`hashmap_list_keys`: Produces an array of size `map->size` that contains each key in the map. \*\*<br>
`hashmap_list_values`: Produces an array of size `map->size` that contains each values for each
key in the map. \*\*<br>
`hashmap_resize`: Changes the capacity of the hashmap.

\* The user must manually free the items contained withing the map if created using `malloc`. <br>
\*\* The user must manually free the result of this function. <br>

### int\_hashmap
This map uses a `long` as a key, and `const void *` as a value, allowing generic casting to
any other type you may want. Negative keys are allowed, as are NULL values.

### str\_hashmap
This map uses a `const char *` as a key, and `long` as a value. This does not allow for differing
types like `int_hashmap` provides, but does properly handle nullable strings as keys. Strings that
are different (e.g. created from `strdup`) but have the same character values are treated as equal.
The map uses the djb2 string hash algorithm, which is provided by the library as an accessible function.

