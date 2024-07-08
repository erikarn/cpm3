This will build the binary pieces from the J15CPM3.EMT file.

The program - build.sh - uses the contents in ../orig/ -
the J15CPM3.EMT file itself and the memory map text file region -
to extract each of the components.

Then, the reverse-loaded binaries are un-reversed to make it
easier to disassemble, compare, swap and such.


