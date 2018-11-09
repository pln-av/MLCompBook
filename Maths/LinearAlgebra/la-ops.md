# Basic Linear Algebra

## Definitions
[comment]: <> (Definition from https://golem.ph.utexas.edu/wiki/instiki/show/Theorems)
Here I want to introduce some notation that I use (which may be non-standard) and some ideas 
which are basic and well known, but helpful to keep in mind when reading the ML literature.
The vector $\mb{u}$ with $n$ elements is either a horizontal or vertical collection of $n$ 
numbers.  In the former $\mb{u}$ is *row* vector (and I note its dimension 
$\mb{u}\equiv\msize{1}{n}$) and the latter it is a *column* vector ($\mb{u}\equiv\msize{n}{1}$).
By default all vectors are column vectors.  A column vector $\mb{u}$ can be written as a row
by tranposing (ie $\transpose{\mb{u}}$).  We define the two vector-vector products:

### Vector Products

The *dot product* of two vectors $\mb{u}$ and $\mb{v}$ is the scalar
\eq{
    \mb{u}\cdot\mb{v} = \transpose{\mb{u}}\mb{v} = \sum \mb{u}_i \mb{v}_i
} 
The *vector product* of two vectors $\mb{u}$ and $\mb{v}$ is the matrix
\eq{
    \mb{u}\transpose{\mb{v}} = 
    \mat{
         u_1  v_1 & u_1  v_2 & \hdots & u_1  v_n \\
         u_2  v_1 & u_2  v_2 & \hdots & u_2  v_n \\
         \vdots & \vdots & \ddots & \vdots \\
         u_n  v_n & u_1  v_n & s & u_n  v_n
    } 
}   
The vectors $\mb{u}$ and $\mb{v}$ are *orthogonal* if their dot products is zero, ie
$\mb{u}\cdot\mb{v} =0$.  The $m\times n$ matrix $A$, where 
\eq{A = \matx{A}{m}{n}}
is often decomposed into its $m$ rows 
\eq{
    A = \colx{ \row{A} }{m}
}
or $n$ columns
\eq{
    A = \rowx{ \col{A} }{n}
}
so as to better represent matrix operations.

### Matrix Products 

There are a number of ways to represent matrix-vector and matrix-matrix products, and different 
representations are usefull in different contexts.  Here I will go through some of the main ideas.
It is good to go through each a couple of times using a $\msize{2}{2}$ or $\msize{3}{3}$ to 
convince yourself of the structure of the following products.

The matrix-vector product $A\mb{x}$ is most commonly viewed as the weighted sum of the columns of $A$ 
ie
\eq{
    A\mb{x} = \sum x_i \col{A}_i
}
or it can be interpreted as a sequence of dot products using the *rows* of $A$
\eq{
    A\mb{x} = \mat{\row{A}_1 \cdot \mb{x} \\ \row{A}_2 \cdot \mb{x} \\ \vdots \\ \row{A}_N \cdot \mb{x}}
}
As might be expected, the vector-matrix product $\transpose{x}A$ is a sequence of dot products
using the *columns* of $A$
\eq{
    \transpose{x}A = 
    \mat{
        \transpose{\mb{x}}\cdot \col{A}_1, & \transpose{\mb{x}}\cdot \col{A}_2, & \hdots, 
        & \transpose{\mb{x}}\cdot \col{A}_n
        }
}
Matrix products $AB$ where $A\equiv\left(m\times n\right)$ and $B\equiv\left(n\times m\right)$ are a matrix 
of dot products
\eq{
    AB = 
    \mat{
      \row{A}_1 \cdot \col{B}_1 & \row{A}_1 \cdot \col{B}_2 & \cdots & \row{A}_1 \cdot \col{B}_m \\
     \row{A}_2 \cdot \col{B}_1 & \row{A}_2 \cdot \col{B}_2 & \cdots & \row{A}_2 \cdot \col{B}_m \\
     \vdots & \vdots & \ddots & \vdots \\
     \row{A}_m \cdot \col{B}_m & \row{A}_1 \cdot \col{B}_m & \cdots & \row{A}_m \cdot \col{B}_m
    } 
}
This representation is the most common way for me to work towards proofs of various matrix properties.
It is also usefull to note that matrix-matrix products can be seen as a collection of the matrix-vector
or vector-matrix products we have just seen.  The matrix-matrix product $AB$ is a row of matrix-vector
products using the columns of $B$
\eq{
    AB = \mat{A\col{B}_1, & A\col{B}_2, & \hdots, & A\col{B}_n}
}
or it is a column of vector-matrix products using the rows of $B$
\eq{
    AB = \mat{ \row{A}_1 B \\ \row{A}_2 B \\ \vdots \\ \row{A}_n B}
}
All of these interpretations will be used in the following notes (I think).

### Special Matrices

There are a number of matrices with special structure which are found repeatedly in
numerical analysis and machine learning.  These are diagonal matrices, triangular
matrices, symmetric matrices, orthogonal matrices and symmetric positive definite matrices.

#### Diagonal Matrices

*Diagonal Matrices* are denoted $D$ and are all zero except the main diagonal, ie $D_{ij}=0$ 
if $i\neq j$.  Diagonal matrices are one of two types of *elementary matrices*

* Diagonal or Scaling matrices
* Permutation matrices

though we will delay discussion of permulation matrices until a little later.  It is good to
note that as elementary matrices, both diagonal and permulation matrices should be seen as 
matrices which perform basic transformations of other matrices, such as scaling columns/rows
or by switching rows/columns around.  Diagonal matrices do that former - ie *Diagonal matrices 
are a scaling operation*. Diagonal matrices are denoted $D$, and permutation matrices $P$.  A 
general elementary matrix is denoted $E$.  Multiplication by an elementary matrix $E$ transforms 
either rows or columns of its operand depending on whether we pre- or post- multiply (ie $EA$ or $AE$).
 
 Operation | Effect
 ----------|---------
 pre-multiply (ie $EA$) | row transformation
 post-multiply (ie $AE$) | column transformation 

Here, put in some examples of elementary matrices.

As with other *elementary matrices*, pre-multiplying is a row transformation, post-multiplying is 
a column transformation. To see this, recognise the matrix product as a matrix of dot products.  In 
the case of pre-multiplication, where $D$ is diagonal with  $D_{ii} = d{i}$
\eqa{
     \func{DA}_{ij} & = \row{D}_i \cdot \col{A}_j \\
                    & = D_{ii} A_{ij} \\
                    & = d_i A_{ij}
} 
where the second line occurs because all elements in $\row{D}_i=0$ except the $\ith{i}$ element, 
so only the $\ith{i}$ element in $\col{A}_i$ (ie $A_{ij}$), survives the dot product.  The result is the 
whole of the $\ith{i}$ row of $A$ is multiplied through by $d_i$.  The analgous result happens 
for post-multiplication. If $A$ is *both* pre and post multiplied we will end up with all pairwise
combinations of the diagonals $D_{ii}$, ie
\eq{
    \func{DAD}_{ij} = d_i d_j A_{ij}
}
This is usefull in particular with correlation and covariance matrices.

*Triangular matrices* are ubiquitious in computation linear algebra, because they can be solved with
little effort.  Matrices can be upper or lower triangular:

* *Upper Triangular:* $U$ is upper triangular if all $U_{ij}=0$ when $i<j$
* *Lower Triangular:* $L$ is lower triangular if all $L_{ij}=0$  when $i>j$

I often refer to `lower` and `upper` as being `types` of triangular matrices.  Assume everywhere that $T$ 
is a triangular matrix of some type, and $U$/$L$ are upper/lower triangular matrices unless otherwise 
specified.  Triangular matrices (aside from diagonal matrices, which are of course both upper and lower
triangular) are very common because the systems of equations they represent are very easy to solve.  Note
that 

* If $T_1,T_2$ have the same type, then the sum $T = T_1 + T_2$ also has the same type. 
* If $T_1,T_2$ have the same type, then the product $T=T_1 T_2$ also has the same type.

*Symmetric Matrices* have corresponding elements across the main diagonal equal, ie $S_{ij}=S_{ji}$.
This simple structure results in many useful properties, and many important matrices, such as 
correlation matrices are symmetric.

*Orthogonal Matrices* are matrices made up of orthogonal columns.   Remember that two vectors $\mb{u}$
and $\mb{v}$ are orthogonal if $\mb{u}\cdot\mb{v}=0$.  If a matrix $Q$ is orthogonal (often $Q$ is used 
to represent orthogonal matrices) $\col{Q}_i \cdot \col{Q}_j= 0$ for any columns $i$, $j$.  The 
special case $Q$ where $\col{Q}_i \cdot \col{Q}_i=1$ is referred to as *orthonormal*.    Orthogonal 
matrices have excellent numerical properties, so many common numerical methods rely on finding 
orthogonal matrices.  The critical feature of orthogonal matrices is the product $\transpose{Q}Q=I$,
since the $\ith{ij}$ element in the product
\eqa{
    \func{ \transpose{Q}Q }_{ij} & = \transpose{ \col{Q}_{i} } \cdot \col{Q}_{j} \\
                                 & = 0 \; \textrm{if} \; i \neq j
}
The critical point is that its transpose (which is easy to find) is its
inverse.  We will discuss inverses shortly. 

*Symmetric Positive Definite Matrices* or *SPD* matrices are common and incredibly useful in both 
numerical and analytic work.  An SPD matrix is a special case of a symmetric matrix (ie if $S$ is 
SPD it is symmetric, though the converse is not necessarily true), where the *quadratic form*
$\transpose{x}Sx>0$ is positive for any $x$.  The *quadratic form* is a quantity that reappears 
a number of times in different contexts, so we should look at it briefly.  The quadratic form is 
a function of $x$, and is essentially a weighted sum of squares of $x$ where the elements in $S$ 
make up the weights.  For example the $2\times 2$ case 
\eqa{
    \transpose{x}A x & = \mat{x_1 & x_2}
                         \mat{
                             A_{11} & A_{12} \\
                             A_{21} & A_{22}
                        }
                        \mat{x_1 \\ x_2} \\
                     & = x_{1}^2 A_{11} + x_1 x_2 \func{A_{12}+A_{21}}+x_{2}^2 
}
More generally, the vector-matrix-vector product $\transpose{\mb{y}}Ax$, which is a more general
expression than the quadratic form, can be written as a sum of all elements in $A$ in the 
form 
\eq{
    \transpose{\mb{y}}Ax = \sum_i \sum_j A_{ij}y_i x_j
}
which is shown below as it is a quite usefull algebra exercise
\eqa{
    \transpose{\mb{y}}Ax 
    & = \func{ \transpose{\mb{y}}\col{A}_1,\;\hdots,\;\;\transpose{\mb{y}}\col{A}_n } \mb{x}  \\
    & = x_1 \func{y_1 A_{11} + y_2 A_{21} + \hdots + y_n A_{n1}} 
        + \hdots + x_n \func{ y_n A_{1n} + y_2 A_{2n} + \hdots + y_n A_{nn}}
}
where each element $A_{ij}$ appears once in the sum in an expression $A_{ij} y_i x_j$, which
gives the above identity.  The special case here, where $\mb{y} = \mb{x}$ gives
\eq{
    \transpose{\mb{x}}Ax = \sum_i \sum_j A_{ij} x_i x_j
}
Last, a common matrix expression in statistics/ML is the matrix $S=\transpose{A}A$, which is often 
called the Grammian.  It is a particularly useful symmetric positive definite matrix.  It is 
symmetric, since
\eqa{
    \transpose{S}= & \transpose{\func{\transpose{A}A}}\\
      = & \transpose{A}\transpose{\func{\transpose{A}}} \\
      = & \transpose{A}A \\
      = & S
}
and positive definite, since 
\eqa{
    \transpose{\mb{x}}S\mb{x} & = \transpose{\mb{x}}\transpose{A}A\mb{x} \\
                              & = \transpose{ A\mb{x}} A\mb{x} \\
                              & = \transpose{\mb{u}}\mb{u} \\
                              & = \sum u_{i}^2 \geq 0
}
where $\mb{u}=A\mb{x}$.  This actually guarantees *semi-positive definiteness* only, but 
if the columns of $A$ are independant, then $S=\transpose{A}A$ is positive definite. We will
look at independance in the next section, but note that being *positive definite* requires
the following is non-zero (it cant be negative)
\eqa{
    \transpose{\mb{x}}\transpose{A}A\mb{x} = & \transpose{\mb{u}}\mb{u} \\
                                           = & \left| \mb{u} \right| \\
                                           = & \left| A\mb{x} \right| > 0
}
which is only zero when $A\mb{x}=0$.  If we have independance in $A$, we are guaranteed 
$S=\transpose{A}A$ is symmetric positive definite.

### Column Space, Rank and Indepdance

Return the matrix-vector product $A\mb{x}$, which we saw is the weighted sum of
the columns of $A$.  The set of all possible combinations of the columns defines the *column space* 
of A - that is, all possible outputs of the operation $A\mb{x}$.  This idea is coupled to the more 
general idea of a *vector space*.  More specifically, the *vector space* generated by a set of $n$ 
vectors $\left\{\mb{u}_1,\hdots,\mb{u}_n\right\}$ is the set of all possible linear combinations of 
the $\mb{u}_i$.  If we combine these vectors to form the columns of a matrix $A$, the we get the linear 
combination from the above matrix-vector product $A\mb{x}$.  The column space of a matrix $A$ then is 
the *vector space* generated by its columns.  This *vector space*, which is the *column space* of $A$ 
is often also called the *range* of $A$, ie
\eq{
    \textrm{range}\left(A\right) 
    = \left\{ \mb{u}\in\mc{R}^n \; \textrm{where there exists} \; \mb{x} \; \textrm{with} \; A\mb{x}=\mb{u} \right\}
}
since it defines which vectors can be produced by summing the columns in $A$.  The *null space*
is a particular set of non-zero weights $\mb{x}$ which produce the zero vector, ie
\eq{
    \textrm{null}\left(A\right) 
    = \left\{ \textrm{all} \; \mb{u} \; \textrm{where} \; A\mb{u}=0 \right\}
}
Not all matrices have a non-empty null space.  If such an $\mb{u}$ exists, then at least one column 
can be written as a linear combination of at least one of the others.  This has significant 
implications - it means that one of the rows in $A$ is a redundant piece of information. If no such 
combination exists - that is, the null space is empty, then the columns are *linearly independant* - 
no columns can be written as a combination of the others.  This concept is again common more generally 
to sets of vectors - ie a set of vectors is linearly independant if no linear combination of the vectors 
produces a zero vector.  Spaces which are *not* linearly independant include redundant vectors.  

The *column rank* of matrix $A$ is the largest number of independant columns in $A$.  If all
columns are independant, the matrix has *full rank*.  It is denoted $\textrm{colrank}\left(A\right)$.
The *row rank* is defined similarly, but is used much less frequently - the *rank* of a matrix
$\textrm{rank}\func{A}$ usually refers to the column rank.  Some examples would be good.

Consider a matrix $A$ and some set of vectors $\rowx{\mb{u}}{n}$.  For every possible combination
of the columns of $A$ (ie every $\mb{v}=A\mb{u}$), it may be possible to choose some combination
of the vectors $\mb{u}_i$ that produces the same $\mb{v}$, and vica versa.  In this sense, the set of 
vectors$\rowx{\mb{u}}{n}$ can substitute for the matrix $A$, since they can both generate the exact same
output (ie they have the same range).  This might be useful if the vectors $\mb{u}_i$ had properties
that were more desireable than the columns of $A$.  Indeed, this is done very commonly in numerical
methods in linear algebra.   If we want to do this substitution however, we shouldnt choose any redundant 
vectors in the $\mb{u}_i$ - we should only use *linearly independant* vectors.  Such a set of vectors is 
said to form a *basis* for $A$.


# Matrix Inverse and Non-Singular Matrices 

## Definitions
Recall the matrix-vector product $A\mb{x}=\mb{y}$.  Is it possible to work out the particular $\mb{x}$ 
which results in a particular $\mb{y}$?  Clearly $\mb{x}$ should be in column space of $A$, ie
$\mb{x} \in \col{A}$.  Also, if $\mb{x}\in\mathcal{R}^n$ so that we have $n$ variables to find, we
need $n$ degrees of freedom - there can be no redundant information in the columns of $A$.  Therefore
$A$ should have full rank, or equivalently have an empty null space. Let us formalise some of these
ideas below.

An $\msize{n}{n}$ matrix $A$ is *non-singular* if there exists another matrix $B$ where $AB=I$.  If
such a matrix exists is it unique, and we denote it $B=\inverse{A}$.  To see this, assume instead that 
$\inverse{A}=B_1$ and $\inverse{A}=B_2$ where $B_1\neq B_2$ - that is, there are two different matrices 
which are inverses of $A$.  By definition
\eqa{
    AB_1 & = I = B_1 A \\
    AB_2 & = I = B_2 A 
}
and by multiplying the second equation by $B_1$ we see 
\eq{
    \left(B_1 A\right)B_2 = B_1 I 
}
where the bracketed term is $I$ and we have $B_2=B_1$.  The previous definitions of *rank*, *range*
and *null space* are usefull here.  When determing if $\inverse{A}$ exists, remember
\eqa{
    \inverse{A} & \iff \textrm{null}\func{A} = \left\{ \varnothing \right\} \\
    \inverse{A} & \iff \textrm{range}\func{A} = \mc{R}^n \\
    \inverse{A} & \iff \textrm{rank}\func{A} = n
}
When finding if $A$ is singular analytically, I ususally check if 
\eq{
     \textrm{null}\func{A} = \left\{ \varnothing \right\}
}
Some important identities that are used regularly are
 \eqa{
     \inverse{\func{AB}} & = \inverse{B}\inverse{A} \\
     \inverse{\func{\transpose{A}}} & = \transpose{\func{\inverse{A}}}
 }
 To check the first, let $M=AB$, and note that 
 \eqa{
     \inverse{B}\inverse{A}M & = \inverse{B}\func{\inverse{A}A}B  = I\\
 }
 so that $\inverse{M} = \inverse{B}\inverse{A} = \inverse{\func{AB}}$. For the 
 second, consider $\transpose{\func{\inverse{A}}}\transpose{A} = \transpose{\func{A \inverse{A}}} = I$

Note that if $A$ is *diagonal* or *triangular*, its determinant is particular simple
\eq{
    \determinant{A} = \prod A_{ii}
}
Since $A$ is non-singular if $\determinant{A}=0$ diagonal and triangular matrices can have no 
zeros on the main diagonal.  If all diagonal elements are non-zero then $\inverse{A}$ exists.  Also,
if $\inverse{A}$ exists, then
\eqa{
    A \; \textrm{is diagonal} \iff & \inverse{A} \; \textrm{is diagonal} \\
    A \; \textrm{is upper triangular} \iff & \inverse{A} \; \textrm{is upper triangular} \\
    A \; \textrm{is lower triangular} \iff & \inverse{A} \; \textrm{is lower triangular}
}
The inverse of a diagonal matrix $D$ has
\eq{ 
    \func{\inverse{D}}_{ii}  = 1/D_{ii}
}
which is what we might expect when viewing $D$ as a scaling operation (ie to make the inverse, just
use the opposite scaling factor!).

## Analytic Matrix Inverse
 
Here, add a discussion of elementary matrix operations, and finding inverses.

## Numeric Matrix Inverse

Include a section on why we dont actually computin matrix inverses, conditioning etc.

Most numeric methods for solving systems of equations
\eq{
    A\mb{x} = \mb{y}
}
involve decomposing $A$ into a product of two matrices (ie perform a *matrix decomposition*), one of 
which is triangular (or has some other simple form which makes solving fast and easy).  The solution
then usually proceeds in two steps.  The most basic is the LU decomposition, where we find lower and 
upper triangular matrices $L$, $U$ where $A=LU$.  The above equation then reduces to 
\eqa{
    A \mb{x} & = \mb{y} \\
    L\func{U \mb{x}} & = \mb{y} \\
    L\mb{v}  & = \mb{y}
}
where $U\mb{x} = \mb{v}$.  Here we solve for $\mb{v}$, which is very fast since $L$ is triangular (as
we will see below).  Once we have $\mb{v}$, we use it to solve for $\mb{x}$ in $U\mb{x}=\mb{v}$.  Again,
since $U$ is triangular this is very fast.  

## Solving Triangular Matrices

Triangular matrices can be solved very quickly using backward/forward substitution. Consider solving
$U\mb{x} = \mb{y}$.  The bottom row only has one one non-zero element $U_{nn} x_n = y_n$, so
\eq{
    x_n = y_n / U_{nn}
}.  The next row has two:
\eqa {
    U_{n-1,n-1}x_{n-1} & + U_{n,n}x_n = y_{n-1} \\
    x_{n-1} & = \func{y_{n-1} - U_{n,n}x_n}/U_{n-1,n-1} 
}
and we know $x_n$ from the previous step.  We continue on recursively.  An upper triangular matrix
is solved the identical way except starting at the top row instead of the bottom.  Clearly the 
process breaks down if $T$ has a zero element on the diagonal, but we expected that since this
would cause $\determinant{T}=0$. A triangular matrix of size $N$ will take $order{N^2}$ operations
to solve in this way.  Consider a particular row in the algorithm described above.  The operations are
\eqa{
    y_i = & \sum_{j=1}^{N} U_{ij} x_j \\
        = & \sum_{j=i}^{N} U_{ij} x_j \\
        = & U_{ii} x_i + \sum_{j=i+1}^{N} U_{ij} x_j \\
    x_i = & \func{y_i - \sum_{j=i+1}^{N} U_{ij} x_j} / U_{ii}
}
where the second line follows since the first elements $U_{i1}$ to $U_{i,i-1}$ are all zero, and in 
last last recognise that we have already found the next elements we are solving for, so that for 
$x_i$ we already have found $x_{i+1}, x_{i+2}, \hdots, x_N$.   Computation of $x_i$ involves 
the summation of $U_{ij} x_j$ for $j=i+1,\hdots,N$, which is $\order{N}$.  This calculation is 
performed $N$ times, once for each of the $N$ rows, so in total we require 
$N \order{N} = \order{N^2}$ operations.

## Creating Triangular Matrices with LU decompositions

The LU decomposition of a matrix $A$ are the upper and lower triangular matrices $U$ and $L$
where $A=LU$.  Conventionally, the main diagonal of $L$ is $L_{ii}=1$.  This ensures uniqueness,
since $A$ has $n^2$ elements, while $L$ has 
\eqa{
    \textrm{Elements} & = n + (n-1) + \hdots + 2 + 1 \\
                      & = \sum_{i=1}^{n} i \\
                      & = n(n+1)/2
}
non-zero elements, as does $U$.  So we have $n(n+1)$ non-zero variables in total to set using the 
$n^2$ elements of $A$.  To make this match set $n$ of them, the diagonal of $U$, to be all one.
When computing $L$ and $U$ analytically, we use sequences of transformations via elementary matrices.  In
practice $LU$ are computed numerically in $\order(N^3)$ operations.  Here, I should add both
numeric and analytic examples of LU decompositions.  Note that many authors refer to decompositions
where rows have been swapped (ie *pivoting*) as a *LU decomposition with pivoting*.  If a $LU$ 
decomposition is attempted directly without any pivoting the algorithm will fail anywhere with a 
zero element on the main diagonal.  For example the algorithm will fail to decompose
\eq{
    A = \mat{0 & 1 \\ 1 & 0}
}
where it is obvious that exchanging the rows leads to an easy solution.  If the pivot operations
on $A$ are collected in the matrix $P$, the the decomposition is $PA=LU$.  Here $P$ is *not*-unique,
but once $P$ is specified $LU$ is.  

The benefit of the $LU$ decomposition $PA=LU$ is that we can use forward and backward substitution
to very quickly solve linear systems.  Consider the system $A\mb{x}=\mb{y}$.  To solve, find $P$
which decomposes $PA=LU$, and solve $PA\mb{x}=LU\mb{x}=P\mb{y}$ in two steps.  The trick is to let 
$U\mb{x}=\mb{v}$, and solve in two steps as follows
\eqa{
    A\mb{x} = & \mb{y} \\
    PA\mb{x} = & P \mb{y} \\
    L\func{U\mb{x}} = &P\mb{y} \\
    L\mb{v} = &P\mb{y}
}
for $\mb{v}$, and then solve 
\eq{
    U\mb{x} = \mb{v}
}
for $\mb{x}$, as required.

# Eigenvalues and Eigenvectors

## Definitions 

*Eigenvectors* and *Eigenvalues* of a matrix $A$ are fundamental quantities used regularly in
matrix operations. The Eigenvector/value pair corresponding to a matrix $A$ is the pair 
$\mb{v}$,$\lambda$ with the property that
\eq{
    A\mb{v} = \lambda \mb{v}
}
The geometric interpretation of this is nice, but I've never found it particularly usefull.  In
two dimensions $\mb{v}$ is a line in the $x-y$ plane, and $A\mb{x}$ is a linear transformation
that changes the `direction` $\mb{v}$ points, as well as its length.  If $\mb{v}$ is an is an
eigenvector of $A$, then the transformation $A\mb{v}$ only changes the length of the line $\mb{v}$,
and not its direction.  What is the practical importance of that?  I'm not really sure.  It 
is quite tedious to find eigenvalue/vector pairs analytically (include a few examples).  Computing 
eigenvectors/values numerically is not as simple as the $LU$ decomposition described earlier either.
Since they are so critically important in all areas of applied maths/statistics, the computation of
eigenvalues is a huge field of study.   Note that if $\lambda$ is an eigenvalue with a corresponding 
eigenvector $\mb{v}$, then any multiple of $\lambda$ is also an eigenvalue.  As such two vectors
$\mb{u}$ and $\mb{v}$ with the above property, where $\mb{u}=c\mb{v}$ (ie one is just a multiple
of the other) do not count as separate eigenvectors.  To see this note that 
\eqa{
    A\mb{v} = & \lambda \mb{v} \\
    A\func{c\mb{v}} = & \func{c\lambda}\mb{v} \\
    A\mb{u} = & \lambda_u \mb{u}
}
where $\mb{u} = c\mb{v}$ and $\lambda_u = c\lambda$.

To finding eigenvalue/vector pairs, note that if $\lambda$ is an eigenvalue of $A$, then
\eqa{
    A\mb{v} = & \lambda \mb{v} \\ 
    \func{A-\lambda I} \mb{v} = & 0
}
which means the vector $\mb{v}$ is in the null-space of the matrix $A-\lambda I$.  If such a vector
exists, so that the null-space is non-empty, we know $\func{A-\lambda I}$ is a singular matrix.
Since the purpose is to find such a $\mb{v}$, we need to find the conditions for which 
$\func{A-\lambda I}$ is singular.  The most obvious is that it has a zero determinant; ie
\eq{
    \determinant{A-\lambda I} = 0
}
This results in a polynomial equation for $\lambda$, which may have multiple solutions, some of
which might be repeated.  This equation is refered to as the *characteristic polynomial* of $A$.  Once 
we have all the solutions $\lambda_i$, we can work out the corresponding $\mb{v}_i$.  These are the eigenvalue/vector pairs.  This is quite a tedious process!  I should put in an example or two here.

Since computing the $\lambda_i$ reduces to working with determinants, the special structure of some
matrices means that computing the $\lambda_i$ is straight forward.  The subtraction $A-\lambda I$ only 
modifies the diagonal of $A$, many matrices retain the special structure after the subtraction.  Most 
importantly:

* **Diagonal Matrices** If $D$ is diagonal, then $A-\lambda I$ is diagonal too.  The determinant of
a diagonal matrix is $\determinant{D}=\prod D_{ii}$, so the characterist polynomial of $A-\lambda I$
is $\determinant{A-\lambda I} = \prod \func{D_{ii} - \lambda} = 0$. As such the diagonal elements
$D_{ii}$ are themselves the eigenvalues of $D$.

* **Triangular Matrices** If $T$ is triangular, then $A-\lambda I$ is triangular too.  The determinant of
a triangular matrix is $\determinant{T}=\prod T_{ii}$, so the characterist polynomial of $A-\lambda I$
is $\determinant{A-\lambda I} = \prod \func{T_{ii} - \lambda} = 0$. As such the diagonal elements
$T_{ii}$ are themselves the eigenvalues of $T$.

Eigenvector/value pairs give a great deal of information about the matrix they were computed from.  Let
$\func{ \lambda_{i}, \mb{v}_{i} }$ be the eigenvalue/vector pairs of the matrix $A$.  

* If any $\lambda_i=0$, then $A$ is singular.  Remember that if $A$ is non-singular, then it has 
an empty null-space.  That is, there can be no non-trivial vector $\mb{v}$ where $A\mb{v}=0$.  
However, if $\lambda=0$ is an eigenvalue, and $\mb{v}$ its corresponding eigenvector, then
$A\mb{v} = \lambda \mb{v} = 0$.  This means the null-space of $A$ is not empty (ie the 
eigenvector $\mb{v}$ is in it), so $A$ is singular.
* An $\func{n\times n}$ matrix A has exactly $n$ eigenvalues.  Some may be repeated, and some may
be complex.
* Every eigenvalue has at least one $eigenvector$.
* If $\lambda_1 \neq \lambda_2$ then their corresponding eigenvectors are linearly independant.
* If $A$ has an eigenvalue with multiplicity $m$, it has at least one eigenvector, but may have 
less than $m$ eigenvectors.  This means that $A$ may have $n$ eigenvectors which are all linearly
independant, but it may have less if any eigenvalues were repeated.
* If $\func{\lambda,\mb{v}}$ are an evalue/vector pair from a non-singular $A$, then 
$\func{\lambda,\mb{v}}$ are an evalue/vector pair from $\inverse{A}$.

Interestingly, the eigenvectors/values of symmetric matrices have a number of additional useful
properties.  

* If $S$ is symmetric, then $S$ has a full set of $n$ eigenvectors.
* If $S$ is symmetric, each of its $n$ eigvvalues are real.
* If $S$ is symmetric, then all $n$ of these eigenvectors are orthogonal.
* If $S$ is symmetric, then its eigenvectors ${v}_i$ and $\mb{v}_j$ are othogonal if the corresponding eigenvalues 
are not repeated (ie $\lambda_i \neq \lambda_j$).

## Diagonalisation

A particularly useful application of eigenvalues and eigenvectors is the following decomposition.
The matrix $A$ is diagonaliseable if $A$ can be written
\eq{
    A = VB\inverse{V}
}
where
\eqa{
    V \; \rightarrow \; & \textrm{columns are e-vectors of} \; A \\
    B \; \rightarrow \; & \textrm{diagonal matrix where} \; B_{ii} \; \textrm{are e-values of } \; A
}
Note that by writting $A=VB\inverse{V} \rightarrow AV = BV$, we see the above link between diagonalisation
and eigenvalue/vector pairs
\eqa{
    A = & VB\inverse{V} \\
    AV  = & VB \\
    \mat{A \mb{v}_1, & A\mb{v}_2,  & \hdots, & A\mb{v}_n} = & 
    \mat{B_1 \mb{v}_1, & B_2\mb{v}_2,  & \hdots, & B_n\mb{v}_n} 
}
and if we equate each column on the left and right hand sides, we have
\eq{
    A\mb{v}_i = B_i \mb{v}_i
}
ie $\mb{v}_i$ and $B_i$ and are eigenvector/value pair.  For $A$ to be diagonalisable we require
$\inverse{V}$ to exist - since this is a matrix whose columns are the evectors of $A$, we require 
all $n$ eigenvectors to be linearly independant.

## Extra Results for SPD Matrices

The additional eigenvector/value results for symmetric matrices also provide additional 
properties regarding diagonalisation.  If $S$ is symmetric, we are guaranteed to have $n$ 
eigenvalues, and its $n$ eigenvectors are all orthogonal. As such, if $S$ is symmetric then it
is diagonalisable, ie $S = VB\inverse{V}$, and since the columns of $V$ are orthogonal, $V$ is 
an orthogonal matrix $V=Q$ and $\inverse{Q}=\transpose{Q}$.  The diagonal form of a symmetric 
matrix is useally written
\eq{
    S = QB\transpose{Q}
} 
If all the eigenvalues of $S$ are positive, then $S$ is SPD.  This converse is also true - if
$S$ is SPD then its eigenvalues are all positive.  Since all $\lambda_i>0$ we are guaranteed
that $S$ is non-singular, and that $\inverse{S}$ is also SPD.  This makes sense - when 
$\lambda_i,\mb{v}_i$ are the eigenvalue/vector pairs of $S$, then $1/\lambda_i,\mb{v}$ are the 
eigenvalue/vector pairs of $\inverse{S}$.  Since $\lambda_i>0$ then $1/\lambda_i$ exists and is 
positive.  So $\inverse{S}$ is symmetric, and all its eigenvalues are positive - it is SPD!

Finally, SPD matrices can be factorised in a particularly efficient way - the *Cholesky decomposition*.
The Cholesky decomposition is another very common matrix decomposition applicable only to 
SPD matrices.  In practice, the Choleksy algorithm determines whether a given matrix is in
fact SPD.  The decomposition of a SPD matrix $S$ is
\eq{
    S = \transpose{U}U
}
where $U$ is upper triangular.  If $S$ is symmetric and such a decomposition exists, then
it must be SPD, since
\eqa{
    \transpose{\mb{x}}S\mb{x} & = \transpose{\mb{x}}\transpose{U}U\mb{x} \\
                              & = \transpose{ \func{U\mb{x}} }U\mb{x} \\
                              & = \left|U\mb{x}\right|^2 > 0
}
As with our discussion of LU decomposition, a Cholesky decomposition allows fast solutions
to systems $A\mb{x}=\mb{y}$ via forward and back subsitutions, since $U$ is triangular.
The particular form of the Cholesky decomposition means the *Cholesky factor* $U$ is often
described as the `square root` of a matrix, which we will see soon in the section on 
random variables and simulation.

# Multivariate Random Variables

## Basics

Consider a set $\left\{x_1,x_2,\hdots,x_m\right\}$ of random variables.  The moments we
will focus on are mean, variance, covariance, and correlation which are defined 
\eqa{
    \textrm{mean:} \; \mu_i               = & \expected\func{x_i}  \\
    \textrm{variance:} \; \sigma_{i}^{2}  = & \expected\left[\func{x_i-\mu_i}^2\right] \\
    \textrm{covariance:} \; \sigma_{ij}^2 = & \expected\left[\func{x_i-\mu_i}\func{x_j-\mu_j}\right] \\
    \textrm{correlation:} \; \rho_{ij}    = & \frac{\sigma_{ij}^2}{\sigma_i \sigma_j}
}
Both the covariances and correlations can be collected into $\msize{m}{m}$ matrices $\Sigma$ and $\Omega$
where
\eqa{
    \Sigma_{ij} & = \sigma_{ij} \\
    \Omega_{ij} & = \rho_{ij} 
}
The matrix $\Sigma$ is built by taking the expectation of 
\eq{
    \Sigma = \expected \left[ \transpose{ \func{\mb{x} - \mu}} \func{ \mb{x}-\mu } \right]
}
since each element $\Sigma_{ij} = \sigma_{ij}$ involves the product $\func{x_i-\mu_i}\func{x_j-\mu_j}$.
The correlation matrix is easily computeable from $\Sigma$, by noting that it just requires scaling
by each pairwise combination of standard deviations $\sigma_i$ and $\sigma_j$.  Using the diagonal
matrix $D$, where $D_{ii} = \sigma_i$ we can write 
\eq{
    D\Omega D = \Sigma
}
where the first $D$ multiplies each *row* of correlations by $\sigma_i$, and the second post 
multiplication multiplies each *column* by $\sigma_j$. More commonly we write
\eq{
    \Omega = \inverse{D}\Sigma\inverse{D}
}
since usually the variances $\Sigma$ are computed first.

The sample estimates of these quantities can be efficiently computed using linear algebra.  Let
$\iota$ be a $\msize{n}{1}$ vector of ones, and collect our $n$ observation of each $\mb{x}_i$ into a 
matrix $X$, where $\col{X}_i$ has the $n$ observations of $\mb{x}_i$, then
\eqa{
    \hat{\mu} = & \transpose{\iota}\mb{x}/n \\
    \hat{\sigma}_{i}^{2} = & \transpose{\func{ \mb{x}_i-\hat{\mu}_i }} \func{\mb{x}_i-\hat{\mu}_i} /\func{n-1} \\
    \hat{\Sigma} = & \transpose{\func{ X -\hat{\mbs{\mu} }}} \func{ X-\hat{\mbs{\mu}} } /\func{n-1}
}
Writing these formulas using linear algebra may seem a little unnecessary, but they are important for
two reasons:

* Many analytic results in statistics and ML follow from these representations
* Linear algebra packages (eg Intel MKL) are highly optimised to compute matrix-vector operations 

It is common to ompuate the covariance of random variables which themselves are combinations of
the variables $\left\{x_1,x_2,\hdots,x_m\right\}$.  Again collect the $x_i$ into a vector $\mb{x}$, and 
define $z_1=\transpose{\mb{a}}\mb{x}$ and $z_2=\transpose{\mb{b}}\mb{x}$.  For simplicity assume that
$\mu_i=0$ and let's compute $\covar\func{z_1,z_2}$, as follows:
\eqa{
    \covar\func{z_1,z_2} & = \covar\left[ \func{ \sum a_i x_i},\func{ \sum b_i x_i} \right] \\
                         & = \expected \func{ \sum_i \sum_j a_i b_j x_i x_j } \\
                         & = \sum_i \sum_j a_i b_j \expected \func { x_i x_j } \\
                         & = \sum_i \sum_j a_i b_j \Sigma_{ij}
}
We can build this quadratic-form like sum using $\expected\func{\mb{x}\transpose{\mb{x}}}$ to get the 
required cross-products so that 
\eqa{
    \covar \func{z_1,z_2} & = \transpose{\mb{a}}\expected\func{\mb{x}\transpose{\mb{x}}}\mb{b} \\
                          & = \transpose{\mb{a}}\Sigma\mb{b}
}
More generally, consider the random variables $\left\{y_1,y_2,...,y_m\right\}$ created from linear
combinations of the $x_i$.  Collect the rvs into $\mb{x}$ and $\mb{y}$ and after setting
\eq{
    \mb{y} = A \mb{x}
}
the objective is to compute $\Sigma_y$ using $\Sigma_x$, as we did above in the case of two linear 
combinations $z_1$ and $z_2$ for $\mb{y}$
\eqa{
    \Sigma_y & = \expected \left[\func{\mb{y}-\mbs{\mu}_y}\transpose{\func{\mb{y}-\mbs{\mu}_y}}\right] \\
             & = \expected \left[M\func{\mb{x}-\mbs{\mu}}\transpose{\func{M\func{\mb{x}-\mbs{\mu}} }}\right] \\
             & = \expected \left[M\func{\mb{x}-\mbs{\mu}}\transpose{\func{\mb{x}-\mbs{\mu}}}\transpose{M} \right] \\
             & = M \expected\left[\func{\mb{x}-\mbs{\mu}}\transpose{\func{\mb{x}-\mbs{\mu}}}\right] \transpose{M} \\
             & = M \Sigma_x \transpose{M}
}
This property is critical - it is the recipe for generate one set of random variables from another.  If
\eq{
    \mb{x}\sim\func{\mbs{\mu}_x,\Sigma_x}
} 
then $\mb{y}=\mb{a}+M\mb{x}$ has distribution 
\eq{
    \mb{y}\sim\func{\mb{a}+\mbs{\mu}_x,M\Sigma_x \transpose{M}}
}

## More on Correlation Matrices

As shown above, correlation matrices provide the recipe for transorming one set of random variables into
another.  It is critical to note that the $\msize{n}{n}$ correlation matrix $\Sigma$ is SPD.  First, it
is clearly symmetric, since it is computed from the product $\mb{x}\transpose{\mb{x}}$.  To show it is
positive definite, we are required to show $\transpose{\mb{x}}\Sigma\mb{x}>0$.  Return to the 
previous computation
\eq{
    \covar \left[ \func{\sum a_i x_i}, \func{\sum b_i x_i} \right] = \transpose{\mb{a}}\Sigma\mb{b}
}
and set $a_i=b_i$.  Then the covariance calculation is a variance, ie
\eqa{
    \covar \left[ \func{\sum a_i x_i}, \func{\sum a_i x_i} \right] & = \variance \func{\sum a_i x_i} \\
                                                        & = \transpose{\mb{a}}\Sigma\mb{a} \\
                                                        & > 0
} 
Since a variance is clearly non-negative.  So we have shown that the covariance matrix $\Sigma$ is SPD. 
From here, many properties follow.  Using a number of these properties, we can show that $\Sigma$ has a 
cholesky decomposition, and infer that the Cholesky factor provides the means to generate multivariate
random variables with a certain required covariance matrix.  To see this, diagonalise $\Sigma$ - we
are guaranteed to be able to do this since $\Sigma$ is symmetric ie
\eq{
    \Sigma = VB\inverse{V}
}
where $B$ is diagonal with $B_{ii}$ the eigenvalues of $\Sigma$, and $\col{V}_i$ the corresponding 
eigenvectors.  We are guaranteed the all $B_{ii}>0$, so we can define $A$ to be another symmetric 
matrix with $A_{ii} = \sqrt{B_{ii}}$.  Finally, note that since $\Sigma$ is symmetric, its eigenvalues 
are orthogonal - so write $V=Q$ to make the orthogonality of the eigenvectors making up the columns of 
$V$ more clear.  Since $\inverse{Q} = \transpose{Q}$, we can write
\eqa{
    \Sigma & = VB\inverse{V} \\
           & = Q AA \transpose{Q} \\
           & = Q A \transpose{ \func{ Q \transpose{A}} } \\
           & = Q A \transpose{ \func{ Q A}} \\
           & = U\transpose{U}
}
where $U=QA$.  This is critical - say I have $\Sigma_z = I$, ie uncorrelated rvs.  Now, I want to
generate $\mb{x}$ with a specific correlation matrix $M$.  Take Cholesky decomposition $U\transpose{U}=M$,
and simulation $\mb{x}=U\mb{z}$.  Then $\mb{x}$ has correlation
\eqa{
    \Sigma_x & = U \Sigma_z \transpose{U} \\
             & = U I \transpose{U} \\
             & = U \transpose{U} \\
             & = M
}
which is the target correlation matrix!