### c-utils
Some utilities and libraries written in C.

### Useful
#### getchar
Read a character from the input and identify it.

#### rmln
Safely removes non-original files. It will only delete symbolic links or files with multiple hard-links. Passing
options can restrict which kinds of links it removes. This program will never delete directories or files that exist nowhere else.

### Not that useful
#### pid-loop
A small, simple program that reads a pid from `stdin` and stays alive for as long as the process it's tracking
is alive.

#### hashmap
(Object: `aishashmap.so`)
This library contains my `int_hashmap` and `str_hashmap` objects. These are dynamic maps that accept either
integers (specifically a `long`) or a string as a key to provide access to an object. See the README in that
directory for more information.

#### vector
(Object: `aisvector.so`)
This library contains my `vector` and `char_vector` objects. These are dynamically-sized arrays that allow for
a variety of dynamic operations like insertion. `vector` has an item type of `void *` allowing casting to any
other type, and, as the name implies, `char_vector` is a vector of `char`.

