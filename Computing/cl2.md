
# Introduction to Libraries and GNU make

The previous section has shown that properly compiling and linking a program can be a
lot of work.  It is also very error prone - if a change was made in one of the `.c` source
code files, did we remember to compile it into a `.o` object file and link everything again?  In
a large project with many thousands (or more) source files this is quite a task.  `C`/`C++` deal
with this in two ways - **libraries** and **GNU make**.

* **Libraries** collect groups of object files together into a single package, so that instead of
having to explicitely link every source file needed we can just link against the package
containing the object code we need.  There are two types of libraries - **static** and
**dynamic** - we will discuss both in the next section.
* **GNU make** is a program that is included as part of the **GNU toolchain** that automates
the whole compiling and linking process (including creating and using libraries).  For very small
projects `make` can be overkill, but as the size of projects grow it can be indespensible.

## Libraries

A library is a collection of object files.  Using libraries is conveniant because it saves us from
having to link against every single object file needed by our code.  There are also memory
savings for dynamic libraries (to be discussed).  We will start with static libraries because
our use of `sum.o` in the previous section was essentially identical to linking `main.c` with a
static library containing one object file `sum.o`.  We will then look at dynamic linking, and see
why dynamic linking is the default library type for `gcc`/`g++`.

These examples are in a new directory `CompilingAndLinking2` which itself contains two
directories `include` and `libs`.  This is not an optimal/standard project organisation but it
will illustrate the issues we need to dealt with.  Our headers will go in `include` and source
code and object files will go in `libs`.  If we were 'selling' our software we would provide
users with the header files and compiled libraries.  We *do not* provide users with the `.c`
source code - this remains our intellectual property!

### Static Libraries

A static library is a collection of object files that get completely linked into our executable
program at the time the program is itself created.  This means that all the object code is
baked into the executable program, and we can run the program at anytime without ever
needing any of the static library code again.  This is conveniant and portable, but it can make
the program binary file large in size.

To make the project structure more interesting, we will define addition and subtraction
functions taking two and three arguments respectively.  The project directory structure will
look as follows:
\blist{}
```console
 - CompilingAndLinking2/
    - main
    - include/
        - binary/
            - sum.h
            - subtract.h
        - ternary/
            - sum.h
            - subtract.h
    - libs/
        - binary_src/
            - sum.c
            - subtract.c
        - ternary_src/
            - sum.c
            - subtract.c
        - libbinary.a
        - libternary.a
```
We will provide the user of our code the ability to add and subtract two numbers (binary
operations) and to add and subtract three numbers (ternary operations).  For the user to
incorporate our addition/subtraction library into their code, they need

 * **Headers** for them to compile their code.
 * **Libraires** which contain the actual implementation of our addition/subtraction code.

We don't want the users to know anything about *how* we do our addition/subtraction (that
would give away our IP, and mean that the user no longer needs to buy our software!), which
is why we would *not* ship the two directories `binary_src` and `ternary_src`.  

First we compile our four source files.  For example, the ternary sum looks like this
\clist{}
```cpp
// libs/ternary_src/sum.c
#include "ternary/sum.h"
int sum(int x, int y, int z)
{
    return x + y + z;
}
```
Here we assume that the compiler will have the project directory
`CompilingAndLinking2/include` added to the header search path `-I`, so that when
looking for the header referred to above the compiler will open the directory
`CompilingAndLinking2/include`, see the directory `ternary`, and then inside that find
`sum.h`.  Since we will use the directory `CompilingAndLinking2` regularly, lets define
\blist{}
```bash
patricklnoble$ export CL2=/Users/patricklnoble/Documents/Projects/CppProjects/Viswanath/CompilingAndLinking2
patricklnoble$ echo $CL2
/Users/patricklnoble/Documents/Projects/CppProjects/Viswanath/CompilingAndLinking2
```
so that the compiler/linker instructions are a little shorter.  Some definitions like this  will be
provided to library users so that they can put the library code wherever they like and our
compiler instructions will still be correct.
\blist{}
```bash
patricklnoble$ g++ -c sum.c -o sum.o -I${CL2}/include
```
Do the same for `subtract.c`.  This completes the object code for the first of our two
libraries - the library for binary addition and subtraction.  To package up the library we use
the GNU archive program `ar`.  `ar` is so named because the collection of object files
(here `subtract.o` and `sum.o` are).  Now create the archive with `ar`
\clist{}
```bash
patricklnoble$ ar -rv libbinary.a sum.o subtract.o
ar: creating archive libbinary.a
a - sum.o
a - subtract.o
```
The `-r` flag adds an object file to an archive, and if the archive doesn't exist it creates a
new one (and `-v` is verbose).  The static library naming convention is is always `libXXX.a`
where `XXX` is the desired name of the library.  The line above is creating functions for binary
operations so the `XXX` is `binary`.  The contents of the archive are interesting:
\blist{}
```cbash
patricklnoble$ nm libbinary.a
libbinary.a(sum.o):
0000000000000000 T __Z3sumii

libbinary.a(subtract.o):
0000000000000000 T __Z8subtractii
```
Move into the the `ternary_src` directory and create a similar archive
\blist{}
```bash
patricklnoble$ ar -rv libternary.a subtract.o sum.o
ar: creating archive libternary.a
a - subtract.o
a - sum.o
```
Now we have the following directory structure and libraries
\blist{}
```bash
patricklnoble$ ll CompilingAndLinking2/libs/*/*.a
  1520 Mar 30 20:08 CompilingAndLinking2/libs/binary_src/libbinary.a
  1536 Mar 30 20:14 CompilingAndLinking2/libs/ternary_src/libternary.a
```
Now inside `$CL2/main` write a `main.` program that uses the libraries.
\clist{}
```cpp
Patricks-MacBook-Pro:main patricklnoble$ cat main.c
#include <iostream>
#include <binary/sum.h>
#include <ternary/sum.h>

int main()
{
int x = 12;
int y = 9;
int z = 21;

int binarySum  = sum(x,y);
int ternarySum = sum(x,y,z);
std::cout << "sum(" << x << "," << y << ") = " << binarySum << std::endl;
std::cout << "sum(" << x << "," << y << "," << z << ") = " << ternarySum << std::endl;
}
```
To compile and link it, first compile
\blist{}
```bash
patricklnoble$ g++ -c main.c -I${CL2}/include
```
Remember, the compiler only needs the headers.  We now have to link `main.o` with our two
libraries to create the executable program.  Now link as follows
\blist{}
```bash
Patricks-MacBook-Pro:main patricklnoble$ g++ main.o -o main -L${CL2}/libs/binary_src -lbinary -L${CL2}/libs/ternary_src -lternary
```
and look in the symbol table
\blist{}
```bash
patricklnoble$ nm main | grep sum
U __Unwind_Resume
0000000100001cb0 T __Z3sumii
0000000100001cd0 T __Z3sumiii
```
Note that the two `sum` functions are resolved inside the executable `main`.  This is how a
static library works.  The object code inside our two libraries has been linking *into* the
executable `main`.  The code in the libraries is never needed again.  Running the program
gives
\blist{}
```bash
Patricks-MacBook-Pro:main patricklnoble$ ./main
sum(12,9) = 21
sum(12,9,21) = 42
```
And we are done!

### Dynamic Libraries

A dynamic library is a collection of object code in the same way as a shared library.  It diff

Compile the source code for binary and ternary functions into shareable objects
\blist{}
```bash
patricklnoble$ g++ -shared -I${CL2}/include -fPIC sum.c subtract.o -o libternary.so
patricklnoble$ g++ -shared -I${CL2}/include -fPIC sum.c subtract.o -o libbinary.so
```
Look inside one of the new shared libraries
\blist{}
```bash
Patricks-MacBook-Pro:ternary_src patricklnoble$ nm libternary.so
0000000000000f70 T __Z3sumiii
0000000000000f90 T __Z8subtractiii
U dyld_stub_binder
```
Now `main.c` is compiled as before
\blist{}
```bash
patricklnoble$ g++ main.c -c -I${CL2}/include
```
Note the `sum` functions are still undefined
\blist{}
```bash
Patricks-MacBook-Pro:main patricklnoble$ nm main.o | grep sum
U __Unwind_Resume
U __Z3sumii
U __Z3sumiii
```
Link `main.o` against the dynamic/shared libraries
\blist{}
```bash
patricklnoble$ g++ main.o -o main -L${CL2}/libs/binary_src -L${CL2}/libs/ternary_src -lbinary -lternary
```
Note that the `sum` functions are still undefined in the executable
\blist{}
```bash
Patricks-MacBook-Pro:main patricklnoble$ nm main | grep sum
U __Unwind_Resume
U __Z3sumii
U __Z3sumii
```
Alas, when we try to run `main` we crash at run time
\blist{}
```bash
patricklnoble$ ./main
dyld: Library not loaded: libbinary.so
Referenced from: /Users/patricklnoble/Documents/Projects/CppProjects/Viswanath/CompilingAndLinking2/main/./main
Reason: image not found
Abort trap: 6
```
On Mac we use the program `otool` to see what dynamic libraries an executable expects
\blist{}
```bash
patricklnoble$ otool -L main
main:
libbinary.so (compatibility version 0.0.0, current version 0.0.0)
libternary.so (compatibility version 0.0.0, current version 0.0.0)
/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 400.9.0)
/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.0.0
```
On linux this is done by `ldd`.  We see both the `binary` and `ternary` libraries are expected.
The error happens because the dynamic linker is unable to find the `binary` library (same goes
for `ternary`).  To instruct the dynamic linker where the library is we need to define the
variable `DYLD_LIBRARY_PATH`
\blist{}
```bash
patricklnoble$ export DYLD_LIBRARY_PATH=${CL2}/libs/binary_src:${CL2}/libs/ternary_src
```
After doing this `main` runs as before.
\blist{}
```bash
Patricks-MacBook-Pro:main patricklnoble$ ./main
sum(12,9) = 21
sum(12,9,21) = 4
```

## make

`make` is a GNU program that automates the process of building and compiling libraries and
programs.  The strength of `make` is in capturing the dependencies that exists between a
library full of headers and source files, and determining whether files need to be recompiled
or not.

A `makefile` consists of a set of rules made up of targets, prerequisites and commands as
follows
\blist
```bash
target: [prereq1 prereq2 ...]
    [command1 command2 command3 ...]
```
A rule is evaluated in the following way:
 * If a target is a file, `make` checks if the target file is more current than all of the prerequisits.
 If it is more current than all prerequisits, stop before running any commands.
 * If a prerequisit is older than a target `make` should search for a rule where the the prerequisit
 **is** the target.  Then `make` should run all the commands in the rule.
