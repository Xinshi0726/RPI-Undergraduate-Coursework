#!/bin/sh

/usr/bin/rm -f hw2.out
/usr/bin/gcc -Wall -Werror -g -o hw2.out *.c

/usr/bin/rm -f hw2-no-parallel.out
/usr/bin/gcc -Wall -Werror -g -D NO_PARALLEL -o hw2-no-parallel.out *.c

/usr/bin/rm -f hw2-quiet.out
/usr/bin/gcc -Wall -Werror -g -D QUIET -o hw2-quiet.out *.c

/usr/bin/rm -f hw2-quiet-no-parallel.out
/usr/bin/gcc -Wall -Werror -g -D QUIET -D NO_PARALLEL -o hw2-quiet-no-parallel.out *.c

# NO_PARALLEL
./hw2-no-parallel.out 3 3 0 0 > hw2-output09-original.txt

# parallel
./hw2.out 3 3 0 0 > hw2-output12-original.txt

# NO_PARALLEL
./hw2-no-parallel.out 3 4 1 1 > hw2-output10-original.txt

# NO_PARALLEL
./hw2-no-parallel.out 3 4 1 0 > hw2-output11-original.txt

# QUIET
./hw2-quiet-no-parallel.out 3 10 0 0 > hw2-output13-original.txt

