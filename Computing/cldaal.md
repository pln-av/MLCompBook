# Compiling and Linking Intel DAAL

The Intel Data Analytics Acceleration Library (DAAL) has all sorts of goodies for us.
Lets try to install it - I assume it will be like installing MKL.  I downloaded it off the
website, and it went into the downloads folders `/Users/patricklnoble/Downloads`.  Actually,
I must have downloaded it the last time too, but probably didnt install.  
\blist{}
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

Wait - are all my errors due to all the `daal` dirs being `root`?  The example failedhere:
\blist{}
```bash
Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu example=cholesky_dense_batch threading=parallel mode=run 
make _make_ex RES_EXT=dylib
mkdir -p _results/gnu_intel64_parallel_dylib
mkdir: _results/gnu_intel64_parallel_dylib: Permission denied
make[1]: *** [_results/gnu_intel64_parallel_dylib/.] Error 1
make: *** [dylibintel64] Error 2
```
Change all of intel dirs.
\blist{}
```
patricklnoble$ sudo chown -R patricklnoble intel
```
Intel instructions were wrong - `cholesky_dense_batch.cpp`, not `cholesky_batch`!  This
works!!
\blist{}
```bash
Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu 
```
Made this file in `_results`!
\blist{}
```bash
patricklnoble$ cat gnu_intel64_parallel_dylib/cholesky_dense_batch.res 

1.000     0.000     0.000     0.000     0.000     
2.000     3.000     0.000     0.000     0.000     
4.000     5.000     6.000     0.000     0.000     
7.000     8.000     9.000     10.000    0.000     
11.000    12.000    13.000    14.000    15.000
```
This actually got run by make:
\blist{}
```bash
Patricks-MacBook-Pro-24560:cpp patricklnoble$ make dylibintel64 compiler=gnu example=cholesky_dense_batch mode=build
make _make_ex RES_EXT=dylib
mkdir -p results/gnu_intel64_parallel_dylib
g++ -Wall -w -Wl,-w -I./source/utils -m64 ./source/cholesky/cholesky_dense_batch.cpp -o _results/gnu_intel64_parallel_dylib/cholesky_dense_batch.exe -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib" -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/../tbb/lib" "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_core.dylib "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_thread.dylib -ldl -ltbb -ltbbmalloc
```
and `rpath` is same as `LD_LIBRARY_PATH`, but it gets search after and it is baked into the binary, rather than leaving a run time dependancy on .  OK, so I just changed the compiler from `g++` to `g++-7` in the makefile - and it works!  So we can compile Intel DAAL with GCC7!  This is the copmile line I used.
To get this I caught the output from the standard intel build using `make`, and searched it.  This will help us build our own versions.
\blist{}
```
g++-7 -Wall -w -Wl,-w -I./source/utils -m64 ./source/cholesky/cholesky_dense_batch.cpp -o _results/gnu_intel64_parallel_dylib/cholesky_dense_batch.exe -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib" -Wl,-rpath,"/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/../tbb/lib" "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_core.dylib "/opt/intel/compilers_and_libraries_2019.1.144/mac/daal/lib"/libdaal_thread.dylib -ldl -ltbb -ltbbmalloc
```