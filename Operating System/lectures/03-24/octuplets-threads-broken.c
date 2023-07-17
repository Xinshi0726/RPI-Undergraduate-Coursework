/* octuplets-threads-broken.c */

/* Run this code to see what happens.....
 *
 * We will usually see all child threads get 16 as their input argument
 *  (to the whattodo() function) -- this occurs because each thread
 *   gets a pointer to the same statically allocated memory for
 *    variable t declared in main()
 *
 * Consider this an "anti-pattern" -- i.e., what not to do when
 *  creating multi-threaded code using pthreads in C....
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CHILDREN 8

/* this function is called by each thread */
void * whattodo( void * arg );
        /*       ^^^^^^
         *  the thread code accepts an arbitrary pointer,
         *   which gives us the flexibility to pass anything
         *    we want to the thread code
         */

int main()
{
  srand( time( NULL ) * getpid() );

  pthread_t tid[CHILDREN];    /* keep track of thread IDs */

  int i, rc, children = CHILDREN;

  for ( i = 0 ; i < children ; i++ )
  {
    int t = 10 + ( rand() % 21 );  /* [10,30] seconds */

    printf( "MAIN: next child thread will nap for %d seconds\n", t );

    rc = pthread_create( &tid[i], NULL, whattodo, &t );

    if ( rc != 0 )
    {
      fprintf( stderr, "pthread_create() failed (%d)\n", rc );
    }
  }

  /* wait for the child threads to terminate */
  for ( i = 0 ; i < CHILDREN ; i++ )
  {
    pthread_join( tid[i], NULL );
    printf( "MAIN: joined a child thread\n" );
  }

  printf( "MAIN: all child threads successfully joined\n" );

  return EXIT_SUCCESS;
}


void * whattodo( void * arg )
{
  int t = *(int *)arg;

  printf( "I'm going to nap for %d seconds...\n", t );
  sleep( t );
  printf( "I'm awake!\n" );

  return NULL;
}












