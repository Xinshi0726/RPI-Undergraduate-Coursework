/* fork-buffering.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  printf( "GOOD MORNING\n" );

  /* Running this in the terminal, the above line is shown once
   *  (because '\n' flushes the stdout buffer)
   *
   * Redirecting this output to a file...
   *
   *   bash$ ./a.out > output.txt
   *
   * ...changes stdout to be block-based (or fully) buffered
   */

  /* When we call fork(), the stdout buffer is copied from
   *  the parent process to the child process
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
  }
  else /* rc > 0 */
  {
    /* PARENT PROCESS */
    usleep( 35 );
    printf( "PARENT: my child process PID is %d.\n", rc );
    printf( "PARENT: my PID is %d.\n", getpid() );
  }

  /* This one line of output below occurs twice, once in the parent,
   *  once in the child process...
   *
   * TO DO: revise the diagram below with this new line of output
   */
  printf( "My PID is %d and I'm terminating...\n", getpid() );

  usleep( 10000 );  /* <== add this so that the bash prompt delays printing */

  /* If a parent process terminates before a child process,
   *  then the child process (orphan) is adopted by top-level PID 1
   */

  return EXIT_SUCCESS;
}


/* If we redirect to output.txt, only two possible outputs
 *  due to buffering:
 *
 * goldsd@linux:~/s23/csci4210$ ./a.out > output.txt ; cat output.txt
 * GOOD MORNING
 * CHILD: happy birthday to me!  My PID is 3836713.
 * CHILD: my parent's PID is 3836712.
 * My PID is 3836713 and I'm terminating...
 * GOOD MORNING
 * PARENT: my child process PID is 3836713.
 * PARENT: my PID is 3836712.
 * My PID is 3836712 and I'm terminating...
 * goldsd@linux:~/s23/csci4210$ ./a.out > output.txt ; cat output.txt
 * GOOD MORNING
 * PARENT: my child process PID is 3837070.
 * PARENT: my PID is 3837068.
 * My PID is 3837068 and I'm terminating...
 * GOOD MORNING
 * CHILD: happy birthday to me!  My PID is 3837070.
 * CHILD: my parent's PID is 3837068.
 * My PID is 3837070 and I'm terminating...
 * goldsd@linux:~/s23/csci4210$ 
 *
 *
 * What are all possible outputs for this code?
 *
 *  TO DO: fix the diagram below to match the code above...
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








