## aisvector.so

### General
Both vectors are essentially dynamically-sized arrays that expand and shrink as needed. All vectors are managed
using their appropriate functions. So char vectors are created using `char_vector_init` while generic vectors are
created using `vector_init`. The objects created are struct pointers, with the struct definition looking like this:
```c
struct vector_st {
    (type) *array;
    size_t length;
    size_t capacity;
};
```

Each has the following basic methods:
`vector_init`: Creates a vector. <br>
`vector_init_c`: Creates a vector with the given initial capacity. <br>
`vector_destory`: Frees the memory associated with the vector.\* <br>
`vector_clear`: Removes all items in the vector.\* <br>
`vector_append`: Adds a new item to the end of the vector. <br>
`vector_prepend`: Adds a new item to the beginning of the vector. <br>
`vector_expand`: Increases the size of the vector by `increase`, filling in new items with `item`.
`vector_shrink`: Decreases the size of the vector by `decrease`.
`vector_get`: Retrieves an item from the vector, setting `success` to `false` if the specified `index`
is out of range. <br>
`vector_set`: Assigns an element in the vector, returning `false` if the specified `index` is out of range. <br>
`vector_remove`: Removes an item from the vector, automatically shifting elements to fill the hole. <br>
`vector_resize`: Modifies the capacity of the vector.

\* The user must manually free the items contained withing the vector if created using `malloc`. <br>

### char\_vector
This is a vector that stores single bytes, or `char`s. In addition to the above methods, `char_vector.h`
also provides two more methods: `char_vector_to_buffer` produces a `malloc`-ed buffer with a copy of the
contents of the vector, and `char_vector_to_string` does the same, but with a terminating null byte at the
end.

### vector
Unlike char\_vector, `vector.h` uses `void *` as the item type, allowing generic casting to other pointer types.
This would allow things like a vector of strings, or a vector of vectors.

