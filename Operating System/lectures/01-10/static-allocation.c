/* static-allocation.c */

/* compile from terminal/shell as follows:
 *
 *   bash$ gcc -Wall -Werror static-allocation.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  printf( "sizeof( int ) is %lu bytes\n", sizeof( int ) );
  printf( "sizeof( int* ) is %lu bytes\n", sizeof( int* ) );
    printf( "sizeof( char ) is %lu bytes\n", sizeof( char ) );
    printf( "sizeof( char* ) is %lu bytes\n", sizeof( char* ) );
    printf( "sizeof( float ) is %lu bytes\n", sizeof( float ) );
    printf( "sizeof( float* ) is %lu bytes\n", sizeof( float* ) );
    printf( "sizeof( double ) is %lu bytes\n", sizeof( double ) );
    printf( "sizeof( double* ) is %lu bytes\n", sizeof( double* ) );

  return EXIT_SUCCESS;
}