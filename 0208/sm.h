#ifndef _VEND_
#define _VEND_

#define UNDEF -2
typedef struct delta_t delta_t;

typedef int (*predicate_t)(void*, delta_t*);
typedef int (*callback_t)(void*, delta_t*);

struct delta_t {
	int state;
	int event;
	int next;
	predicate_t predicate;
	callback_t callback;
};

typedef struct sm_t {
	int state;
	int final;
	int spurious;
	delta_t *deltas;
	void *ctx;
} sm_t;

void sm_contructor(sm_t *sm, int state, int final, int spurious, delta_t * deltas, void *ctx);
int sm_exec(sm_t *sm, int event);

#endif
