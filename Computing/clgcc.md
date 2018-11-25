## Installing Tools on Mac

### GNU Toolchain

What is a *toolchain*?

When you download `XCode`, Apple installs a whole heap of tools for building
`C++` (and other languages).  This is nice, but as with all Apple products, if
you dont use them in exactly the way that Apple foresaw, or you want more fine 
control over what is going on, things can be difficult (in my opinion).

After downloading `XCode`, you might expect that using `g++` at the command line
would be using the `GNU` `C++` compiler - but actually it is using `clang++` - see
the output below
\blist{}
```bash
patricklnoble$ g++ --version
Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
Apple LLVM version 9.1.0 (clang-902.0.39.1)
Target: x86_64-apple-darwin17.3.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```
My favourite thing about `clang` is that it (in my opinion) often gives really helpful
errors messages when you make errors in your `C++`.  However, `GNU` is so ubiquitous in
industry that I think from a practical point of view it is best to get used to working with
`GNU` compilers.  I dont have enough knowledge about performance/features etc - this is just
a statement about which compiler/developer tools seem to me to be used most often.  In addition,
unless you are at the bleeding edge of language features, or really pushing performance hard,
you are unlikely to notice much of a difference between the compilers.  

`GNU` is the default on linux, but not on `mac`.  So as with other tools, we will use `homebrew`
to install it.  I have installed `gcc6`, which is pretty recent - in industry, I wouldnt be suprised
if many companies were not yet using `gcc6`.  In addition, I installed `gcc7` - it has essentially
all `C++17` features - I don't think we will be using them, but if we can be future proof at no
obvious cost, we might as well.  To get both execute
\blist{}
```bash
patricklnoble$ brew install gcc@6
patricklnoble$ brew install gcc@7
```
where I havent shown all the output.  This installed both (as well as some other dependencies
\footnote{see https://formulae.brew.sh/formula/gcc to see what they are}). As 
usual, `brew` installs `symlink`s in `/usr/local/bin` which all point to `brew`'s standard installation
directory `/usr/local/Cellar`.  To use these compilers, we use `gcc-6`,`gcc-7` for the `C` compilers
and `g++-6`,`g++-7` for the `C++` compilers.  This installs most of what we need -  
\blist{}
```bash
Patricks-MacBook-Pro-24560:bin patricklnoble$ ll g*
-r-xr-xr-x  1 patricklnoble  staff  1280564 Oct 26 05:13 gfortran-6*
-r-xr-xr-x  1 patricklnoble  staff   700828 Oct 26 05:13 gcov-tool-6*
-r-xr-xr-x  1 patricklnoble  staff   665944 Oct 26 05:13 gcov-dump-6*
-r-xr-xr-x  1 patricklnoble  staff   740740 Oct 26 05:13 gcov-6*
-r-xr-xr-x  1 patricklnoble  staff    43620 Oct 26 05:13 gcc-ranlib-6*
-r-xr-xr-x  1 patricklnoble  staff    43612 Oct 26 05:13 gcc-nm-6*
-r-xr-xr-x  1 patricklnoble  staff    43588 Oct 26 05:13 gcc-ar-6*
-r-xr-xr-x  1 patricklnoble  staff  1275492 Oct 26 05:13 gcc-6*
-r-xr-xr-x  1 patricklnoble  staff  1279756 Oct 26 05:13 g++-6*
```
however, there are some notables missing - for example, what about `ld` and `as` - the `GNU` 
linker and assembler? I found an entry on the `homebrew` github page from `fxcoudert`, who 
maintains `homebrew` that says

> GNU binutils simply do not support as or ld on macOS.

So I guess when we use this version of `gcc`, we will be using the `GNU` compiler, the `clang` 
linker provided by Darwin, and maybe `GNU` assembler.  I have never had an issue with this
setup in my experience.  
Compile with GNU c++17 features - this is *structured bindings*.
\clist{}
```cpp
patricklnoble$ g++-7 -std=gnu++1z test17.cpp -o test
#include <tuple>
#include <iostream>
std::tuple<char, int, bool> mytuple()
{
  char a = 'a';
  int i = 123;
  bool b = true;
  return std::make_tuple(a, i, b);
}
int main()
{

  auto [a, i, b] = mytuple();
  std::cout << a << std::endl;
  std::cout << i << std::endl;
  std::cout << b << std::endl;
}
```
so the `gnu++1z` is the flag for `C++17` features.  I think `c++14` is deafult
in `gcc7`.  Here is compiling with `std::thread` - i thought it needed 
don't seem to need to link for `std::thread`?.  I think this works:
\clist{}
```cpp
#include <iostream>
#include <thread>
void call_from_thread(int tid)
{
  std::cout << "Hello, World from thread: " << tid << std::endl;
}
static int n = 10;
int main()
{
  std::thread t[n];
  //Launch a group of threads
  for (int i = 0; i < n; ++i) {
    t[i] = std::thread(call_from_thread,i);
  }
  std::cout << " main" << std::endl;
  for (int i=0; i<n; ++i)
    {
      t[i].join();
    }
  return 0;
}
```
using just `g++-7 testt.cc -o test`.  But now I see online that maybe 
compile will work without `-lpthread`, but it is not correct?  Is this true?
Confirm?  I notice that in `gcc-7 -v` it is saying the `PTHREAD` is expected 
threading model 
`\blist{}
```bash
Thread model: posix
```
so maybe everything is being done by default behind the scenes.  I did see this
```blist{}
```bash
/usr/include/pthread
```
so the compiler can most likely find `pthread.h`.  Anyways, lets move on until this
becomes more relevant.

## Installing DAAL

The Intel Data Analytics Acceleration Library (DAAL) has all sorts of goodies for us.
Lets try to install it - I assume it will be like installing MKL.  I downloaded it off the
website, and it went into the downloads folders `/Users/patricklnoble/Downloads`.  Actually,
I must have downloaded it the last time too, but probably didnt install.  
`blist{}
```bash
Patricks-MacBook-Pro-24560:Downloads patricklnoble$ ll *daal*
-rw-r--r--@ 1 patricklnoble  staff  505818831 Mar 16  2018 m_daal_2018.1.126.dmg
-rw-r--r--@ 1 patricklnoble  staff  349861058 Nov 17 10:52 m_daal_2019.1.144.dmg
```
To install it lets use the usual Mac way of installing `.dmg` files.  Click on it in finder,
then open the `daal` icon.  Confirm you want to open it.  Install as `root`, we will deal with
this later.  You will need to enter your password.  It wants to install into `opt/intel`, which
is where `mkl` and `tbb` is - don't try to customise - just allow the installer to use this 
location.  Click install - the installation only took a few seconds.  The 2019 `daal` seems to
have installed with 2019 suffixes everywhere, whereas MKL is using 2018.  I guess this is due
to the different versions I got.  Lets see how this resolves itself.  

Clicking `finish` brings up an `intel` *get_started* page, which actually is an `html` file 
\blist{}
```bash
/opt/intel/documentation_2019/en/daal/ps2019/get_started.htm
```
It confirms the installation went to `/opt/intel/compilers_and_libraries_2019.1.144/mac`.
`intel` refers to this directory as `<instal dir>=/opt/intel/compilers_and_libraries_2019.1.144/mac`.
 I notice here there is another copy of `tbb` - I suspect `daal` is dependant on `tbb`, so ships
 with a version it can use (without requiring you to have installed an independant copy - which
 we already did).  I see later you can compile with option `-daal=parallel` or `-daal=sequential`.

The script `opt/intel/compilers_and_libraries_2019.1.144/mac/daal/bin/daalvars.sh` will help
setup up our environment to use `daal`.  Most notably, it sets

* `LIBRARY_PATH` - used by the linker to resolve libraries needed at link time
* `LD_LIBRARY_PATH` used by the loader to find *dynamic* libraries required at run time.
* `CPATH` - where the compiler should look for headers.  Essentially everything that would
be passed to the compiler with `-I` flag can be put here.  Note that `-I` takes precedence over
`CPATH`, otherwise, they are identical.
* `DAALROOT` - a variable used by `daal` itself, as the root for where you installed all the
code.

You need to tell `daal` to use 64 bit versions.  So run the script like this:
\blist{}
```bash
Patricks-MacBook-Pro-24560:bin patricklnoble$ source daalvars.sh intel64
Patricks-MacBook-Pro-24560:bin patricklnoble$ echo $DAALROOT
/opt/intel/compilers_and_libraries_2019.1.144/mac/daal
```
where the last line confirms that it has worked properly - the `daal` libraries know where they
were installed!  Lets try run the sample program.  I saved it here
\blist{}
```bash
/Users/patricklnoble/Documents/Projects/Repos/Book/Computing/daal_test/my_first_daal_program.cpp
```
The header paths are already in my `CPATH`, as well as the library paths `LD_LIBRARY_PATH` and
`LIBRARY_PATH`.  Can I compile it?  It is using the `intel` compiler, which I don't have.  Instead,
I'll try with `gcc`.
\blist{}
```bash
patricklnoble$ gcc-7 my_first_daal_program.cpp -daal=parallel -o test_daal
```
Disaster!  `gcc` doesn't understand the `-daal` option, and it has a mountain of *undefined symbols*
ie the error ```Undefined symbols for architecture x86_64:```.  This means our linking has failed.
Lets first see if it compiles.
\clist{}
```
patricklnoble$ gcc-7 my_first_daal_program.cpp -c
```
Works!  So it must just be the link line that is wrong.  In the *Build Your Application
With Different Compilers* section, I see it suggests we need `libdaal_core.dylib` and `libdaal_sequential.dylib`, which are in `<instal-dir>/daal/lib`.  I can definitely 
see them in see them in `/opt/intel/compilers_and_libraries_2019/mac/daal/lib`.  Can I manually do this?  Still failing!  The 
linker is battling with what looks like standard library code?  How strange? Lets go try the examples - see if that gets us 
somewhere more productive.  For examples, go here `/opt/intel/compilers_and_libraries_2019/mac/daal/examples/cpp`, and follow
*Step 4* in the instructions.  Examples are in `daal.lst`, and I'll choose `cholesky_dense_batch`. 

Wait - are all my errors due to all the `daal` dirs being `root`?  The example failed
here:
Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu example=cholesky_dense_batch threading=parallel mode=run 
make _make_ex RES_EXT=dylib
mkdir -p _results/gnu_intel64_parallel_dylib
mkdir: _results/gnu_intel64_parallel_dylib: Permission denied
make[1]: *** [_results/gnu_intel64_parallel_dylib/.] Error 1
make: *** [dylibintel64] Error 2

Change all of intel dirs.

patricklnoble$ sudo chown -R patricklnoble intel

Intel instructions were wrong - cholesky_dense_batch.cpp, not cholesky_batch!  This
works!!

Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu 

Made this file in `_results`!

patricklnoble$ cat gnu_intel64_parallel_dylib/cholesky_dense_batch.res 

1.000     0.000     0.000     0.000     0.000     
2.000     3.000     0.000     0.000     0.000     
4.000     5.000     6.000     0.000     0.000     
7.000     8.000     9.000     10.000    0.000     
11.000    12.000    13.000    14.000    15.000

This actually got run by make:

Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu example=cholesky_dense_batch mode=build
make _make_ex RES_EXT=dylib
mkdir -p _results/gnu_intel64_parallel_dylib
g++ -Wall -w -Wl,-w -I./source/utils -m64 ./source/cholesky/cholesky_dense_batch.cpp -o _results/gnu_intel64_parallel_dylib/cholesky_dense_batch.exe -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib" -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/../tbb/lib" "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_core.dylib "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_thread.dylib -ldl -ltbb -ltbbmalloc

rpath is same as LD_LIBRARY_PATH, but it gets search after.  OK, so I just changed the
compiler from `g++` to `g++-7` in the makefile - and it works!  So we can compile 
Intel DAAL with GCC7!  This is the copmile line.

g++-7 -Wall -w -Wl,-w -I./source/utils -m64 ./source/cholesky/cholesky_dense_batch.cpp -o _results/gnu_intel64_parallel_dylib/cholesky_dense_batch.exe -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib" -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/../tbb/lib" "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_core.dylib "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_thread.dylib -ldl -ltbb -ltbbmalloc