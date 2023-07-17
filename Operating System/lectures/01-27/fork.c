/* fork.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  /* create a new process */
  int a[] = {1,2,3,5};
  pid_t rc = fork();
  if ( rc == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  if ( rc == 0 )
  {
    /* CHILD PROCESS */
    a[3] = 4;
    printf("CHILD: a is %d\n",a[3]);
    printf( "CHILD: happy birthday to me!  My PID is %d.\n", getpid() );
    printf( "CHILD: my parent's PID is %d.\n", getppid() );
    sleep(10);
  }
  else /* rc > 0 */
  {
    /* PARENT PROCESS */
    usleep( 35 );
    int status;
    waitpid( rc, &status, 0 );
    printf("PARENT: a is %d\n",a[3]);
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
