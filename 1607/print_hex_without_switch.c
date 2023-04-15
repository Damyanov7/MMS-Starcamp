#include<stdio.h>

void print_hex(int a);

int main()
{
	print_hex(-1);
	print_hex(1);
	print_hex(-13);
	print_hex(13);
	print_hex(-16000);
}


void print_hex(int a) {

	char c[16] = { '0','1','2','3','4',
			 '5','6','7','8','9',
			 'a','b','c','d','e','f' };
	unsigned int mask;
	int t;
	
	for (mask = 0xf0000000u, t = 28; mask; mask >>= 4, t -= 4)
		
		printf("%c", c[(a & mask) >> t]);
	
	printf("\n");
	
	return;
}
