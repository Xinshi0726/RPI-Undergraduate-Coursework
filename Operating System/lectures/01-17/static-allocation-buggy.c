/* static-allocation-buggy.c */

/* compile from terminal/shell as follows:
 *
 *   bash$ gcc -Wall -Werror static-allocation-buggy.c
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


                           /*       +------------------------+ */
  char name[5] = "David";  /* name: | D | a | v | i | d | \0 | */
                           /*       +------------------------+ */

  printf( "Hi, %s\n", name );

  char xyz[5] = "QRSTU";
  printf( "Hi again, %s\n", name );
  printf( "xyz is %s\n", xyz );

  /* correct the bugs above by allocating [6] instead of [5] */


  char * cptr = "ABCdEFGHIJKLMNOPQRSTUVWXYZ";  /* implied '\0' at the end */

#if 0
  char cptr[] = "ABCdEFGHIJKLMNOPQRSTUVWXYZ";  /* implied '\0' at the end */
#endif

  printf( "cptr points to \"%s\"\n", cptr );

  cptr[3] = 'D';
  /* seg-fault occurs here because cptr points to read-only memory */

  printf( "cptr points to \"%s\"\n", cptr );

  /* to do: try replacing char * cptr with a more specific
   *         static memory allocation ---- char cptr[27];
   */


  return EXIT_SUCCESS;
}