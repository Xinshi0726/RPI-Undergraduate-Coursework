/* dynamic-allocation.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  char * path = malloc( 16 );  /* dynamically allocate 16 bytes
                                   on the runtime heap */
  if ( path == NULL )
  {
    perror( "malloc() failed" );
    return EXIT_FAILURE;
  }

  char * path2 = malloc( 16 );

  if ( path2 == NULL )
  {
    perror( "malloc() failed" );
    return EXIT_FAILURE;
  }


  printf( "sizeof path is %lu\n", sizeof( path ) );

  strcpy( path, "/cs/goldsd/s23/" );
  printf( "path is \"%s\" (strlen is %lu)\n", path, strlen( path ) );

  strcpy( path2, "AAAAAAAAAAAAAAAA" );  /* one-byte overflow/overrun */
  printf( "path2 is \"%s\" (strlen is %lu)\n", path2, strlen( path2 ) );

  strcpy( path2, "/cs/goldsd/s23/blah/BLAH/blAh/blaH/meme" );
  printf( "path is \"%s\" (strlen is %lu)\n", path, strlen( path ) );
  printf( "path2 is \"%s\" (strlen is %lu)\n", path2, strlen( path2 ) );

  free( path );
  free( path2 );

  return EXIT_SUCCESS;
}