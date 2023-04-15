#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

uint32_t convert(uint32_t value);

int main() {
	uint32_t t= 0xdeadbeef;
	printf("%x \n", convert(t));
}

uint32_t convert(uint32_t val) {
	uint32_t cval = 0;
	uint32_t mask = 0x000000ff;
	int pos_cval;
	
	for(pos_cval = 24; val; val >>= 8, pos_cval -= 8) {
		cval |= ((mask & val)) << pos_cval;
	}
	
	return cval;
}
