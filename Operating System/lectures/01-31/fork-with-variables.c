/* fork-with-variables.c --- pass-by-fork */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  int x = 5;
/*  int * y = &x; */
  int * z = calloc( 20, sizeof( int ) );
  *(z + 3) = 12;

  /* When we call fork(), all memory gets duplicated and
   *  copied into the child process memory image
   *
   * fork() will attempt to create a new process by
   *  duplicating/copying the memory image of the
   *   existing running process (parent)
   *
   * this includes all statically and dynamically allocated memory,
   *  i.e., x, y, z, rc, etc.
   */

  /* create a new process */
  pid_t rc = fork();

  if ( rc == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  if ( rc == 0 )
  {
    /* CHILD PROCESS */
    printf( "CHILD: happy birthday to me!  My PID is %d.\n", getpid() );
    printf( "CHILD: my parent's PID is %d.\n", getppid() );

    x += 12;
    printf( "CHILD: x is now %d\n", x );
  }
  else /* rc > 0 */
  {
    /* PARENT PROCESS */
    usleep( 35 );
    printf( "PARENT: my child process PID is %d.\n", rc );
    printf( "PARENT: my PID is %d.\n", getpid() );

    x += 120;
    printf( "PARENT: x is now %d\n", x );
  }

  /* This one line of output below occurs twice, once in the parent,
   *  once in the child process...
   *
   * TO DO: revise the diagram below with this new line of output
   */
  printf( "My PID is %d and I'm terminating...\n", getpid() );


  /* Both parent and child processes end up here...
   *  ...so both parent and child processes must free their
   *      respective dynamically allocated memory
   */
  free( z );

  usleep( 10000 );  /* <== add this so that the bash prompt delays printing */

  /* If a parent process terminates before a child process,
   *  then the child process (orphan) is adopted by top-level PID 1
   */

  return EXIT_SUCCESS;
}


/*
 * goldsd@linux:~/s23/csci4210$ ./a.out 
 * PARENT: my child process PID is 2072537.
 * PARENT: my PID is 2072536.
 * CHILD: happy birthday to me!  My PID is 2072537.
 * CHILD: my parent's PID is 2072536.
 * goldsd@linux:~/s23/csci4210$ ./a.out 
 * PARENT: my child process PID is 2073028.
 * PARENT: my PID is 2073026.
 * CHILD: happy birthday to me!  My PID is 2073028.
 * CHILD: my parent's PID is 2073026.
 *
 *
 * What are all possible outputs for this code?
 *
 *                              rc = fork()
 *                               /      \
 *                              /        \
 *                             /          \
 *                       <PARENT>        <CHILD>
 * PARENT: my child process PID...      CHILD: happy birthday to me! ...
 * PARENT: my PID is ...                CHILD: my parent's PID is ...
 *
 * (1) lines shown above in the <PARENT> section interleave with
 *      the lines shown in the <CHILD> section
 *
 * (2) lines shown above in the <PARENT> section occur in that given order;
 *      same for <CHILD> section
 *
 *
 * goldsd@linux:~/s23/csci4210$ ./a.out 
 * CHILD: happy birthday to me!  My PID is 2079145.
 * PARENT: my child process PID is 2079145.
 * PARENT: my PID is 2079144.
 * My PID is 2079144 and I'm terminating...
 * CHILD: my parent's PID is 2079144.
 * My PID is 2079145 and I'm terminating...
 * goldsd@linux:~/s23/csci4210$ 
 * goldsd@linux:~/s23/csci4210$ 
 * goldsd@linux:~/s23/csci4210$ 
 * goldsd@linux:~/s23/csci4210$ ./a.out 
 * PARENT: my child process PID is 2079331.
 * PARENT: my PID is 2079329.
 * My PID is 2079329 and I'm terminating...
 * CHILD: happy birthday to me!  My PID is 2079331.
 * CHILD: my parent's PID is 2079329.
 * My PID is 2079331 and I'm terminating...
 *
 */








