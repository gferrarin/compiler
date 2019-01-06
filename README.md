# compiler

A small compiler that transforms code in J into LC4 assembly code in a similar way to how the lcc compiler converts C code into assembly.

This program, when provided with a properly formatted J source file, will produce the corresponding assembly code, i.e.

>> ./jc foo.j

will produce a new file called foo.asm if foo.j contains an acceptable program otherwise it will print out helpful error messages.
