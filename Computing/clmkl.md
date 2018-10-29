
# Compiling and Linking MKL

### BLAS and LAPACK

BLAS (Basic Linear Algebra Subroutines) and LAPACK (Linear Algebra Package) are names
given to numerical linear algebra packages.  In these notes we use two different
implementations:

* **Eigen C++** : Eigen is very expressive high performance open source library for linear
algebra.
* **Intel MKL**: the Intel MKL is a super high performance library optimised for Intel
hardware.  It is much less expressive and more difficult to work with the Eigen, but cannot be
beat in terms of raw performance.

BLAS is usually described as having three parts
* **Level 1**: vector operations
* **Level 2**: matrix - vector operations
* **Level 3**: matrix - matrix operations

These notes continue those in *CompilingAndLinking1* and look at how to install, compile
and link programs using the Intel MKL.
### MKL directories

In the documentation it refers to `<Composer XD directory>` and `<mkl directory>`
directories.  I think these are
\blist{}
```bash
/opt/intel
/opt/intel/mkl
```
on my system  So we have the relevant installs.  Now we need to set a number of
environment variables which is done using scripts in
blist{}
```bash
/opt/intel/mkl/bin
```
To use the `IA-64` architecture we source the following file
\blist{}
```bash
patricklnoble$ source /opt/intel/mkl/bin/mklvars.sh intel64
```
which appears to set a number of import environment variables like `MKLROOT` and
`DYLD_LIBRARY_PATH`.  Note that the docs suggest that directories
\blist{}
```bash
/opt/intel/mkl/bash/[intel32/intel64]
```
exist but I can't seem to see them.  The download also clearly knows we are on a
mac - note where `MKLROOT` points
\blist{}
```bash
Patricks-MacBook-Pro:examples patricklnoble$ echo $MKLROOT
/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl
```
It looks like the headers required are all in `$MKLROOT/include`, and we can see them
all being included by the header `$MKLROOT/include/mkl.h`
\clist{}
```cpp
patricklnoble$ grep '#include' $MKLROOT/include/mkl.h | head -n 5
#include "mkl_version.h"
#include "mkl_types.h"
#include "mkl_blas.h"
#include "mkl_trans.h"
#include "mkl_cblas.h"
```
The libraries themselves are in `$MKLROOT/bin`.

Intel installed everything as roolt.  This means you can't uncompress the examples.  Go
to `/` and change the owner of all the directories to `patricklnoble`
\blist{}
```bash
sudo chown -R opt patricklnoble
```
Decompress all the `C` examples in `$MKLROOT/examples`
\blist{}
```bash
tar -xzf examples_core_c.tgz
```
I went to the `solverc` examples in `$MKLROOT/examples/solverc` and use the `makefile`
there to build the static 64-bit examples
\blist{}
```bash
make libintel64 compiler=gnu
```
Have a look at the ouput below - this might help us compile and link our own versions.
\blist{}
```bash
----- Compiling gnu_lp64_omp_intel64_lib ----- cg_mrhs_stop_crt_c
gcc -m64  -w -I"/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/include" \
./source/cg_mrhs_stop_crt_c.c \
"/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/lib/libmkl_intel_lp64.a" \
"/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/lib/libmkl_intel_thread.a" \
"/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/lib/libmkl_core.a" \
-Wl,-rpath,/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/../compiler/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/../tbb/lib \
-L"/opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/../compiler/lib" -liomp5 -lpthread -lm -o _results/gnu_lp64_omp_intel64_lib/cg_mrhs_stop_crt_c.out
----- Execution gnu_lp64_omp_intel64_lib ----- cg_mrhs_stop_crt_c
_results/gnu_lp64_omp_intel64_lib/cg_mrhs_stop_crt_c.out > _results/gnu_lp64_omp_intel64_lib/cg_mrhs_stop_crt_c.res
```
This wrote all the executables and output files here
\blist{}
```bash
/opt/intel/mkl/examples/solverc/_results/gnu_lp64_omp_intel64_lib
```
Looking in the `*.res` files, it looks like we were able to compile and run these examples!
For example, looking inside `dss_sym_c.res`
\blist{}
```bash
patricklnoble$ cat /opt/intel/mkl/examples/solverc/_results/gnu_lp64_omp_intel64_lib/dss_sym_c.res
determinant power is 0
determinant base is 2.25
Determinant is 2.25
Solution array:  -326.333 983 163.417 398 61.
```
it isnt obvious what the example was but it certainly did something.

Lets try to use a routine ourselves.  A  really simple call is `cblas_dscalx` which implements
vector-scalar multplication.  An intel example using it is here
`/opt/intel/mkl/examples/cblas/source/cblas_dscalx.c`.  I have written a simple
`main.cpp` to call it.
\clist{}
```cpp
#include <iostream>
#include "mkl.h"
int main()
{
    std::cout << "Compile MKL" << std::endl;
    MKL_INT n = 10;           // size of vector
    MKL_INT incx = 1;         // increment
    double a = 3.70;          // scalar
    double* x = (double *)mkl_calloc(10, sizeof(double),64);
    
    //  write a test array
    for (int i=0; i<n; i++)
    {
        x[i] = i;
        std::cout << "x["<< i << "] = " << x[i] << std::endl;
    }
    // call mkl cblas_dscal
    cblas_dscal(n, a, x, incx);
    
    // print output
    for (int i=0; i<n; i++)
    {
        std::cout << "x[" << i << "] = " << x[i] << std::endl;
    }
}
```
I compile it like this
\blist{}
```bash
g++ main.cpp -I$MKLROOT/include ${MKLROOT}/lib/libmkl_intel_ilp64.a ${MKLROOT}/lib/libmkl_sequential.a ${MKLROOT}/lib/libmkl_core.a -lpthread -lm -ldl -o main
```
which produces the correct result!
\blist{}
```bash
patricklnoble$ ./main
Compile MKL
x[0] = 0
x[1] = 1
x[2] = 2
x[3] = 3
x[4] = 4
x[5] = 5
x[6] = 6
x[7] = 7
x[8] = 8
x[9] = 9
x[0] = 0
x[1] = 3.7
x[2] = 7.4
x[3] = 11.1
x[4] = 14.8
x[5] = 18.5
x[6] = 22.2
x[7] = 25.9
x[8] = 29.6
x[9] = 33.3
```
