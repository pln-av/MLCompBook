# Testing and examples of defined macros

Some quick notes on my macros.

Command | Effect
--------|---------
`\eq{ ... }` | `\begin{equation} .... \end{equation}`
`\mat{1&2\\4&5}` | $\mat{1&2\\3&4 }$
`\matx{A}{m}{n}` | $\matx{A}{m}{n}$
`\rowx{A}{n}` | $\rowx{A}{n}$
`\colx{A}{m}` | $\colx{A}{m}$
`\row{A}_i` | $\row{A}_i$
`\col{A}_j` | $\col{A}_j$

A matrix is a $3$x$3$ grid
\eq{
 A = \mat{ 1 & 2 & 3 \\ 4 & 5 & 6 \\ 7 & 8 & 9}
}
but now I can write 
\eq{
A = \matx{a}{m}{n}
}
or for 3x3 \eq{ \matt{a} }.  Finally, I need to define column and row operators.  How about this
\eq{
    \row{A}_i = \matrow{A}{i}{n}
}
\eq{
    \col{A}_j = \matcol{A}{j}{m}
}