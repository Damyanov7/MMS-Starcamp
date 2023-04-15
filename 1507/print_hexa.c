#include <stdio.h>

void print_hex0(int a);
void print_hex1(int a);
void print_hex2(int a);

int main(void) {

	
	print_hex0(-1);
	print_hex0(1);
	print_hex0(-13);
	print_hex0(13);
	print_hex0(-16000);
	
	printf("\n Second way to print stuff (with 0x0000000f)");
	
	print_hex1(-1);
	print_hex1(1);
	print_hex1(-13);
	print_hex1(13);
	print_hex1(-16000);
	return 0;
}

void print_hex0(int a) {
	unsigned int mask;
	int t;
	
	for (mask = 0xf0000000u, t = 28; mask; mask >>= 4, t -= 4)
		switch ((a & mask) >> t) {
		case 0:
			printf("0");
			break;
		case 1:
			printf("1");
			break;
		case 2:
			printf("2");
			break;
		case 3:
			printf("3");
			break;
		case 4:
			printf("4");
			break;
		case 5:
			printf("5");
			break;
		case 6:
			printf("6");
			break;
		case 7:
			printf("7");
			break;
		case 8:
			printf("8");
			break;
		case 9:
			printf("9");
			break;
		case 10:
			printf("a");
			break;
		case 11:
			printf("b");
			break;
		case 12:
			printf("c");
			break;
		case 13:
			printf("d");
			break;
		case 14:
			printf("e");
			break;
		case 15:
			printf("f");
			break;
		}
		
	
	printf("\n");
	
	return;
}

void print_hex1(int a) {
	unsigned int mask;
	int t;
	
	for (mask = 0x0000000fu, t = 28; mask ; mask<<=4, t-=4, a<<=4 )
	
		switch ( (a & mask << t) >> 28 ) {
		case 0:
			printf("0");
			break;
		case 1:
			printf("1");
			break;
		case 2:
			printf("2");
			break;
		case 3:
			printf("3");
			break;
		case 4:
			printf("4");
			break;
		case 5:
			printf("5");
			break;
		case 6:
			printf("6");
			break;
		case 7:
			printf("7");
			break;
		case 8:
			printf("8");
			break;
		case 9:
			printf("9");
			break;
		case 10:
			printf("a");
			break;
		case 11:
			printf("b");
			break;
		case 12:
			printf("c");
			break;
		case 13:
			printf("d");
			break;
		case 14:
			printf("e");
			break;
		case 15:
			printf("f");
			break;
		}
		
	
	printf("\n");
	
	return;
}

void print_hex2(int a) {
}
