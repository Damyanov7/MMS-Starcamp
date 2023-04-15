#include<stdio.h>

int setbits1(int a, int m, int n, int k);
int setbits0(int a, int m, int n, int k);
int flipbits(int a, int m, int n, int k);

int main()
{
	printf("%d\n", setbits1(8,0,1,2));
	printf("%d\n", setbits0(15,0,1,2));
	printf("%d\n", flipbits(7,0,1,2));
}

int setbits1(int a, int m, int n, int k)
{
	int mask=0;
	if(m >= 0) mask |= (1 << m);
	if(n >= 0) mask |= (1 << n);
	if(k >= 0) mask |= (1 << k);
	
	return (mask | a);
}

int setbits0(int a, int m, int n, int k)
{
	int mask=0;
	if(m >= 0) mask |= (1 << m);
	if(n >= 0) mask |= (1 << n);
	if(k >= 0) mask |= (1 << k);
	
	return (a & ~mask);
}

int flipbits(int a, int m, int n, int k)
{
	int mask=0;
	if(m >= 0) mask |= (1 << m);
	if(n >= 0) mask |= (1 << n);
	if(k >= 0) mask |= (1 << k);
	
	return (a ^ mask);
}
