/* buffering.c */

/* When printing to the terminal (shell) via stdout (fd 1),
 *  a '\n' character will "flush" the stdout buffer,
 *   i.e., output everything that has been stored in
 *    the stdout buffer so far
 *
 * ==> this is called line-based buffering
 *
 * TO DO: add a '\n' to the end of each debugging printf()
 *
 * When we instead output fd 1 to a file...
 *
 *  bash$ ./a.out > output.txt
 *
 *  ...the '\n' character no longer flushes the stdout buffer
 *
 * ==> this is called block-based buffering or
 *      this is referred to as fully buffered
 *
 *
 * A third type of buffering is non-buffered, which is what
 *  is used for stderr (fd 2)
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
#if 0
  setvbuf( stdout, NULL, _IONBF, 0 );
#endif

  fprintf( stderr, "here0" );
  printf( "HERE0" );
  int * x = NULL;

  fprintf( stderr, "here1" );
  printf( "HERE1" );
  *x = 1234;
  fprintf( stderr, "here2" );
  printf( "HERE2" );

  return EXIT_SUCCESS;
}
