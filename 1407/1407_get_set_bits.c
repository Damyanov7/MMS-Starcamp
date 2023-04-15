#include<stdio.h>

int setbit0(int a, int k);
int setbit1(int a, int k);
int flipbit(int a, int k);

int main()
{
	printf("%d\n", setbit1(4,2));
	printf("%d\n", setbit0(7,1));
	printf("%d\n", flipbit(7,1));
}

int setbit1(int a, int k)
{
	return a |= 1 << k;
}

int setbit0(int a, int k)
{	
	return a &= ~(1 << k);
}

int flipbit(int a, int k)
{	
	return (a ^ (1 << k));
}


