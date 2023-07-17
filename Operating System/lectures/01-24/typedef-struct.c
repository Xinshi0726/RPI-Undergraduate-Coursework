/* typedef-struct.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
  unsigned int rin;   /* e.g., 660000001 */
  char rcsid[16];     /* e.g., "goldsd3" */
  /* ... */
}
student_t;


int main()
{
  printf( "sizeof student_t is %lu\n", sizeof( student_t ) );

  /* static allocation */
  student_t me;
  me.rin = 660000001;
  strcpy( me.rcsid, "goldsd3" );

  /* dynamic allocation */
  student_t * st;  /* sizeof( student_t * ) == 8 */
  st = malloc( sizeof( student_t ) );  /* or calloc() */
             /* ^^^^^ 20 */

  (*st).rin = 661234567;
  strcpy( (*st).rcsid, "smithj385" );

  printf( "me: RIN %u; RCSID %s\n", me.rin, me.rcsid );
  printf( "st: RIN %u; RCSID %s\n", st->rin, st->rcsid );

  free( st );


  /* dynamically allocate an array of student_t of size 6800 */
  /*  (allocates 6800 x 20 bytes on the runtime heap...)     */
  student_t * students = calloc( 6800, sizeof( student_t ) );

  students[500] = me;
  *(students+500) = me;

  printf( "students[500]: RIN %u; RCSID %s\n",
          students[500].rin, students[500].rcsid );

  me.rcsid[2] = '!';

  printf( "me: RIN %u; RCSID %s\n", me.rin, me.rcsid );
  printf( "students[500]: RIN %u; RCSID %s\n",
          students[500].rin, students[500].rcsid );

  free( students );

  return EXIT_SUCCESS;
}









