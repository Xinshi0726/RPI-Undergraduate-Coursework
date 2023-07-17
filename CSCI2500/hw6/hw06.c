/* 
HW 6: Cache Simulator
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

// maximum size of the input
#define MAX_ADDRESSES 1024
#define CACHE_SIZE 256

/******************************************************************************/
/* Just doing everything in our main boi */
/******************************************************************************/
int main(int argc, char** argv)
{
  // read associativity and set cache parameters
  int assoc = atoi(argv[1]);
  int set_size = 256/assoc;
  printf("%s%d\n","Cache size: ", 256);
  printf("%s%d\n","Cache associativity: ",assoc);
  printf("%s%d\n", "Cache sets: ",set_size);
  // read memory addresses
  int count = 0;
  char str[256];
  // allocate memory for cache structure
  int cache[256];
  for (int i = 0; i<256; i++)
  {
      cache[i] = 0;
  }
  int table[256];
  for (int i = 0; i<256; i++)
  {
      table[i] = -1;
  }

  // run the simulation
  int hits = 0;
  int misses = 0;
  int flag = 0;
  while (fgets(str, 256, stdin) != NULL) {
        int address = atoi(str);
        flag = 0;
        count+=1;
        for (int i = 0; i<assoc;i++)
        {
            if (table[address%set_size+i] == address)
            {
               flag = 1;
               cache[address%set_size+i] += 1;
               break;
            }
        }
        if (flag == 0)
        {
            int min = 99999;
            int select = 0;
            for (int i = 0; i<assoc; i++)
            {
                if (cache[address%set_size+i] < min)
                {
                    min = cache[address%set_size+i];
                    select = i;
                }
                cache[address%set_size+select] = 0;
                table[address%set_size+select] = address;
            }
            printf("%d %s",address,"(miss)\n");
        }
        else{
            printf("%d %s",address,"(hit)\n");
        }
        hits += flag;
        misses += (1-flag);
  }

    // output final statistics
  printf("Cache accesses: %d\n", count);
  printf("Cache hits: %d\n", hits);
  printf("Cache misses: %d\n", misses);
  printf("Overall hit rate: %f\n", ( hits + 0.0 ) / count);

  // clean up memory

  return 0;
}
