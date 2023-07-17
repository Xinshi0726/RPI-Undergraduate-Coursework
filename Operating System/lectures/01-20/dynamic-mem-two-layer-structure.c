/* dynamic-mem-two-layer-structure.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  /* array of strings (i.e., array of char arrays) */

  char ** names;   /* ... array of char* ... */
                   /* same as: char * names[]; */


#if 0
  names = malloc( 47 * sizeof( char * ) );    /* <== uninitialized */
#endif
  names = calloc( 47, sizeof( char * ) );

  names[2] = calloc( 5, sizeof( char ) );
  strcpy( names[2], "Finn" );
  printf( "Hi, %s\n", names[2] );

#if 0
  /* memory leak */
  names[2] = NULL;
  names[2] = calloc( ... );
  /* DON'T DO THE ABOVE... */
#endif

  /* use realloc() to expand the size of names[2] */
  names[2] = realloc( names[2], 14 * sizeof( char ) );
  strcat( names[2], " & Basil" );
  printf( "Hi again, %s\n", names[2] );


  printf( "==> %c\n", names[2][3] );
  printf( "==> %c\n", *( *(names + 2) + 3 ) );

  free( names[2] );
  free( names );

  return EXIT_SUCCESS;
}

#if 0
                 char*
               +------+
  names---> [0]| NULL |
               +------+
            [1]| NULL |    [0] [1] [2] [3]
               +------+   +--------------------+
            [2]|   =====> | F | i | n | n | \0 |
               +------+   +--------------------+
                 ....
               +------+
           [46]| NULL |
               +------+

#endif
