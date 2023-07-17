/* 
Lab 5: From C to shining C
CSCI-2500 Spring 2021
Prof. Slota
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>


// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

/******************************************************************************/
/* Function Calls */
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
    if ((A && !B) || (!A && B))
        return TRUE;
    else
        return FALSE;
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
    // TO DO: implement a 2-input decoder
    if (!I0 && !I1)
        *O0 = TRUE;
    else
        *O0 = FALSE;
    if (!I0 && I1)
        *O1 = TRUE;
    else
        *O1 = FALSE;
    if (I0 && !I1)
        *O2 = TRUE;
    else
        *O2 = FALSE;
    if (I0 && I1)
        *O3 = TRUE;
    else
        *O3 = FALSE;

}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
    if (I0 && !S || I1 && S)
        return TRUE;
    else
        return FALSE;
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
    if (!S0 && !S1 && I0 || !S0 && S1 && I1 || S0 && !S1 && I2 || S1 && S0 && I3)
        return TRUE;
    else
        return FALSE;
}

void adder(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
    *Sum = xor_gate(xor_gate(A,B), CarryIn);
    *CarryOut = and_gate(A,B)||CarryIn&&(xor_gate(A,B));

}

void ALU(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Op0, BIT Op1,
         BIT* Result, BIT* CarryOut)
{
    BIT t1 = A && multiplexor2(Binvert,B,!B);
    BIT t2 = A || multiplexor2(Binvert,B,!B);
    adder(A,multiplexor2(Binvert,B,!B),CarryIn,CarryOut,Result);
    if (Op0 && Op1)
        *Result = UNDEF;
    else
        *Result = multiplexor4(Op0, Op1, t1,t2, *Result, FALSE);
}


/* main() provides some unit testing */
int main()
{
    /* Unit test for NOT gate:
     *      _
     *  A | A
     * ---|---
     *  0 | 1
     *  1 | 0
     */
    printf("===== Unit test for not_gate =====\n");
    printf("not_gate( A ) | RESULT\n");
    printf("--------------|-------\n");
    for (BIT A = 0; A < 2; ++A)
        printf("not_gate( %d ) | %d\n", A, not_gate(A));


    // Unit test for OR gate:
    printf("\n===== Unit test for or_gate =====\n");
    printf("or_gate( A, B ) | RESULT\n");
    printf("----------------|-------\n");
    for (BIT A = 0; A < 2; ++A)
        for (BIT B = 0; B < 2; ++B)
            printf("or_gate( %d, %d ) | %d\n", A, B, or_gate( A, B ));


    // Unit test for AND gate:
    printf( "\n===== Unit test for and_gate =====\n" );
    printf( "and_gate( A, B ) | RESULT\n" );
    printf( "-----------------|-------\n" );
    for (BIT A = 0; A < 2; ++A)
        for (BIT B = 0; B < 2; ++B)
            printf("and_gate( %d, %d ) | %d\n", A, B, and_gate(A, B));


    // Unit test for XOR gate:
    printf( "\n===== Unit test for xor_gate =====\n" );
    printf( "xor_gate( A, B ) | RESULT\n" );
    printf( "-----------------|-------\n" );
    for (BIT A = 0; A < 2; ++A)
        for (BIT B = 0; B < 2; ++B)
            printf("xor_gate( %d, %d ) | %d\n", A, B, xor_gate(A, B));


    // Unit test for 2-input decoder:
    printf( "\n===== Unit test for 2-input decoder =====\n" );
    printf( "decoder( I0, I1 ) | ( O0, O1, O2, O3 )\n" );
    printf( "------------------|-------------------\n" );
    BIT O0, O1, O2, O3;
    O0 = O1 = O2 = O3 = UNDEF;
    for (BIT I0 = 0; I0 < 2; ++I0)
        for (BIT I1 = 0; I1 < 2; ++I1) {
            decoder2(I0, I1, &O0, &O1, &O2, &O3);
            printf("decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n",
                   I0, I1, O0, O1, O2, O3);
        }


    // Unit test for 2-input multiplexer
    printf( "\n===== Unit test for 2-input multiplexor =====\n" );
    printf( "multiplexor( S ) | ( I0, I1 ) | RESULT\n" );
    printf( "-----------------|------------|-------\n" );
    for (BIT S = 0; S < 2; ++S)
        for (BIT I0 = 0; I0 < 2; ++I0)
            for (BIT I1 = 0; I1 < 2; ++I1)
                printf("multiplexer( %d ) | ( %2d, %2d ) | %2d\n",
                       S, I0, I1, multiplexor2(S, I0, I1));


    // Unit test for 4-input multiplexer
    printf("\n===== Unit test for 4-input multiplexor =====\n" );
    printf("multiplexor( S0, S1 ) | ( I0, I1, I2, I3 ) | RESULT\n");
    printf("----------------------|--------------------|-------\n");
    for (BIT S0 = 0; S0 < 2; ++S0)
        for (BIT S1 = 0; S1 < 2; ++S1)
            for (BIT I0 = 0; I0 < 2; ++I0)
                for (BIT I1 = 0; I1 < 2; ++I1)
                    for (BIT I2 = 0; I2 < 2; ++I2)
                        for (BIT I3 = 0; I3 < 2; ++I3)
                            printf( "multiplexer( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) | %2d\n",
                                    S0, S1, I0, I1, I2, I3, multiplexor4(S0, S1, I0, I1, I2, I3));


    // Unit test for 1-bit adder:
    printf( "\n===== Unit test for 1-bit adder =====\n" );
    printf( "adder( A, B, CI ) | ( CO, Sum )\n" );
    printf( "------------------|------------\n" );
    BIT CarryOut, Sum;
    CarryOut = Sum = UNDEF;
    for (BIT A = 0; A < 2; ++A)
        for (BIT B = 0; B < 2; ++B)
            for (BIT CarryIn = 0; CarryIn < 2; ++CarryIn) {
                adder(A, B, CarryIn, &CarryOut, &Sum);
                printf("adder( %d, %d, %2d ) | ( %2d, %3d )\n",
                       A, B, CarryIn, CarryOut, Sum);
            }


    // Unit test for 1-bit ALU
    printf("\n===== Unit test for 1-Bit ALU =====\n");
    printf("ALU( A, B ) | ( Bi, CI, Op0, Op1 ) | ( Re, CO )\n" );
    printf("------------|----------------------|-----------\n" );
    BIT Result = UNDEF;
    for (BIT A = 0; A < 2; ++A)
        for (BIT B = 0; B < 2; ++B)
            for (BIT Binvert = 0; Binvert < 2; ++Binvert)
                for (BIT CarryIn = 0; CarryIn < 2; ++CarryIn)
                    for (BIT Op0 = 0; Op0 < 2; ++Op0)
                        for (BIT Op1 = 0; Op1 < 2; ++Op1) {
                            ALU(A, B, Binvert, CarryIn, Op0, Op1, &Result, &CarryOut);
                            printf( "ALU( %d, %d ) | ( %2d, %2d, %3d, %3d ) | ( %2d, %2d )\n",
                                    A, B, Binvert, CarryIn, Op0, Op1, Result, CarryOut);
                        }

    return 0;
}

