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
  int x = 5;   /* x is statically allocated (on the stack) */
               /*   (4 bytes are allocated on the stack)   */

  printf( "x is %d\n", x );
  printf( "sizeof( int ) is %lu bytes\n", sizeof( int ) );
  printf( "sizeof( x ) is %lu bytes\n", sizeof( x ) );

  int * y = NULL;  /* y is statically allocated (on the stack) */
  printf( "sizeof( int* ) is %lu bytes\n", sizeof( int* ) );
  printf( "sizeof( y ) is %lu bytes\n", sizeof( y ) );

#if 0
  /* seg-fault! */
  printf( "y points to %d\n", *y );
#endif

  y = &x;  /* & is the address-of operator */
  printf( "y points to %d\n", *y );

  printf( "sizeof( int* ) is %lu bytes\n", sizeof( int* ) );
  printf( "sizeof( char* ) is %lu bytes\n", sizeof( char* ) );
  printf( "sizeof( double* ) is %lu bytes\n", sizeof( double* ) );
  printf( "sizeof( void* ) is %lu bytes\n", sizeof( void* ) );

  printf( "sizeof( int ) is %lu bytes\n", sizeof( int ) );
  printf( "sizeof( char ) is %lu bytes\n", sizeof( char ) );
  printf( "sizeof( double ) is %lu bytes\n", sizeof( double ) );
  printf( "sizeof( void ) is %lu bytes\n", sizeof( void ) );

  return EXIT_SUCCESS;
}