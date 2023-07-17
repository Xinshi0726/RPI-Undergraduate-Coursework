/* exp-random.c */

/* Instead of using a uniform distribution for process arrival
 *  times, we use an exponential distribution, which should
 *   better model the real-world system...
 *
 * We use what is called a Poisson process
 *
 * And we will assume an M/M/1 queue
 *
 * Randomly generated values are to be the times between
 *  process arrivals, i.e., interarrival times
 *
 * Events occur continuously and independently, but we have
 *  arrivals occurring at a constant average rate
 *                          ^^^^^^^^^^^^^^^^^^^^^
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

int main()
{
  /* use a hard-coded value below instead of the current timestamp
   *  to generate the same pseudo-random number sequence each time
   *   you run this code
   */
  srand48(1024);

                          /* uniform to exponential distribution: */
                          /*                                      */
  double min = 0;         /*        -ln(r)                        */
  double max = 0;         /*  x = ----------                      */
  double sum = 0;         /*        lambda                        */
                          /*                                      */
  double lambda = 0.001;  /* average should be 1/lambda ===> 1000 */

  int iterations = 10000000;  /* <== make this number very large */

  for ( int i = 0 ; i < iterations ; i++ )
  {
    double r = drand48();  /* uniform dist [0.00,1.00) -- also see random() */

    /* generate the next pseudo-random value x */
    double x = -log( r ) / lambda;   /* log() is natural log (see man page) */

#if 0
    /* skip values that are far down the "long tail" of the distribution */
    if ( x > 3000 ) { i--; continue; }

    /* TO DO: Since adding the above line of code will lower the
     *         resulting average, try to modify lamdba to get back
     *          to an average of 1000
     */
#endif

    /* display the first 20 pseudo-random values */
    if ( i < 20 ) printf( "x is %lf\n", x );

    sum += x;
    if ( i == 0 || x < min ) { min = x; }
    if ( i == 0 || x > max ) { max = x; }
  }

  double avg = sum / iterations;

  printf( "minimum value: %lf\n", min );
  printf( "maximum value: %lf\n", max );
  printf( "average value: %lf\n", avg );
}