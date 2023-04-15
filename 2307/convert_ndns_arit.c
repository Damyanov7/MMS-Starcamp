#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

uint32_t convert(uint32_t value);
uint64_t convert64(uint64_t val);

int main() {
	uint32_t t = 0xdeadbeef;
	printf("%x \n%x \n", t, convert(t));
	printf("------------------\n");
	uint64_t t64 = 0xdeadbeefcafebabel;
	printf("%lx \n%lx \n", t64, convert64(t64));
}

uint32_t convert(uint32_t val) {
	
	return (uint32_t)((unsigned char)val << 24) |
	(uint32_t)( ((unsigned char)(val >> 8)) << 16 ) |
	(uint32_t)( ((unsigned char)(val >> 16)) << 8 ) |
	(uint32_t)( ((unsigned char)(val >> 24)) << 0 );
}

uint64_t convert64(uint64_t val) {
	return ((uint64_t)convert(val)) << 32 | (uint64_t)convert(val >> 32);
}


