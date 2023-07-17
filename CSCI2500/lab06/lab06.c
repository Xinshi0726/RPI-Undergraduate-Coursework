/* 
Lab 6: Decoding the instructions
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

/******************************************************************************/
/* Normal circuits */
/* Modify/add additional circuits as necessary (e.g., 3-input AND gate) */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}


/******************************************************************************/
/* Checkpoint 1 functions */
/* Add helper functions as necessary. I did:
  - One for converting integer->2's complement binary
  - One for converting register string to binary representation */
/******************************************************************************/
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
    for (int i = length-1; i >= 0; i--) {
        immediate[i] = B_Number % 10;
        B_Number /= 10;
    }
}

void convert_instructions()
{
  char line[256] = {0};
  char op[5];char r1[20];char r2[5];char r3[5];
  int rd[5]; int rt[5];int rs[5];int shamt[5] = {0,0,0,0,0};
  sscanf(line, "%s %s %s %s",op,r1,r2,r3);
  while (fgets(line, 256, stdin) != NULL) {
    // TODO: perform conversion
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
    if (strcmp(op,"j") == 0)
      {
          int opcode[6] = {0,0,0,0,1,0};
          unsigned long long int temp;
          sscanf(r1,"%d",&temp);
          int immediate[26];
          decimalToBinary(temp,immediate,26);
          for (int i = 0; i<6; i++)
          {
              Output[i] = opcode[i];
          }
          for (int i = 0; i<26; i++)
          {
              Output[i+6] = immediate[i];
          }
      }
    if (strcmp(op,"lw") == 0)
    {
        int opcode[6] = {1,0,0,0,1,1};
        unsigned long long int temp;
        sscanf(r3,"%d",&temp);
        if (strcmp(r1,"s0") == 0)
        {
            rs[0] = 0; rs[1] = 1; rs[2] = 0; rs[3] = 0; rs[4] = 0;
        }
        else if (strcmp(r1,"t0") == 0)
        {
            rs[0] = 1; rs[1] = 0; rs[2] = 0; rs[3] = 0; rs[4] = 0;
        }
        if (strcmp(r2,"s0") == 0)
        {
            rt[0] = 0; rt[1] = 1; rt[2] = 0; rt[3] = 0; rt[4] = 0;
        }
        else if (strcmp(r2,"t0") == 0)
        {
            rt[0] = 1; rt[1] = 0; rt[2] = 0; rt[3] = 0; rt[4] = 0;
        }
        int immediate[16];
        decimalToBinary(temp, immediate,16);
        for (int i = 0; i<6; i++)
        {
            Output[i] = opcode[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+6] = rs[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+11] = rt[i];
        }
        for (int i = 0; i<16; i++)
        {
            Output[i+16] = immediate[i];
        }
    }
    if (strcmp(op,"add") == 0)
    {
        int opcode[6] = {0,0,0,0,0,0};
        int funct[6] = {1,0,0,0,0,0};
        if (strcmp(r1,"t0") == 0)
        {
             rs[0] = 0; rs[1] = 1; rs[2] = 0; rs[3] = 0; rs[4] = 0;
        }
        else if (strcmp(r1,"s0") == 0)
        {
            rs[0] = 1; rs[1] = 0; rs[2] = 0; rs[3] = 0; rs[4] = 0;
        }
            rt[0] = 1; rt[1] = 0; rt[2] = 0; rt[3] = 0; rt[4] = 0;
        if (strcmp(r3,"t0") == 0)
        {
            rd[0] = 0; rd[1] = 1; rd[2] = 0; rd[3] = 0; rd[4] = 0;
        }
        else if (strcmp(r3,"s0") == 0)
        {
            rd[0] = 1; rd[1] = 0; rd[2] = 0; rd[3] = 0; rd[4] = 0;
            }
        for (int i = 0; i<6; i++)
        {
            Output[i] = opcode[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+6] = rs[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+11] = rt[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+16] = rd[i];
        }
        for (int i = 0; i<5; i++)
        {
            Output[i+21] = shamt[i];
        }
        for (int i = 0; i<6; i++)
        {
            Output[i+26] = funct[i];
        }
   }
    for (int i = 0; i <32; ++i) {
      printf("%d", Output[i]);
    }
    printf("\n");
  }
  
  return;
}

/******************************************************************************/
/* Checkpoint 2 circuits */
/******************************************************************************/

void decoder3(BIT* I, BIT EN, BIT* O)
{
  // TODO: implement 3-to-8 decoder using gates
  // See lecture slides, book, and/or online resources for logic designs
  if (EN == FALSE)
  {return;}
  O[0] = and_gate(and_gate(not_gate(I[0]),not_gate(I[1])),not_gate(I[2]));
  O[1] = and_gate(and_gate(not_gate(I[0]),not_gate(I[1])),I[2]);
  O[2] = and_gate(and_gate(not_gate(I[0]),I[1]),not_gate(I[2]));
  O[3] = and_gate(and_gate(not_gate(I[0]),I[1]),I[2]);
  O[4] = and_gate(and_gate(I[0],not_gate(I[1])),not_gate(I[2]));
  O[5] = and_gate(and_gate(I[0],not_gate(I[1])),I[2]);
  O[6] = and_gate(and_gate(I[0],I[1]),not_gate(I[2]));
  O[7] = and_gate(and_gate(I[0],I[1]),I[2]);
    return;
}

void decoder5(BIT* I, BIT* O)
{
    BIT Io = I[3];
    BIT I1 = I[4];
    BIT D0, D1, D2, D3;
    decoder2(Io,I1,&D0, &D1, &D2, &D3);
    BIT temp[3] = {I[2],I[1],I[0]};
    BIT o1[8] = {0};
    BIT o2[8] = {0};
    BIT o3[8] = {0};
    BIT o4[8] = {0};
    decoder3(temp, D0, o1);
    decoder3(temp, D1, o2);
    decoder3(temp, D2, o3);
    decoder3(temp, D3, o4);
    for (int i = 0; i<8;i++)
    {
        O[i] = o1[i];
    }
    for (int i = 0; i<8;i++)
    {
        O[i+8] = o2[i];
    }
    for (int i = 0; i<8;i++)
    {
        O[i+16] = o3[i];
    }
    for (int i = 0; i<8;i++)
    {
        O[i+24] = o4[i];
    }
  return;
}

/******************************************************************************/
/* Insane in the membmain */
/******************************************************************************/
int main()
{
  // convert some arbitrary number of instructions from stdin
  convert_instructions();
  
  // Unit test for 5-to-32 decoder
  printf("\n===== Unit test for 5-to-32 decoder =====\n");
  printf("decoder( I4, I3, I2, I1, I0 ) | ( O31, O30, ..., O1, O0 )\n");
  printf("------------------------------|--------------------------------------------------------------------------------------------------\n" );
  BIT I[5] = {0};
  BIT O[32] = {0};
  for (I[4] = 0; I[4] < 2; ++I[4])
    for (I[3] = 0; I[3] < 2; ++I[3])
      for (I[2] = 0; I[2] < 2; ++I[2])
        for (I[1] = 0; I[1] < 2; ++I[1])
          for (I[0] = 0; I[0] < 2; ++I[0]) {
              decoder5(I, O);
              printf( "decoder( %2d, %2d, %2d, %2d, %2d, ) | ( ",
                I[4], I[3], I[2], I[1], I[0]);
              printf("%2d", O[31]);
              for (int i = 30; i >= 0; --i)
                printf(", %2d", O[i]);
              printf(" )\n");
            }

  return 0;
}


