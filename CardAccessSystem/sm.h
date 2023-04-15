#ifndef _STM_H_
#define _STM_H_

#define ANY -1
#define UNDEF -2

typedef struct delta_t delta_t;

typedef int (*predicate_t)(void *, delta_t *);
typedef int (*callback_t)(void *, delta_t *);

struct delta_t {
	int current;
	int event;
	int next;
	predicate_t predicate;
	callback_t callback;
};

typedef struct state_t {
	int current;
	int final;
	int spurious;
	delta_t *deltas;
	void *ctx;
} state_t;

void state_constructor(state_t *state, int Initial, int Final, int Spurious, delta_t *Deltas, void *Context);
int state_exec(state_t *state, int event);
void state_reset(state_t *state, int current);

#endif

