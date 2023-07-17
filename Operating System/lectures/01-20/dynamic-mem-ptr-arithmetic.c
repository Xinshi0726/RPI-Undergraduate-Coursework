/* dynamic-mem-ptr-arithmetic.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  /* dynamically allocate 100 bytes (on the heap) */
  char * s = malloc( 100 );

  /* dynamically allocate 100 bytes (on the heap) */
  char * t = calloc( 100, sizeof( char ) );
    /* calloc() will initialize these 100 bytes */
    /*   to be all zero bytes, i.e., '\0'       */

  printf( "\"%s\" \"%s\"\n", s, t );

  s[0] = 'A';
  s[1] = 'B';
  s[2] = 'C';
  s[3] = '\0';
  s[20] = 'Z';
  s[21] = 'Y';
  s[22] = 'X';
  s[23] = '\0';

  printf( "\"%s\" \"%s\"\n", s, t );

  printf( "\"%s\" \"%s\"\n", &s[20], t );
  printf( "\"%s\" \"%s\"\n", s + 20, t );
  printf( "\"%s\" \"%s\"\n", s + 22, t );

  free( s );
  free( t );


  int * v = calloc( 1, sizeof( int ) );
  *v = 1234;
  printf( "v points to %d\n", *v );
  free( v );


  /* dynamically allocate memory for an array of integers of size 47 */
  int * numbers = calloc( 47, sizeof( int ) );

  numbers[18] = 1234;
  *(numbers + 18) = 1235;
 /* ^^^^^^^^^^^^ */
 /*  numbers + ( 18 x sizeof(int) ) == numbers + 72  */
 /*   (NEVER TYPE THIS...!) */

  printf( "%d\n", numbers[18] );
  printf( "%d\n", *(numbers + 18) );

  printf( "%d\n", *(numbers + 400) );
  printf( "%d\n", *(numbers + 4000) );
  printf( "%d\n", *(numbers + 40000) );

  free( numbers );

  return EXIT_SUCCESS;
}








