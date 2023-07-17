/* 
Lab 7: Insane in the cache-brain
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************/
/* Checkpoint 1  */
/******************************************************************************/
/* Given a range of bit index values (i.e., bit positions 0..31),
 * which is an inclusive range, this function determines and
 * returns the number of blocks in the cache
 *
 * e.g., given the example address below, an idx_start of 4
 *       and an idx_end of 11 would return 2^8 or 256
 *
 *    2    1    3    5    C    6    9    0  <== hex
 * 0010 0001 0011 0101 1100 0110 1001 0000  <== binary
 *                          ^       ^
 *                         end    start
 */
unsigned block_count(unsigned idx_start, unsigned idx_end)
{
  // TODO: Calculate the block count 
  unsigned count = 0;
  count = pow(2,idx_end-idx_start+1);

  return count;
}


/******************************************************************************/
/* Checkpoint 2  */
/******************************************************************************/
/* Given an address and range of bits in the index
 * (i.e., start-to-end, inclusive), return the index
 *
 * e.g., given the address below, an idx_start of 4,
 *       and an idx_end of 11
 *
 *    2    1    3    5    C    6    9    0  <== hex
 * 0010 0001 0011 0101 1100 0110 1001 0000  <== binary
 *                          ^       ^
 *                         end    start
 *
 * this function should return 0110 1001 in binary
 * or 0x69 in hex or 105 decimal
 */
void decimalToBinary(unsigned long long int N, int* immediate, int length) {
    // To store the binary number
    unsigned long long int B_Number = 0;
    int cnt = 0;
    while (N != 0) {
        int rem = N % 2;
        unsigned long long int c = pow(10, cnt);
        B_Number += rem * c;
        N /= 2;

        // Count used to store exponent value
        cnt++;
    }
    for (int i = 0; i < length; i++) {
        immediate[i] = B_Number % 10;
        B_Number /= 10;
    }
}
unsigned get_index(unsigned address, unsigned idx_start, unsigned idx_end)
{
  // TODO: Calculate the integer value of the index
  // Hint: you'll be implicitly converting from integer->binary->integer 
  unsigned index = 0;
  int immediate[32];
  decimalToBinary(address,immediate,32);
  for (int i = 0; i<=idx_end-idx_start; i++)
  {
      index += pow(2,i)*(immediate[idx_start+i]);
  }
  return index;
}


/******************************************************************************/
/* Checkpoint 3  */
/******************************************************************************/
/* Utilizing slides 22-32 of Chapter 5 as a reference,
 * calculate the total bits in the cache block,
 * including data, tag, and valid bits.
 * Assume 32-bit addressing.
 *
 * The bob argument should be the leftmost bit number
 * of the block-offset bits (BOB); e.g., a 3 here means
 * bits 0-3 (inclusive) are used for the BOB.
 *
 * e.g., in calling get_cache_block_size( 2, 16, 1 ),
 *       we would first determine that we need 15 bits
 *       for the index (bits 2..16, inclusive).  Next,
 *       we have two block-offset bits (bits 0..1, inclusive).
 *       Given this, we next determine that the tag consists
 *       of 32-15-2=15 bits; therefore, the block size is:
 *
 *                  1 validity bit
 *                 15 bits (tag) 
 *               + 32 bits (data)
 *              ------------------------
 *                 48 bits (block size)
 */
unsigned get_cache_block_size(unsigned idx_start, unsigned idx_end, 
  unsigned bob)
{
  // TODO: calculate cache block size
  unsigned size = 0;
  size += 1;
  size += 32;
  size += 32 - (idx_end-idx_start+1)-(bob+1);
  return size;
}


/******************************************************************************/
/* Main boi */
/******************************************************************************/
int main()
{
  printf( "block_count( 4, 11 ) returned %d (should be 256)\n",
          block_count( 4, 11 ) );

  printf( "block_count( 5, 13 ) returned %d (should be ?)\n",
          block_count( 5, 13 ) );

  printf( "block_count( 3, 14 ) returned %d (should be ?)\n",
          block_count( 3, 14 ) );
  
  
  printf( "get_index( 0x2135C690, 4, 11 ) returned %d (should be 105)\n",
          get_index( 0x2135C690, 4, 11 ) );

  printf( "get_index( 0x2135C690, 5, 17 ) returned %d (should be ?)\n",
          get_index( 0x2135C690, 5, 17 ) );
  
  printf( "get_index( 0x2135C690, 2, 14 ) returned %d (should be ?)\n",
          get_index( 0x2135C690, 2, 14 ) );


  printf( "get_cache_block_size( 2, 16, 1 ) returned %d (should be 48)\n",
          get_cache_block_size( 2, 16, 1 ) );

  printf( "get_cache_block_size( 4, 15, 3 ) returned %d (should be ?)\n",
          get_cache_block_size( 4, 15, 3 ) );

  printf( "get_cache_block_size( 5, 18, 4 ) returned %d (should be ?)\n",
          get_cache_block_size( 5, 18, 4 ) );

  return 0;
}
