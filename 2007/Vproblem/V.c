#include<stdlib.h>
#include"V.h"

char *longest(char *s, char c){
	char *p = NULL;
	size_t  count=0u, max = 0u;
	
	
		for(s; *s; s++){
			if(*s == c){ 
				count ++; 
				if(count > max){
					max = count;
					p = (s+1) - count;
					}
				}else count =0u;
			}
	return p;
}

