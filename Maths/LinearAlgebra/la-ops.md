# Basic Linear Algebra Operations and Definitions
[comment]: <> (Definition from https://golem.ph.utexas.edu/wiki/instiki/show/Theorems)
Here I want to introduce some notation that I use (which may be non-standard) and some ideas 
which are basic and well known, but helpful to keep in mind when reading the ML literature.
The vector $\mb{u}$ with $n$ elements is either a horizontal or vertical collection of $n$ 
numbers.  In the former $\mb{u}$ is *row* vector (and I note its dimension 
$\mb{u}\equiv\msize{1}{n}$) and the latter it is a *column* vector ($\mb{u}\equiv\msize{n}{1}$).
By default all vectors are column vectors.  A column vector $\mb{u}$ can be written as a row
by tranposing (ie $\transpose{\mb{u}}$).  We define the two vector-vector products:

###### *Vector Products*
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

The vectors $\mb{u}$
A $m\times n$ matrix $A$, where 
\eq{A = \matx{A}{m}{n}}
is often decomposed into its $m$ rows 
\eq{
    A = \colx{ \row{A} }{m}
}
or $n$ columns
\eq{
    A = \rowx{ \col{A} }{n}
}
so as to better represent matrix operations.  With this notation we view the matrix product $AB$ where 
$A\equiv\left(m\times n\right)$ and $B\equiv\left(n\times m\right)$ as a matrix of dot products
\eq{
    AB = 
    \mat{
      \row{A}_1 \cdot \col{B}_1 & \row{A}_1 \cdot \col{B}_2 & \cdots & \row{A}_1 \cdot \col{B}_m \\
     \row{A}_2 \cdot \col{B}_1 & \row{A}_2 \cdot \col{B}_2 & \cdots & \row{A}_2 \cdot \col{B}_m \\
     \vdots & \vdots & \ddots & \vdots \\
     \row{A}_m \cdot \col{B}_m & \row{A}_1 \cdot \col{B}_m & \cdots & \row{A}_m \cdot \col{B}_m
    } 
}

The matrix-vector product $A\mb{x}$ is the weighted sum of the columns of $A$ ie
\eq{
    A\mb{x} = \sum x_i \col{A}_i
}
The set of all possible combinations of the columns defines the *column space* of A.  This idea
is couple to the idea of a *vector space*.  More specifically, the *vector space* generated by a 
set of $n$ vectors $\left\{\mb{u}_1,\hdots,\mb{u}_n\right\}$ is the set of all possible linear
combinations of the $\mb{u}_i$.  If we combine these vectors to form the columns of a matrix $A$,
the we get the linear combination from the above matrix-vector product $A\mb{x}$.  The column 
space of a matrix $A$ then is the *vector space* generated by its columns.  This *vector space*,
which is the *column space* of $A$ is often also called the *range* of $A$, ie
\eq{
    \textrm{range}\left(A\right) = \left\{ \textrm{all possible} A\mb{x}_i \right\}
}
since it defines which vectors can be produced by summing the columns in $A$.  The *null space*
is a particular set of non-zero weights $\mb{x}$ which produce the zero vector, ie
\eq{
    \textrm{null}\left(A\right) = \left\{ \textrm{all} \mb{x}_i \textrm{where} A\mb{x}_i=0. \right\}
}
If such an $\mb{x}_i$ exists, then one column can be written as a linear combination of at 
least one of the others.  This has significant implications - see below.  If no such combination
exists, then the columns are *linearly independant* - one cannot be written as a combination
of the others.  This concept is again common to sets of vectors - ie a set of vectors is
linearly independant if no linear combination of the vectors produces a zero vector.  Spaces
which are *not* linearly independant include redundant vectors.  

Consider a matrix $A$ and some set of vectors $\rowx{u}{n}$.  For every possible combination
of the columns of $A$ (ie every $v = A\mb{x}$), it may be possible to choose some combination
of the vectors $\mb{u}_i$ that produces the same $v$, and vica versa.  In this sense, the set of 
vectors$\rowx{u}{n}$ can substitute for the matrix $A$, since they can both generate the exact same
output (ie they have the same range).  If we want to do this substitution, we shouldnt choose any 
redundant vectors in the $\mb{u}_i$ - we should only use *linearly independant* vectors.  Such
a set of vectors is said to form a *basis* for $A$.

## Non-Singular Matrices

Consider the inverse problem.  Is it possible to 