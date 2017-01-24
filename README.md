## c-utils
Some small utilities and libraries written in C. They're mostly CLI utilities, and they're likely not too useful. If you actually use one of these, please tell me.

All files are available under the MIT license unless otherwise specified. Check for a `LICENSE` file in a directory to see if it has an overriding license.

### Single file programs
* `getchar` - Read a character from stdin and identify it.
* `getch` - `getchar`, but in curses. Accepts weird keys like page down.
* `pid-loop` - Simple program that reads a pid from stdin and stays alive as long as the process it's tracking is alive. Not very fancy.
* `xfiller` - An X11 program that opens a window. That's it. Useful for padding on tiling WMs.

### Directories
**`rmln`** (in directory `remove_links/`)

A "safer" `rm`, since it only deletes links. You can have it delete only symbolic or hard links, though by default it will remove both. Includes a man page at no extra charge, but only if you order now.

**`libais`**

A "standard library" or something. Not super useful right now. The eventual idea is to centralize a bunch of useful things that I use in one place.

### Compilation and Running
Generally I write ANSI C, so you should have no problem finding a compatible compiler. Other than that, dependencies vary on the program. For example, `xfiller` obviously depends on Xlib.

Be sure you invoke `make` in each directory manually, since the top level Makefile only compiles sources directly in that directory.

