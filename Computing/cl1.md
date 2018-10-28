#  Creating Executable Programs

## Simple Introduction to Compiling and Linking

Here is a simple example to go into my latex notes regarding compiling and linking.

We use three simple `C` programs `main.c`,  `sum.c` and `sum.h`.  We will compile the two
source files seperately so that we can see what the preprocessor, compiler, and linker each
do.  The `main.c` source file 
\clist{} 
```C
#include "sum.h"
int main()
{
    int x = 10;
    int y = 12;
    int z = sum(x,y);
}
```
is run through the preprocessor using the `-E` flag (note that the preprocessor adds some
extra output that helps with compiler warnings etc.  These lines start with a `#` so I will just
remove these lines for simplicity's sake.)
\blist{}
```bash
patricklnoble$ gcc -E main.c | grep -v '#'
int sum(int a, int b);
int main()
{
    int x = 10;
    int y = 12;
    int z = sum(x,y);
}
```
The preprocessor has literally copied the contents of `sum.h` which was included into
`main.c`.  This is the code that the compiler actually sees - it now knows everything it
needs about the call to the `sum` function - its name `sum`, it takes two `int` arguments
and its return type `int`.  This is the *declaration* of the function `sum` - the name, the
number and types of its arguments, and its return types.

To compile (and assemble) we use the `-c` flag, and we get a binary *object* file `main.o`.
This contains the actually machine instruction in binary form that the cpu can execute.  We
will later look at these files in more detail, but for now lets just look at the names inside it.  To
do this we look at the *symbol table* of the program `main.o` using the program `nm`.
\blist{}
```bash
patricklnoble$ nm main.o
0000000000000000 T _main
                 U _sum
```
The `U` before `_sum`  (on mac, symbol names like `sum` get a leading underscore for
some reason) means that the function `sum` is *undefined*.  A function *definition* tells the
compiler what the function actually *does*, as opposed to just its definition (type
information).  The `T` means that `main` is defined in the `TEXT` section of the object file.
Again we will look at these files in more detail later.

The final step on the way to a running executable file is to run the linker.  The `gcc` linker is
called `ld`, but it is almost never a good idea to try to link a program directly using `ld`.
Instead, we again call `gcc` which knows that if the input files are object files (ie `.o` files) it
should use the linker and not the compiler.  Calling the `gcc` linker on our object file `main.o`
gives
\blist{}
```bash
patricklnoble$ gcc main.o -o test
Undefined symbols for architecture x86_64:
"_sum", referenced from:
_main in main.o
ld: symbol(s) not found for architecture x86_64
```
A few things.  Note from the bottom line that we called `gcc main.o etc`, but the error
has come from the linker `ld`.  `gcc` was clever enough to know that since we passed an
object file it should use `ld` to try and link it.  The error gives us information we already
knew from looking at the object file ourself with `nm`: the *name* `_sum` in the object file is
undefined.  Noone know what the function `sum` actually does, so we can't expect to be
able to create a executable program using it!

To fix the situation we need a *definition* for the function `sum`.  It should be consistent with
the *declaration* we provided in `sum.h` (ie that it should take two integer arguments, and
return an integer result).  The definition is in `sum.c`
\clist{}
```C
// sum.c
int sum(int a, int b)
{
    return a + b;
}
```
We compile this into an object file too `gcc -c sum.c` which produces `sum.o`
\blist{}
```bash
patricklnoble$ nm sum.o
0000000000000000 T _sum
```
Inside `sum.o` we see that `sum` is defined (it no longer has the `U` prefix that undefined
names have).  Now it is the linker's job to take the definition of `sum` from `sum.o` and jam
it into `main.o` so that `sum` is no longer undefined there.
\blist{}
```bash
patricklnoble$ gcc sum.o main.o -o test
```
The input files can be in any order, they just have to be object files (with a `.o` suffix).  The
`-o` option allows you to give the outputed executable a particular name - here I chose the
name `test`.  Let's look at the names in `test`:
\blist{}
```bash
patricklnoble$ nm test
0000000100000000 T __mh_execute_header
0000000100000f80 T _main
0000000100000f60 T _sum
U dyld_stub_binder
```
There are a couple of points to be made here.  First and most importantly, note that `sum`
(which on Mac is called `_sum`) is no longer undefined.  The linker took the `main.o` object
file, saw that `_sum` was undefined in it, and found the definition in `sum.o`.  It was able to
combine the two files correctly so everything is defined properly.  Second, there are some
extra bits here.  What are `__mh_execute_header` and `dyld_stub_binder`?  Why is it ok
that `dyld_stub_binder` remains undefined - can we have undefined names in an executable
program?  The answer is that in addition to the steps outlined above there is a lot more
that the compiler, assembler and linker (and loader) need to do to prepare a program to
actually run.  It is possible (common) to use external source code (ie a library) that is only
*linked* with your program when your program is actually run, as opposed to when it is
being compiled and linked as in this chapter.  This type of library is known as a *dynamic*
library.  These will be undefined in the executable file (and so will show up as `U` in the
symbol table) and will not be resolved until run time.  In this case `dyld_stub_binder`
helps with this run time process for certain (unknown to me) low-level system libraries on
mac.  The `__mh_execute_header` helps when the program is actually loaded and fed into
the cpu to run.

## Locating headers and object files

In the above examples `main.c`, `sum.c` and `sum.h` are all in the same directory.  This
makes things easier because we don't have to tell `gcc` where the required files are - it
looks in the current directory by default.  So what if your headers or object files are
somewhere else?  This is very common when using code from a library (or a library of
your own).

In the present directory I create a new directory `dir` and move `sum.c` and `sum.h` here.
Now the code from above will fail.
\blist{}
```bash
main.c:2:10: fatal error: 'sum.h' file not found
#include "sum.h"
^~~~~~~
1 error generated.
```
This failure comes from the preprocessor when trying to load the header `sum.h` - it cannot
find the file.  Adding the `-v` option shows where `gcc` looked for `sum.h` (in addition to the
current directory)
\blist{}
```bash
patricklnoble$ gcc -c main.c -v
Apple LLVM version 9.0.0 (clang-900.0.39.2)
Target: x86_64-apple-darwin17.3.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
#include "..." search starts here:
#include <...> search starts here:
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.0.0/include
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/usr/include
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks (framework directory)
End of search list.
main.c:2:10: fatal error: 'sum.h' file not found
#include "sum.h"
^~~~~~~
1 error generated.
```
Note that I removed some of the non-necessary output.  To add another directory to search
use the `-I` option and say to also search in `dir`.
\blist{}
```bash
#include "..." search starts here:
#include <...> search starts here:
dir
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.0.0/include
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/usr/include
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks (framework directory)
End of search list.
```
So `dir` was added to the search path, and `sum.h` was found.  Now we have the `main.o`
object file.  However the function `sum` is still undefined!  While the preprocessor just needed
the new directory to search, the linker needs two pieces of information - the name of the
file to link to `main.o` and the directory for the linker to search in.  The syntax is as follows.
\blist{}
```bash
patricklnoble$ gcc main.o -lsum.o -Ldir
```
The `-l` should proceed every file that needs to be linked, and `-L` should specify which
directories the linker should search for the required `-l` files.  You can see which directories
the linker searched using the linker `ld` itself.  Leaving out the `-L`  notice in the output
\blist{}
```bash 
patricklnoble$ ld -v main.o -lsum.o
@(#)PROGRAM:ld  PROJECT:ld64-305
configured to support archs: armv6 armv7 armv7s arm64 i386 x86_64 x86_64h armv6m armv7k armv7m armv7em (tvOS)
Library search paths:
/usr/lib
Framework search paths:
/Library/Frameworks/
/System/Library/Frameworks/
ld: library not found for -lsum.o
```
the linker shows its default search locations (and the different architectures this linker
supports - tbd later).  Now add the `-L` and run again,
\blist{}
```bash
Patricks-MacBook-Pro:CompilingAndLinking1 patricklnoble$ ld -v main.o -lsum.o -Ldir
@(#)PROGRAM:ld  PROJECT:ld64-305
configured to support archs: armv6 armv7 armv7s arm64 i386 x86_64 x86_64h armv6m armv7k armv7m armv7em (tvOS)
Library search paths:
dir
/usr/lib
Framework search paths:
/Library/Frameworks/
/System/Library/Frameworks/
```
and see the the new directory `dir` has been added to the library search path.

* Important:
** The **compiler** uses the `-I` arguments to find headers.  Since it already knows the header file
name, only the directories to search are needed.
** The **linker** uses both the `-l` and `-L` arguments.  The linker needs to know both the
name of the file containing object code and the directories to search for.  

When we move on to makefiles and libraries we are just automating the process of compiling
projects in small parts, exactly how we compiled `main.c` and `sum.c` seperately and then
linked them together.  The program `make` helps automate this process when building
complicated projects.

## Compiling C and C++ Programs

Linking C++ programs with source code in C (and other languages) can be a bit involved
because C++ has more complicated naming rules than C because of function/method
overloading (tbd).  Lets do an example.

First, the extensions given to source code matters because `gcc` uses it to infer the language
the code is written in.  The list of file extensions is
[here](https://gcc.gnu.org/onlinedocs/gcc/Invoking-G_002b_002b.html)  Note that `gcc` will
try to compile `.c`, `.` etc files in `C++` but it won't include the standard library properly
and will usually fail with a wall or errors.

* Use `gcc` to compile `C` programs, and use `.c` and `.h` for source files and headers.
* Use `g++` to compile `C++` programs.

A trivial `C++` version of the previous `main.c` is given below in `main.`
\clist{}
```C
// main.c
#include <iostream>
#include "sum.h"
int main()
{
    int x = 10;
    int y = 12;
    int z = sum(x,y);
    std::cout << x << " + " << y << " = " << z << std::endl;
}
```
Try to compile it with `gcc` and watch it crash!  Compiling it as a `C++` program using
`g++ -c main. -Idir` gives a much longer symbol table than our previous `C` programs.
Most of this is standard library code.  Look at what has happened to the `sum` function name:
\blist{}
```bash
patricklnoble$ nm main.o | grep sum
U __Unwind_Resume
U __Z3sumii
```
Whereas the relationship between the name of a function in a `C` source file and the name
given by the compiler in the symbol table is obvious, in `C++` that is no longer true.  This can
make linking together compiled `C++` code (eg `main.o` above) with `C` code (ie `sum.o` we
compiled earlier).    One option would be to recompile `sum.c` using `g++` instead of `gcc`,
so that we use `C++` naming instead of `C`.  Moving into directory `dir` we compile with `C++`
and looking at the symbol table
\blist{}
```bash 
patricklnoble$ g++ -c sum.c
patricklnoble$ nm sum.o
0000000000000000 T __Z3sumii
```
we see that the name given to the `sum` function by the `C++` compiler is the same as what is
expected by the compiler in `main.cc`.  Now the whole process just works
\blist{}
```bash
patricklnoble$ g++ main.o -o main -Ldir -lsum.o
patricklnoble$ ./main
10 + 12 = 22
```
This is a horrible solution though - it means we need to have two different versions of `sum.o`,
one for use in `C++` and one for `C`.  It would be much better to be able to tell the `C++`
compiler that the `sum` function defined in `sum.o` was compiled by a `C` compiler.  To do this
change the declaration of the `sum` function
\blist{}
```bash
patricklnoble$ cat dir/sum.h
extern "C" int sum(int x, int y);
```
where the `extern "C"` tells compilers including `sum.h` that the function `sum` has a `C`
name rather than a `C++` name.  Now compile `main.` again
\blist{}
```bash
patricklnoble$ g++ -c main. -Idir
```
with the new `sum.h` header, and look in the symbol table for the name of the `sum` function:
\blist{}
```bash 
patricklnoble$ nm main.o | grep sum
U __Unwind_Resume
0000000100001ce0 T _sum
```
Now the `C++` compiler expects the `sum` function to be a `C` function, and now the linker
will easily be able to find `_sum` in the object files.
\blist{}
```bash 
patricklnoble$ g++ main.o -Ldir -lsum.o -o main
patricklnoble$ ./main
10 + 12 = 22
```
