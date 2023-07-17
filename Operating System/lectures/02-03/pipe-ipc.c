/* pipe-ipc.c */

/* A "pipe" is a unidirectional communication channel -- man 2 pipe */
/*               ^^^                                                */

/* TO DO: Draw a diagram that shows all possible outputs for this code */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int pipefd[2];  /* array to hold the two pipe (file) desciptors:
                     pipefd[0] is the "read" end of the pipe
                     pipefd[1] is the "write" end of the pipe */

  /* create a pipe */
  int rc = pipe( pipefd );  /* the input argument pipefd will be
                               populated with the OS-assigned
                               read and write pipe/file descriptors */

  if ( rc == -1 )
  {
    perror( "pipe() failed" );
    return EXIT_FAILURE;
  }

  /* fd table:

     0: stdin
     1: stdout
     2: stderr                   +--------+
     3: pipefd[0] <====READ======| buffer | think of this buffer as a
     4: pipefd[1] =====WRITE====>| buffer |  temporary hidden file...
                                 +--------+
   */

  printf( "Created pipe: pipefd[0] is %d and pipefd[1] is %d\n",
          pipefd[0], pipefd[1] );


  pid_t p = fork();  /* fork() will copy the fd table to the child */

  if ( p == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  /* fd table (immediately after fork() is called):

     [PARENT]                                          [CHILD]
     0: stdin                                          0: stdin
     1: stdout                                         1: stdout
     2: stderr                   +--------+            2: stderr
     3: pipefd[0] <====READ======| buffer |===READ===> 3: pipefd[0]
     4: pipefd[1] =====WRITE====>| buffer |<==WRITE=== 4: pipefd[1]
                                 +--------+
   */

  /* fd table (after closing the unused ends of the pipe in parent/child

     [PARENT]                                          [CHILD]
     0: stdin                                          0: stdin
     1: stdout                                         1: stdout
     2: stderr                   +--------+            2: stderr
     3: pipefd[0] <====READ======| buffer |            3:
     4: pipefd[1]                | buffer |<==WRITE=== 4: pipefd[1]
                                 +--------+
   */

  if ( p == 0 )   /* CHILD process */
  {
#if 1
    close( pipefd[0] );   /* close the "read" end of the pipe */
#endif

    /* write data to the pipe */
    int bytes_written = write( pipefd[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26 );
/*----CONTEXT SWITCH from CHILD process to PARENT process------------------*/
    printf( "CHILD: Wrote %d bytes to the pipe\n", bytes_written );

#if 0
    /* read data from the pipe */
    char buffer[20];
    int bytes_read = read( pipefd[0], buffer, 10 );
    buffer[bytes_read] = '\0';
    printf( "CHILD: Read %d bytes: \"%s\"\n", bytes_read, buffer );

    close( pipefd[0] );   /* close the "read" end of the pipe */
#endif
  }
  else /* p > 0    PARENT process */
  {
    close( pipefd[1] );   /* close the "write" end of the pipe */

    /* read data from the pipe */
    char buffer[20];
    int bytes_read = read( pipefd[0], buffer, 10 );
//    buffer[bytes_read] = '\0';
    printf( "PARENT: Read %d bytes: \"%s\"\n", bytes_read, buffer );

#if 0
    usleep( 10 );
#endif

    bytes_read = read( pipefd[0], buffer, 10 );
//    buffer[bytes_read] = '\0';
    printf( "PARENT: Read %d bytes: \"%s\"\n", bytes_read, buffer );

    bytes_read = read( pipefd[0], buffer, 10 );
//    buffer[bytes_read] = '\0';
    printf( "PARENT: Read %d bytes: \"%s\"\n", bytes_read, buffer );

  }

  usleep( 1000 );

  return EXIT_SUCCESS;
}
