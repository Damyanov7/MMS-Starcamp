#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sm.h"

#define ERROR 1
#define INVALID -1

#define SSTATE 1
#define FSTATE 2

#define EENTER 1 
#define ELEAVE 2
#define EEOF 3


#define MAX_NOF_CARDS 256
#define MAX_SZOF_INPUT 16



typedef struct card_t { 
	uint8_t access_level; 
	uint8_t current_level;
	uint8_t card_id[6];
} card_t;

typedef struct context_t {
	card_t *cards;
	card_t current_card;
	
} context_t;

static int hextodec(char ch);
static int input_file(card_t *cards);
static void initialize_ctx(context_t *ctx, card_t * cards);
static int find_event(char *str_, context_t * ctx);
static int enter_func(void *ctx_, delta_t *deltas);
static int leave_func(void *ctx_, delta_t *deltas);

		
static delta_t deltas[] = {
   		{ SSTATE, ELEAVE, SSTATE, NULL, leave_func },
   		{ SSTATE, EENTER, SSTATE, NULL, enter_func },
   		{ SSTATE, EEOF, FSTATE, NULL, NULL },
   		{ UNDEF, UNDEF, UNDEF, NULL, NULL}
   		
};

 /* The program reads characters (0-255) from input file and stores their data them in an "array" of card_t
    Each card is identified by Access level and id of the card.
    The user enters Hexadecimally input, example nffb1b2b3b4b5b6, where 'n' stands for enter
    and on the same place 'l' could be entered for leave. The next byte, in our case - "ff" stands for the access level that
    the id_card has. The next 6 bytes (b1b2b3b4b5b6) stand for the ID of the card. 
    
    In my input file I have entered "1211111 :111111 :121111", '1' in asci table as hexadec is 31, '9' is 39 and ':' is 3A
    Example for entering a level with card_t ":111111" is (stdinput): 39313131313131, and you will enter that card at level "39"*/
    

int main() {
	state_t sm;
	card_t card[MAX_NOF_CARDS]; // File info will be inputted here
	context_t ctx;
	
	// Input size should be 1 byte for Leave/Enter state, 2 bytes 
	//for access level, 12 bytes for card_id and 1 byte for '\0' == 16
	
	char stdinput[MAX_SZOF_INPUT];
	int event; 
	int ret = 0; // if no error occur, ret 0;
	
	
	//Init to 0 everything, afterwards get info from file
	(void)initialize_ctx(&ctx, card);
	if(input_file(card)) {
		ret = ERROR;
		goto exit;
	}

	(void)state_constructor(&sm, SSTATE, FSTATE, UNDEF,	deltas, &ctx);
	
   	
   	while (1) {
   		printf("Enter new request(N-ENTER/L-LEAVE/A-ABORT)\n");
   		scanf("%s", stdinput);
   		event = find_event(stdinput, &ctx);
   		if (event == INVALID) {
   			printf("Invalid input!\n");
   			continue;
   		}
   	
   		//if state exec returns something diff than 1, EOF has come 
   		if (state_exec(&sm, event) != 0) break;
   	
   	}	
 
exit:
	return ret;  	  	
}




static int hextodec(char ch) {
	int ret;
	
	if(ch >= '0' && ch <= '9') {
		ret = ch - '0';
	} else if (ch >= 'a' && ch <= 'f') {
		ret = ch - 'a' + 10;
	}
	
	return ret;
}


static int input_file(card_t *cards) {
	FILE *fptr;
	char ch;
	int pos, ret, j;
	
	ret = 0;
	fptr = fopen("file_input","r");
	if (!fptr) {
       	fprintf(stderr, "Error opening file!\n");
       	ret = ERROR;
       	goto err;
   	}
	
	ch = fgetc(fptr);
	pos = 0; 
   	j = 0;	
   	while (ch != EOF && ch!= '\n') {
   		
   		if (pos < 1) {
   			cards[j].access_level = ch;
   			pos++;
   		}
   		else if (pos >= 1 && pos <= 7) {
   			cards[j].card_id[pos - 1] = ch;
   			pos++;
   		} 
   		
   		if(pos == 7) {
   			 pos = 0;
   			 j++;
   		}
   		
   		ch = fgetc(fptr);
   	}
   	
   	fclose(fptr);

err:
	return ret;
}

static void initialize_ctx(context_t *ctx, card_t * cards) {
	int i, j;
	
	ctx->cards = cards;
	for (j = 0; j < MAX_NOF_CARDS; j++) {
		ctx->cards[j].current_level = 0;
		ctx->cards[j].access_level = 0;
		for(i = 0; i < 6; i++)
			ctx->cards[j].card_id[i] = 0;	
	}
	
	for(j = 0; j < 6; j++) 
		ctx->current_card.card_id[j] = 0;
	ctx->current_card.access_level = 0;
	
	return;
}

static int find_event(char *str_, context_t * ctx) {
	int event, pos, i, count;
	char* str = str_;
	
	count = 0;
	pos = 0;
	i = 0;

	if(*str == 'a' || *str == 'A') {
		printf("ABORT\nExiting ...\n");
		event = EEOF;
		goto exit;
	}
	
	// 1 byte for the N or L, 2 bytes for the access level (ff) and 12 bytes for the card_id, and lastly 1 byt for the '\0'
	
	if(strlen(str) != 15) {
		event = INVALID; 
		goto exit;
	}
	
	if ( (*str == 'n') || (*str == 'N') ) 
		event = EENTER;
	else if ( (*str == 'l') || (*str == 'L') ) 
		event = ELEAVE;
	else { 
		event = INVALID; 
		goto exit; 
	}
	
	while (*(++str)) {
		if (pos < 2) {
			ctx->current_card.access_level = (ctx->current_card.access_level << 4) + hextodec(*str);
			pos++;
		} else if (pos < 14) {
			
			if (count == 2) {
				count = 0;
				i++;
			}
			
			ctx->current_card.card_id[i] = (ctx->current_card.card_id[i] << 4) + hextodec(*str);
			count++;
		}
	}
	
exit:
	return event;
}

int enter_func(void *ctx_, delta_t *deltas) {
	int result = 0, j, i;
	int flag = 1;
	
	context_t *ctx = (context_t *)ctx_;
	
	for (j = 0; j < 5; j++) {
		flag = 1;
		for (i = 0; i < 6; i++) {
			if(ctx->current_card.card_id[i] != ctx->cards[j].card_id[i]) {
				flag = 0;
				break;
			}			
		}
		
		if (flag) {
			if (ctx->cards[j].current_level != 0) {
				printf("REJECT - THIS CARD HAS ALREADY ENTERED A LEVEL\n");
				goto result; 
			} else if (ctx->current_card.access_level <= ctx->cards[j].access_level) {
				printf("PERMIT - ENTERING %x LEVEL\n", ctx->current_card.access_level);
				ctx->cards[j].current_level = ctx->current_card.access_level;
				goto result; 
			} else { 
				printf("REJECT - YOU HAVE NO ACCESS TO THIS LEVEL\n");
				goto result; 
			}
		}
	}
	printf("Invalid ID.\n");
result:
	return result;
}

int leave_func(void *ctx_, delta_t *deltas) {
	int result = 0, i, j;
	int flag = 1;
	
	context_t *ctx = (context_t *)ctx_;
	
	
	for (j = 0; j < 5; j++) {
		flag = 1;
		for (i = 0; i < 6; i++) {
			if(ctx->current_card.card_id[i] != ctx->cards[j].card_id[i]) {
				flag = 0;
				break;
			}
		}			
			
		if (flag) {
			if (ctx->cards[j].current_level == 0) {
				printf("REJECT - YOU CANNOT LEAVE BEFORE ENTERING\n");
				goto result; 
			} else if (ctx->current_card.access_level <= ctx->cards[j].access_level) {
				printf("PERMIT - LEAVING TO %x LEVEL\n", ctx->current_card.access_level);
				ctx->cards[j].current_level = ctx->current_card.access_level;
				goto result; 
			} else { 
				printf("REJECT - YOU HAVE NO ACCESS TO THIS LEVEL\n");
				goto result; 
			}		
		}
	}
result:
	return result;
}

