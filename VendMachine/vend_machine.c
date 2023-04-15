#include <stdio.h>
#include <string.h>
#include "sm.h"

#define TRUE 1
#define SSTART 1
#define SFINAL 2

#define EMONEY 1
#define EABORT 2

typedef struct context_t {
	unsigned target_sum;
	unsigned coins_count[6];
	unsigned coins_values[6];
	unsigned coin;
	unsigned current_sum;
	unsigned flag_done;
} context_t;

void initialize_context(context_t *ctx);
int print_result(void* v, delta_t* t) ;
int addNchek(void* v, delta_t* t);
int my_atoi(char *str);
int change(context_t* v);

delta_t deltas[] = {
	{SSTART, EABORT, SFINAL, NULL, print_result},
	{SSTART, EMONEY, SSTART, addNchek, NULL},
	{SSTART, EMONEY, SFINAL, NULL, print_result},
	{UNDEF, UNDEF, UNDEF, NULL, NULL}
};

	//This is a smelly code, which I practiced on doing the vend_machine with state machine from the last 2 camps test.
	//It works, if you want to try it out, run it and the console will tempt you to enter values (it will tell you what for).
	//There are a lot of useles things in the code because they were part of the main idea but in the process it actually could've 
	//been done a lot shorter and it ended up kinda long, so I wont be wasting time cleaning it. 

int main() {

	context_t ctx;
	initialize_context(&ctx);
	sm_t sm;
	
	int event;
	
	sm_contructor(&sm, SSTART, SFINAL, UNDEF, deltas, &ctx);
	
	while (1) {
		char value[6];
		printf("Enter value (1c, 2c, 5c, 10c, 20c, 50c, ABORT): ");
		scanf("%s", value);
	
		if (strcmp(value, "ABORT") >= 0) 
			event = EABORT;
		else {
			if (my_atoi(value)) {
				event = EMONEY;
				ctx.coin = my_atoi(value);
			} 
			else {
				printf("You've enetered invalid value, please try again.\n");
				continue;
			}
		}
		
		if (!(sm_exec(&sm, event))) break;		
	
	}
	
	
	
	return 0;
}


void initialize_context(context_t *ctx) {
	int j;
	int holder;
	printf("Enter target sum dollars: ");
	scanf("%d", &holder);
	ctx->target_sum = holder * 100;
	printf("Enter target sum cents: ");
	scanf("%d", &holder);
	ctx->target_sum += holder;
	
	
	printf("Enter coin count 1c: ");
	scanf("%d", &ctx->coins_count[0]);
	printf("Enter coin count 2c: ");
	scanf("%d", &ctx->coins_count[1]);
	printf("Enter coin count 5c: ");
	scanf("%d", &ctx->coins_count[2]);
	printf("Enter coin count 10c: ");
	scanf("%d", &ctx->coins_count[3]);
	printf("Enter coin count 20c: ");
	scanf("%d", &ctx->coins_count[4]);
	printf("Enter coin count 50c: ");
	scanf("%d", &ctx->coins_count[5]);
	
	ctx->coins_values[0] = 1;
	ctx->coins_values[1] = 2;
	ctx->coins_values[2] = 5;
	ctx->coins_values[3] = 10;
	ctx->coins_values[4] = 20;
	ctx->coins_values[5] = 50;
	
	ctx->current_sum = 0;
	ctx->flag_done = 0;
}

int my_atoi(char *str) {
	char *c = str;
	int ret = 0;
	
	while(*c) {
		if(*c < '0' && *c > '9') {
			ret = 0;
			goto error;
		}
			
		ret = (ret * 10) + (int)(*c - '0');
		c++;
	}
	
	if ( !(ret == 1 || ret == 2 || ret == 5 || ret == 10 || ret == 20 || ret == 50) )ret = 0;		
error:
	return ret;
}

	// Add n check functions, first it accumulates to the current_sum
	// Then check if we've gotten to the desired target_sum
	// Return 1 if we achieved current_sum > target_sum
int addNchek(void* v, delta_t* t) {
	context_t *ctx = (context_t*)v;
	int j;
	int ret = 0;
	
	ctx->current_sum += ctx->coin;
	for(j = 0; j < 6; j++) {
		if (ctx->coin == ctx->coins_values[j]) {
			ctx->coins_count[j] += 1;
		}
	}
		
	if(ctx->current_sum >= ctx->target_sum) {
		ctx->flag_done = TRUE;
		ret = 1;
	}
	 
	return ret;
}

int print_result(void* v, delta_t* t) {
	context_t *ctx = (context_t*)v;
	
	if (t->event == EABORT) {
		printf("\n\nAborted!\n");
		printf("Current sum of coins: %d\n", ctx->current_sum);
		for (int j = 0; j < 6; j++) 
			printf("Machine bank count %d: %d\n", ctx->coins_values[j], ctx->coins_count[j]);		
	}
	else {
		printf("\n\nDone!\n");
		if(change(ctx)) 
			printf("The change is: %d\n", ctx->current_sum - ctx->target_sum);
		else 
			printf("Cannot return change, returning the inserted coins: %d\n", ctx->current_sum);
			
		for (int j = 0; j < 6; j++) 
				printf("Machine bank count %d: %d\n", ctx->coins_values[j], ctx->coins_count[j]);	
	}
	
	return 1;
}

// This part is smelly, got some other things to study so I'm leaving it like this. PS probably the whole code smells :D
int change(context_t* v) {
	context_t *ctx =  (context_t*)v;
	int i;
	int change;
	int ret = 0;
	
	
	// In case that there is no need of returning change
	change = ctx->current_sum - ctx->target_sum;
	if (change == 0) {
		ret = 1;
		goto done;
	}

	
	
	// This is the logic that will set "ret" to either 
	// 1 if You can return the change and modify the counts correctly
	// 0 if You cannot return change and return the money back, modifying 
	// the counts correctly again
	for(i = 5; i >= 0; i--) {
		int count = ctx->coins_count[i];
		while (ctx->coins_values[i] <= change) {
			if (!count) 
				break;
			change -= ctx->coins_values[i];
			count--;
		}
		if (change == 0) {
			ret = 1;
			break;
	        }
	}
	// Logic ends here
	
	
	if (ret) {
		change = ctx->current_sum - ctx->target_sum;
		for(i = 5; i >= 0; i--) {
			while (v->coins_values[i] <=  change) {
				if (!(v->coins_count[i])) 
					break;
				change -= v->coins_values[i];
				ctx->coins_count[i]--;
			}
		if (change == 0) break;
		}
	} 
	else {
		change = ctx->current_sum;
		for(i = 5; i >=0; i--) {
			while (v->coins_values[i] <=  change) {
				if (v->coins_count[i]) break;
				change -= v->coins_values[i];
				ctx->coins_count[i]--;
			}	
			
			if (change == 0) break;
		}
	}
done:		
	return ret;
}

