/* fd-write-redirect.c */

/* TO DO: write a corresponding fd-read.c example that reads in */
/*         the outfile.txt file and re-populates a variable of  */
/*          type student_t (also the int and short variables)   */

/* bash$ hexdump -C outfile.txt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct
{
  unsigned int rin;  /* e.g., 660000001 */
  char rcsid[16];    /* e.g., "goldsd3" */
}
student_t;


int main()
{
  close( 1 );   /* close file descriptor 1 (stdout) */

  /* fd table:
       0 stdin
       1 
       2 stderr
   */

  /* note that this close(1) also stops the line-based buffering */


  char * name = "outfile.txt";

                   /* attempt to open this file:
                       O_WRONLY for writing
                       O_CREAT create the file if necessary
                       O_TRUNC truncate the file if it already exists
                              (i.e., erase the contents, set size to 0)
                       (also O_APPEND) */

  /* (the file that is opened will be block-based buffering) */

  int fd = open( name, O_WRONLY | O_CREAT | O_TRUNC, 0660 );
             /*                                      ^^^^
                                                      |
                                      leading 0 means this is in octal
                                                               (base 8)
                0660 ==> 110 110 000
                         rwx rwx rwx
                         ^^^ ^^^ ^^^
                          |   |   |
                          |   |  no permissions for other/world
                          |   |
                          | rw for group permissions
                          |
                     rw for file owner */

  printf( "fd is %d\n", fd );

  if ( fd == -1 )
  {
    perror( "open() failed" );
    return EXIT_FAILURE;
  }

  /* fd table:
       0 stdin
       1 outfile.txt (O_WRONLY)
       2 stderr
   */

  char buffer[20];
  sprintf( buffer, "ABCD%02dEFGH", fd );
  int rc = write( fd, buffer, strlen( buffer ) );
  printf( "wrote %d bytes to fd %d\n", rc, fd );

  rc = write( fd, buffer, 3 );
  printf( "wrote %d bytes to fd %d\n", rc, fd );

  /* write some binary data to the file... */
  int important = 32768;
  rc = write( fd, &important, sizeof( int ) );
  printf( "wrote %d bytes to fd %d\n", rc, fd );

  short q = 0xfade;   /* in memory (little endian): de fa */
  rc = write( fd, &q, sizeof( short ) );
  printf( "wrote %d bytes to fd %d\n", rc, fd );

  student_t s1;
  s1.rin = 660000001;
  strcpy( s1.rcsid, "goldsd3" );
  rc = write( fd, &s1, sizeof( student_t ) );
  printf( "wrote %d bytes to fd %d\n", rc, fd );

  /* at this point, all of the printf() output has been appended to
   *  a buffer that is associated with stdout (fd 1); the fflush()
   *   call will force the data in the buffer to be written to the
   *    outfile.txt output file
   */

#if 1
  fflush( stdout );   /* flush the stdout output buffer */
#endif

  close( fd );

  return EXIT_SUCCESS;
}






