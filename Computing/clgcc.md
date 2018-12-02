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

