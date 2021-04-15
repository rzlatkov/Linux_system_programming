# gcc -o execfilename main.c calc_sum.c
#!/bin/bash
echo "compiling..."
gcc -c calc_sum.c
gcc -c main.c
echo "obj files created"
gcc -o $1 calc_sum.o main.o
echo "file type"
file $1
