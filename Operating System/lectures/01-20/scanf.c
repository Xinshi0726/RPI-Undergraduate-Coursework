/* scanf.c */

/* compile from terminal/shell as follows:
 * bash$ gcc -Wall -Werror scanf.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main()
{                  /*       [0]1 2 3 4  ...[31] */
                   /*       +-----------------+ */
  char name[32];   /* name: | | | | | | ... | | */
                   /*       +-----------------+ */

  printf( "name: %s\n", name );

  printf( "Enter your name: " );
#if 0
  scanf( "%s", name );
#endif
  scanf( "%31s", name );
  printf( "Hello %s\n", name );

  /*
   *      char * name;
   *      scanf( "%ms", &name );      <== does this work on a Mac?
   *       ...
   *      free( name );
   */

  float x;

  printf( "Enter a number: " );
  scanf( "%f", &x );   /* & is the address-of operator */
  printf( "The square root of %f is %f\n", x, sqrt( x ) );


  return EXIT_SUCCESS;
}
