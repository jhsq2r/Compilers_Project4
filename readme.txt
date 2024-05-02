Author: Jarod Stagner
Date: 05/02/2024

This program is designed to take input as shown below
./Project4 [file]
The input file should contain code for the program to parse
First, it will remove comments and tokenize the file
Second, it will parse according to our languages grammar
  and produce a parse tree
Third, it will go through a staticSemantics function that will output
  any error that has to do with scoping of variables
Lastly, it will output ASM code to the console.

Note: Due to the projects building on one another and the mistakes
I've made in the other projects, this project does not work to the fullest.
I think it has something to do with how I edited the grammar, but I had
difficulty generating working code for the stack operatings. While the peices work,
they dont fix together properly.

My code works on all code examples given in on canvas becides the last one. However, 
scoping is still correct: global and local variables are working as intended.

The code doesn't work on the 4th testcase just because of one extra ADD but
if that ADD is removed it will work.

This is a full credit attempt: local option

Repos: https://github.com/jhsq2r/Compilers_Project4