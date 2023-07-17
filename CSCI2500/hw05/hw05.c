/* 
HW 5: Processing MIPS instructions using logical gates simulated in C
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};

/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT and_gate(BIT A, BIT B);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void set_op(char op, BIT* OP);
void convert_to_binary(int a, BIT* A);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void shifter(BIT* A, BIT* R, BIT Control);
void left_shifter32(BIT* A, BIT* B, BIT* R);
void multiplier16(BIT* A, BIT* B, BIT* R);
void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum);
void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set);
void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut);
void Control(BIT* A, BIT* B, BIT* OP, BIT* S);

  
/******************************************************************************/
/* Functions provided for your convenience */
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

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}


void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}
void decoder3(BIT* I, BIT* O)
{
    // TODO: implement 3-to-8 decoder using gates
    // See lecture slides, book, and/or online resources for logic designs
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
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
    return or_gate((or_gate(and_gate(and_gate(not_gate(S0),not_gate(S1)), I0), and_gate(and_gate(not_gate(S0), S1), I1))) , or_gate(and_gate(and_gate(S0, not_gate(S1)), I2), and_gate(and_gate(S1, S0), I3)));
}
BIT multiplexor8(BIT S0, BIT S1, BIT S2, BIT I0, BIT I1, BIT I2, BIT I3, BIT I4, BIT I5, BIT I6, BIT I7)
{
    BIT I[3] = {S0,S1,S2};
    BIT O[8] = {0};
    decoder3(I,O);
    BIT I01 = or_gate(and_gate(I0,O[0]),and_gate(I1,O[1]));
    BIT I23 =or_gate(and_gate(I2,O[2]),and_gate(I3,O[3]));
    BIT I45 = or_gate(and_gate(I4,O[4]),and_gate(I5,O[5]));
    BIT I67 = or_gate(and_gate(I6,O[6]),and_gate(I7,O[7]));
    BIT I0123 = or_gate(I01,I23);
    BIT I4567= or_gate(I45,I67);
    BIT res = or_gate(I0123, I4567);
    return res;
}


// This will set the OP bits for Control() based on the specified operation
void set_op(char op, BIT* OP)
{
  switch (op) {
    case '&': OP[2] = FALSE; OP[1] = FALSE; OP[0] = FALSE; break;
    case '|': OP[2] = FALSE; OP[1] = FALSE; OP[0] = TRUE;  break;
    case 'n': OP[2] = FALSE; OP[1] = TRUE;  OP[0] = FALSE; break;
    case '+': OP[2] = FALSE; OP[1] = TRUE;  OP[0] = TRUE;  break;
    case '-': OP[2] = TRUE;  OP[1] = FALSE; OP[0] = FALSE; break;
    case '<': OP[2] = TRUE;  OP[1] = FALSE; OP[0] = TRUE;  break;
    case 's': OP[2] = TRUE;  OP[1] = TRUE;  OP[0] = FALSE; break;
    case '*': OP[2] = TRUE;  OP[1] = TRUE;  OP[0] = TRUE;  break;
    default: break;
  }
}

/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
unsigned long long int power1 (unsigned long long int x, unsigned long long int y)
{
    if (y == 0)
        return 1;
    else if ((y % 2) == 0)
        return power1 (x, y / 2) * power1 (x, y / 2);
    else
        return x * power1 (x, y / 2) * power1 (x, y / 2);

}
void convert_to_binary(int a, BIT* A)
{
  // TODO: convert integer to 2's complement BIT representation
  // Note: A[0] is least significant bit and A[31] is most significant bit
  // For this function ONLY: you're allowed to use logical if-else statements
    // To store the binary number
    int counter = 0;
    while (a > 0)
    {
        A[counter] = a%2;
        a = a/2;
        counter += 1;
    }
}

void shifter(BIT* A, BIT* R, BIT Control)
{ 
  // TODO: implement 1-bit shifter
  // One call to this circuit will shift A one bit to the left or right
  // The result will be stored in R
  // Control = 0 -> shift left
  // Control = 1 -> shift right
  // See: http://www.mathcs.emory.edu/~jallen/Courses/355/Syllabus/1-circuits/shifter.html
  R[0] = and_gate(Control,A[1]);
  for (int i = 1; i< 31; i++)
  {
      R[i] = or_gate(and_gate(A[i-1],not_gate(Control)),and_gate(A[i+1], Control));
  }
  R[31] = and_gate(not_gate(Control),A[30]);
}

void left_shifter32(BIT* A, BIT* B, BIT* R)
{
  // TODO: Implement 32-bit left shifter
  // You'll shift input A to the left by the number specific in B
  // The result will be stored in R
  // You can assume that B >= 0
  // There's no specified circuit that you need to implement here. The logic
  // that I used for my implementation was basically:
  // B_temp = B, A_temp = A
  // for i = 0..31
  //   use ALU to check if B_temp < 1
  //   do left shift
  //   use mux to store B_temp in R if the result from above was false
  //   subtract 1 from B_temp using ALU
  BIT ls[32];
  BIT CarryOut;
  BIT A_temp[32];
  BIT B_temp[32];
  BIT B_temp1[32];
  BIT one[32] = {0};
  one[0] = 1;
  BIT shift[32];
  copy_bits(A,A_temp);
  copy_bits(B,B_temp);
  for (int i = 0; i<32; i++)
  {
      ALU32(B_temp,one, 1,1,1,1,ls,&CarryOut);
      shifter(A_temp,shift,0);
      for (int j = 0; j<32;j++)
      {
          R[j] = multiplexor2(ls[0],shift[j],A_temp[j]);
          A_temp[j] = multiplexor2(ls[0],shift[j],A_temp[j]);
      }
      ALU32(B_temp,one,1,1,1,0,B_temp1, &CarryOut);
      copy_bits(B_temp1,B_temp);
  };
  
}


void multiplier16(BIT* A, BIT* B, BIT* R)
{  
  // TODO: Implement 16-bit multiplier
  // S = A * B
  // You can assume A and B are both less than 2^16
  // This should work regardless if A and B are positive/negative
  // See the diagram on slide 49 of Chapter-3d
  // - You can use your 32-bit ALU here for addition
  // - You can use your shifter as well for R/L shifts
  // - For 'control', you're allowed to use a for loop
  BIT add[32];
  BIT CarryOut;
  BIT R_temp[32];
  copy_bits(R,R_temp);
  BIT A_temp[32];
  BIT B_temp[32];
  BIT A_temp1[32];
  BIT B_temp1[32];
  copy_bits(A,A_temp);
  copy_bits(B,B_temp);
  for (int i = 0; i<31; i++)
  {
      for (int j = 0;j<32;j++){
          add[j] = multiplexor2(A_temp[0],0,B_temp[j]);
      }
      ALU32(R_temp,add,0,0,1,0,R,&CarryOut);
      copy_bits(R,R_temp);
      shifter(A_temp, A_temp1, 1);
      shifter(B_temp, B_temp1, 0);
      copy_bits(A_temp1,A_temp);
      copy_bits(B_temp1,B_temp);
  }
  copy_bits(R_temp,R);
}


void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution
    BIT x0 = xor_gate(A, B);
    *Sum = xor_gate(CarryIn, x0);

    BIT y0 = and_gate(x0, CarryIn);
    BIT y1 = and_gate(A, B);
    *CarryOut = or_gate(y0, y1);
}


void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut, BIT* Set)
{
  // TODO: implement a 1-bit ALU 
  // Note: this will need modifications from Lab 5 to account for 'slt'
  // See slide 30 in Chapter-3d
    // TODO: implement a 1-bit ALU
    BIT x0 = multiplexor2(Binvert, B, not_gate(B));

    BIT y0 = and_gate(A, x0);
    BIT y1 = or_gate(A, x0);

    BIT y2 = FALSE;
    adder1(A, x0, CarryIn, CarryOut, &y2);
    *Set = y2;
    *Result = multiplexor4(Op0, Op1, y0, y1, y2, Less);
}


void ALU32(BIT* A, BIT* B, BIT Binvert, BIT CarryIn, 
  BIT Op0, BIT Op1, BIT* Result, BIT* CarryOut)
{
  // TODO: implement a 32-bit ALU
  // You'll need to essentially implement a 32-bit ripple addder here
  // See slide 31 in Chapter-3d
  BIT Set;
  BIT R;
  BIT CarryIn1 = CarryIn;
  for (int i = 0; i<32; i++)
  {
      ALU1(A[i],B[i],Binvert,CarryIn,0,Op0,Op1, &R, CarryOut, &Set);
      CarryIn = *CarryOut;
      Result[i] = R;
  }
    ALU1(A[0],B[0],Binvert,CarryIn1,Set,Op0,Op1, &R, CarryOut, &Set);
    Result[0] = R;
}


void Control(BIT* A, BIT* B, BIT* OP, BIT* Result) 
{
  // The will be the control circuit for all of our ALU operations
  // You will do some specified operation between A and B and store the final 
  // output into Result
  // One difficulty might be in converting our 3-bit OP input to the 2-bit
  // control for the ALU. You can use SOP circuits for this.
  // For my implementation, I compute 4 results here and use a 4-mux to get
  // the final Result:
  //  1. Output from ALU (and, or, add, sub, slt)
  //  2. Logical nor (using output from ALU)
  //  3. Left shift
  //  4. Multiplication
  // You can use this approach or any other that you can come up with, as long
  // as it is mostly representative of an implementable circuit.
  // Note: be careful with your multiplication and shifter circuits. You can 
  // create/copy temporary 32-bit binary values as often as necessary. Just make
  // sure you haven't overwritten the A, B inputs before you return to main.
    BIT CarryOut;
    BIT And[32] = {0};
    BIT Or[32] = {0};
    BIT nor[32] = {0};
    BIT add[32] = {0};
    BIT sub[32] = {0};
    BIT slt[32] = {0};
    BIT sll[32] = {0};
    BIT mul[32] = {0};
    ALU32(A,B,0,0,0,0,And,&CarryOut);
    ALU32(A,B,0,0,0,1,Or,&CarryOut);
    ALU32(A,B,0,0,0,1,nor,&CarryOut);
    for (int i = 0; i<32 ; i++)
    {
        nor[i] = not_gate(nor[i]);
    }
    ALU32(A,B,0,0,1,0,add,&CarryOut);
    ALU32(A,B,1,1,1,0,sub,&CarryOut);
    ALU32(A,B,1,1,1,1,slt,&CarryOut);
    left_shifter32(A,B,sll);

    multiplier16(A,B,mul);
    for (int i = 0; i<32; i++)
    {
        Result[i] = multiplexor8(OP[2],OP[1],OP[0],And[i],Or[i],nor[i],add[i],sub[i],slt[i],sll[i],mul[i]);
    }
}


/******************************************************************************/
/* Main */
/******************************************************************************/
int main()
{
  // Read in our operands and operation
  int a = 0;
  int b = 0;
  char op = '\0';
  scanf("%d %c %d", &a, &op, &b);

  // Do our necessary converstions
  BIT A[32] = {FALSE};
  BIT B[32] = {FALSE};
  BIT R[32] = {FALSE};
  convert_to_binary(a, A);
  convert_to_binary(b, B);

  // Set our OP bits for the operation
  BIT OP[3] = {FALSE};
  set_op(op, OP);

  // Call our control circuit to do the business
  Control(A, B, OP, R);

  // Output our result
  print_binary(A);
  printf("\n%c\n", op);
  print_binary(B);
  printf("\n=\n");
  print_binary(R);

  return 0;
}

