# Scope, Lifetime and Linkage

## Linkage

A common build case is where symbols are shared between different object files.  To see
how to do this, we need a few definitions.  

# Translation Unit

A **translation unit** or **compilation unit** is the basic piece of code that the 
compiler works with.  It consists of a single source file (ie a `.c`,`.cc` etc) file 
after it has been processed by the preprocessor.  It will include all headers, substituted
macros, flags/switches (ie all `#include`,`#ifndef`,`#define` etc).  It is the job of 
the compiler to turn a translation unit into a object file.  After different translation
units have been turned into multiple object files, it is the job of the **linker** to
peice them together.

# Scope

A symbol name always refers to a particular entity - be it a variable, function, class,
namespace etc etc.  In different parts of a program however, the same name can refer to
different entities.  The region of the program where a particular name refers to a particular
entity is referred to as a *scope*.  Scopes are usually delimited by braces `{...}` in 
control statements, functions, classes, namespaces and so on.  All symbols declared outside 
of *any* particular scope are implicitely in the unnamed *global scope*.  If one scope is 
inside another scope (ie is nested), then the symbol in the outside scope is available in 
the inside scope as well.   

# Definition vs Declaration

`C`/`C++` makes a clear distinction between **definitions** and **declarations**.  A 
declaration informs the compiler that a name exists, and it has a certain type.  No memory
for the object referred to is actually allocated at this point.  This means the compiler
does not need to know a lot about the name - ie what it does/how it works.  Below are 
some declarations.
\clist{}
```cpp
int function1(double, int);
void function2(float*, float*);
class TestClass;
extern int;   
```
The last - `extern int;` is interesting.  Variables are often declared and defined at 
the same time.  For instance
\clist{}
```cpp
double function1(double x, double y) { return x + y; }
class TestClass { };
```
Essentially, if there are braces `{}` there is both declaration and definition happening.
Variables confuse things a little things, because a statement which appears as a declaration
for a class is *both* a definition and a declaration for a variable eg
\clist{}
```cpp
class TestClass;  // declaration
int x;            // declaration AND definition
```
Confusing.  It is possible to *only* declare a variable however - it requires the use of 
the `extern` keyword as in the below code
\clist{}
```cpp
extern int x;
```
Functions can also be declared using `extern`, but it is redundant since functions are 
`extern` by default.  The exception is, as we have seen previously, we want to refer to
`C` code defined in a library somewhere.  There we use `extern` (along with a directive
to the compiler that it should not `C++` mangle the name of the function).  But here, it
is the name wrangling the necessitated the `extern` keyword.  Classes are not declared
`extern` - we will see how to deal with them.

A critical rule for compilers and linkers is the *one definition rule*.  You can declare
a symbol as many times as you like.  However, it can only be *defined once*.  The following
code is perfectly legal
\clist{}
```cpp
int func(int,int);   // decl 1
int func(int,int);   // decl 2
int func(int,int);   // decl 3
int func(int x ,int y) { return x + y; }  // def 1
```
however the following is *very* illegal
\clist{}
```cpp
int func(int x, int y) { return x + y; }
int func(int x, int y) [ return x + y; }
```
where the compiler throws the following errors
\blist{}
```bash
patricklnoble$ g++14 test.cc -c
test.cc:4:5: error: redefinition of 'func'
int func(int x, int y) { return x + y; }
    ^
test.cc:3:5: note: previous definition is here
int func(int x, int y) { return x + y; }
    ^
1 error generated.
```

This is pretty obvious - don't define a symbol twice.  However, this can lead to quite
subtle errors when we try to link multiple object files together.  Consider the following
trivial example.  The header `header.h` contains the following function *definition*
\clist{}
```cpp
// header.h
int func(int x, int y) { return x + y; }
```
which is included into two source files which intend to use this function, the first is a 
ridiculous fused multiple add
\clist{}
```cpp
// source1.cc
#include "header.h"
#include "source1.h"
int fmadd(int a, int x, int y)
{
    return func(a*x,y);
}
```
and the second a fused divide add
\clist{}
```cpp
// source2.cc
#include "header.h"
#include "source2.h"
int fdadd(int a, int x, int y)
{
    return func(a/x,y);
}
```
Both compile with no problems
\blist{}
```bash
patricklnoble$ g++ -c source1.cc source2.cc
```
into `source1.o` and `source2.o`.  Are functions are constructed properly
\blist{}
```bash
patricklnoble$ g++ -c source1.cc source2.cc
patricklnoble$ nm -C source1.o 
0000000000000000 T func(int, int)
0000000000000020 T fmadd(int, int, int)
patricklnoble$ nm -C source2.o 
0000000000000000 T func(int, int)
0000000000000020 T fdadd(int, int, int)
```
Now, lets make a program which actually uses these.  We include both `source1.h` and 
`source2.h` to get the *declarations* of `fmadd` and `fdadd`, as is good practice.  This 
also compiles
\blist{}
```bash
patricklnoble$ g++ -c test.cc
```
However, when we try to link together, we get the following errors:
\blist{}
```bash
patricklnoble$ g++ source1.o source2.o test.o
duplicate symbol __Z4funcii in:
    source1.o
    source2.o
ld: 1 duplicate symbol for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation
```
The dreaded *duplicate symbol* error.  How did this happen - we didnt define anything 
twice, did we?  Yes we did!  Both `source1.cc` and `source2.cc` included `header.h`, 
which contained the definition of `func`.  The name `int func(int,int)` is now **defined** 
in two translation units - the one belonging to `source1.cc` and once belonging to 
`source2.cc`.  Each compiled without problem, since they are compiled seperately.  But
the linker choked - which `func(int,int)` should it use?  See in the `nm -C` output - 
`func(int,int)` is defined identically twice!  This is the reason that we should 
generally put all declarations in headers, and definitions in sources.  Very often we
need the *definition* of names in multiple places, but we want to use only one
*declaration*.

# Sharing names between translation units

Sometimes we may want to share data between two translation units.  Here is a contrived
example.  Here is a `main`, where we increment a variable `counter` which is in the global
namespace.  The function `logCounter` logs the current value of `counter`.
\clist{}
```cpp
#include <iostream>
#include "logCounter.h"
int counter = 0;
int main()
{
  for (int i=0; i<5; ++i)
  {
    logCounter();
    ++counter;
  }
}
```
The function `logCounter` function is declared and defined in `logCounter.[cc|h]`
\clist{}
```cpp
patricklnoble$ cat logCounter.cc 
#include "logCounter.h"
void logCounter(void)
{
  std::cout << counter << std::endl;
}
```
However, the compiler cannot compile this - what is `counter`?
\blist{}
```bash
patricklnoble$ g++14 -c logCounter.cc
logCounter.cc:4:16: error: use of undeclared identifier 'counter'; 
```
The solution is to instruct the compiler that `counter` exists - that it will be *defined
somewhere else*.  They way to do this is to *declare* `counter` using `extern`.
\clist{}
```cpp
// logCounter.cc
#include "logCounter.h"
void logCounter(void)
{
  extern int counter;  // declare counter here.  
                       // linker will deal with finding counter later
  std::cout << counter << std::endl;
}
```
Now everything compiles and link properly!
\blist{}
```bash
patricklnoble$ g++ -c logCounter.cc test.cc
patricklnoble$ g++ logCounter.o test.o -o test
patricklnoble$ ./test
0
1
2
3
4
```
The variable `counter` is said to have **external linkage**.  That is its name (ie `counter`
is known across multiple translation units.  Importantly, `counter` in `logCounter.cc` and
`counter` in `main.cc` are referring to the same peice of memory - this is the crux of 
external linkage.  If I print `counter`'s address in both `logCounter.cc` and `test.c`
this is apparent. Remember that this is the work of the linker - the compilation of each 
translation unit happens completely independantly.
\clist{}
```cpp
// logCounter.cc
#include "logCounter.h"
void logCounter(void)
{
  extern int counter;
  std::cout << counter << "," << &counter << std::endl; 
}
// test.cc
#include <iostream>
#include "logCounter.h"
int counter = 0;
int main()
{
  for (int i=0; i<5; ++i)
  {
    std::cout << "&counter = " << &counter << std::endl;
    logCounter();
    ++counter;
  }
}
```
and recompiling/linking
\blist{}
```bash
patricklnoble$ g++ -c logCounter.cc test.cc
patricklnoble$ g++ logCounter.o test.o -o test
patricklnoble$ ./test
&counter = 0x101ffd0e8
0,0x101ffd0e8
&counter = 0x101ffd0e8
1,0x101ffd0e8
... etc
```

This process is quite dangerous - the writer of `test.cc` may not expect that the call
to `logCounter()` will be able to modify their counter variable!  As such, often global
variables are hidden in a `namespace`.  Note that the above procedure works inside a 
general namespace though.  If `counter` is hidden in namespace `ns`, it can still be 
referred to as follows:
\clist{}
```cpp
// test.cc
#include <iostream>
#include "logCounter.h"
namespace ns { int counter = 0; }  // hidden in namespace ns
int main()
{
... 

// logCounter.cc
#include "logCounter.h"
namespace ns { extern int counter; }  // just refer to counter in ns
void logCounter(void)
{
  std::cout << ns::counter << "," << &ns::counter << std::endl; 
}
```
The variable does need to be in global namespace for the linker to be able to resolve
the name.  Returning the code back to its original form, except define `counter` inside
`main` - ie not in global namespace
\clist{}
```cpp
// test..cc
#include <iostream>
#include "logCounter.h"
int main()
{
  int counter = 0;  // counter is NOT in global namespace
  for (int i=0; i<5; ++i)
  {
    logCounter();
    ++counter;
  }
}
```
both files are still perfectly legal to the compiler, but the linker will not be able
to find the variable `counter` referred to in `logCounter.cc`.  This is the dreaded
`undefined symbol` linker error.
\blist{}
```bash
patricklnoble$ g++ -c logCounter.cc test.cc
patricklnoble$ g++ logCounter.o test.o -o test
Undefined symbols for architecture x86_64:
  "_counter", referenced from:
      logCounter() in logCounter.o
```

This all begs the question - what if I don't want a variable in my program - ie like 
`counter`, to be visible in any other translation unit?  This gives me more certainty
that some other peice of code I've never seen (eg `logCounter.cc`) is not doing something
nefarious with my `counter`!.  What is needed is **internal linkage**.  That is, you
want the name `counter` to only be known inside your translation unit, and nowhere else.
This will stop the *linker* from being able to find your variable when it is linking 
together your code and other code written elsewhere.  There are two ways to do this,
using the *static keyword* or *anonymous namespaces*.  The former, using `static` is 
now deprecated in favour of the later.  That said, seeing `static` in code is very 
common so important to know.  To use `static` in this context, define `counter`
\clist{}
```cpp
#include <iostream>
#include "logCounter.h"
static int counter = 0;
int main()
{
  for (int i=0; i<5; ++i)
  {
    logCounter();
    ++counter;
  }
}
```
while leaving `logCounter.cc` unchanged, so that it uses the declaration 
`extern int counter;`.  Both files compile with no problems, but now the linker will
complain that `counter` is unresolved.
\blist{}
```bash
patricklnoble$ g++ -c logCounter.cc test.cc
patricklnoble$ g++ logCounter.o test.o -o test
Undefined symbols for architecture x86_64:
  "_counter", referenced from:
      logCounter() in logCounter.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation
```
This is the effect of giving `counter` *internal linkage*.  It stops the linker from 
being able to *see* `counter` from a different translation unit.  The other option is to 
hide `counter` in a anonymous (unnamed) namespace like below:
\clist{}
```cpp
patricklnoble$ cat test.cc
#include <iostream>
#include "logCounter.h"
namespace { int counter = 0; }
...
```
Again, compiling works but linking fails as above.  In this case, it is very clear *why*
the failure happened by looking at the symbol table:
\blist{}
```bash
patricklnoble$ nm test.o
                 U __Z10logCounterv
00000000000000b0 b __ZN12_GLOBAL__N_17counterE
0000000000000000 T _main
```
Different compilers will handle this different - but see that symbol for `counter` has
`_GLOBAL__N_` appended to the front - this is due to the anonymous namespace, and is
how the compiler stops the linker from finding the symbol.  In `gcc` it is even more
explicit, where the entry looks like
\blist{}
```bash
patricklnoble$ nm test.o
... entries
.... b (anonymous namespace)::counter
```

## Lifetime

The period of the program's execution that a symbol refers to and owns a peice of memory
is the lifetime of the name.  Global variables are are created when the program starts 
running, and are not destroyed until the programs ends.  Variables local to a particular
scope (ie a function, or class etc) have different lifetime rules depending on their 
definition.  

Most variables defined in some local scope have *automatic lifetime* - that
is they are created when program execution hits that scope (ie this function is called etc)
and are destroyed automatically after the scope ends.  There are some exceptions to this
idea - *static* lifetimes, and memory dynamically allocated.  In the latter case, we know
that the symbol referring to the dynamic memory will be destroyed, but the memory it 
pointed to is not deallocated.  In this sense the variable has *automatic* lifetime, 
because the symbol is destroyed, but in another sense it does not, because the memory it
points to still remains.

**Local static objects** are created *before* the first pass through the object's 
definition, and are not destroyed until after the program terminates.  They are persistant.
Note that their *definition* actually only occurs once - even if the definition appears
like it is encountered many times.  For example, this program:
\clist{}
```cpp
#include <iostream>
void func(void)
{
  static int x = 0;  // this only occurs once!
  std::cout << "x = " << x << std::endl;
  x += 1;
}

int main()
{
  for (size_t i=0; i<5; ++i)
  {
      func();
  }
}
```
print the following:
\blist{}
```bash
Patricks-MacBook-Pro-24560:CompilingAndLinking3 patricklnoble$ ./test
x = 0
x = 1
x = 2
x = 3
x = 4
```

Classes can have **static** members and member functions.  These members/member functions 
belong to the class itself, and not any particular instance of the class.  Static methods 
can only refer to static members - since they do not belong to a particular class instance,
they do not get a ```this``` pointer when called.  

There are some subtleties to declaring and defining static class members.  Since a static
data member does not belong to a particular instance of the class, they are not defined 
when we create an instance of the class and are not intialised by the class's constructor.
As such, they need to be *defined outside of the class scope*.  To refer to static members
or member functions, one can use the class namespace and access directly, or they can be 
referred to using an instance of the class.  The following snipped demonstrates these
issues.
\clist{}
```cpp
#include <iostream>
class A
{
public:
  static int data;   // is a declaration only.  a definition here is illegal!
  static int f(int x) { std::cout << x << std::endl;}
};

int A::data = 10;    // definition must be outside of class
int main()
{
    A a;
    std::cout << A::data << std::endl;  // use class scope to refer to data
    std::cout << a.data << std::endl    // use instance to refer to data.
    std::cout << A::f(20) << std::endl; // use class scope to refer to f
    std::cout << a.f(20) << std::endl;  // use instance to refer to f
}
```
Note that in the above, the static member `A::data` was defined *once* when the program
executes, and was never defined again - regardless of how many instances of `A` were 
created during the running of the program.  It is not destroyed until the program itself
terminates.  It does not matter if *any* instances of `A` ever existed in the first place.

Further, objects can themselves be declared **static**.  As opposed to local/automatic
variables, static objects are not destroyed until the end of the entire program.
`GeeksForGeeks` has a great example.  In the following, an instance of the class `A`
is constructed inside the call to the function `func`.
\clist{}
```cpp
class A
{
public:
  A(void) { std::cout << " A constructor " << std::endl; }
  ~A(void) { std::cout << " A destructor " << std::endl; }
};
void func(void)
{
  std::cout << " func called. " << std::endl;
  A a;
  std::cout << " func exited. " << std::endl;
}

int main()
{
  std::cout << " main() called " << std::endl;
  func();
  std::cout << " main() exited " << std::endl;
}
```
In the following output, we see that the object `a` of type `A` is created inside the 
function `func`.  As a local variable inside the scope of the function, it has automatic
storage/lifetime and is destroyed when the function terminates.
\blist{}
```bash
Patricks-MacBook-Pro-24560:CompilingAndLinking3 patricklnoble$ ./test
 main() called 
 func called. 
 A constructor 
 func exited. 
 A destructor 
 main() exited
```
Now, we change the storage class to static, and observe the difference.  Now `func` 
is
\clist{}
```cpp
void func(void)
{
  std::cout << " func called. " << std::endl;
  static A a;
  std::cout << " func exited. " << std::endl;
}
```
and when the program is recompiled and run, we see that `a` is *not* destroyed at the
end of `func`, but instead is destroyed after `main` terminates - ie the end of the 
program!
\blist{}
```bash
patricklnoble$ ./test
 main() called 
 func called. 
 A constructor 
 func exited. 
 main() exited 
 A destructor
```


