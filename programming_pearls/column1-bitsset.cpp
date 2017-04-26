/* Copyright (C) 1999 Lucent Technologies */
/* From 'Programming Pearls' by Jon Bentley */

/* bitsort.c -- bitmap sort from Column 1
 *   Sort distinct integers in the range [0..N-1]
 */


/*
Some important things:

a[1 + N/BITSPERWORD] is the size of the array required to represent the numbers.

a[i>>SHIFT] gives you the index in a, where i will be represented in the bits position. 
For example, when i = 15, then it is represented in  i/32 i.e is 0th index (a[0])
when i is 63, then it is represented by i/32 i.e is 1st index (a[1])
(1<<(i & MASK) gives the bit position in the 32 byte. 63 will be 31st byte and when this 
is ORed with a[i>>SHIFT], we set the 31st byte in a[1].

When we test, we get the bit postion of i, and then perform the reverse operation of what we did prior.
a[i>>SHIFT] &   (1<<(i & MASK));



*/

#include <stdio.h>

#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F
#define N 10000000

int a[1 + N/BITSPERWORD];

void set(int i) 
{
    int sh = i>>SHIFT;
    a[i>>SHIFT] |=  (1<<(i & MASK)); 
}

void clr(int i) 
{ 
    a[i>>SHIFT] &= ~(1<<(i & MASK));
}

int  test(int i)
{ 
    return a[i>>SHIFT] &   (1<<(i & MASK)); 
}

int main()
{   int i;
for (i = 0; i < N; i++)
    clr(i);

    /*Replace above 2 lines with below 3 for word-parallel init
    int top = 1 + N/BITSPERWORD;
    for (i = 0; i < top; i++)
    a[i] = 0;
    */

while (scanf("%d", &i) != EOF)
    set(i);
for (i = 0; i < N; i++)
        if (test(i))
    printf("%d\n", i);
return 0;
}
