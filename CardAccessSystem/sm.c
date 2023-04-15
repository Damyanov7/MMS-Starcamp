#include <errno.h>
#include "sm.h"

void state_constructor(state_t *state, int Initial, int Final, int Spurious, delta_t *Deltas, void *Context) {
	state->current = Initial;
	state->final = Final;
	state->spurious = Spurious;
	state->deltas = Deltas;
	state->ctx = Context;
	
	return;
}

void state_reset(state_t *state, int current) {
	state->current = current;
	
	return;
}

int state_exec(state_t *state, int event) {
	int ret = UNDEF;
	int j;
	
	errno = 0;
	
	if (state->current == state->spurious) {
		errno = EINVAL;
		goto error;
	}
	
	if (state->current == state->final) {
		ret = 0;
		goto error;
	}
	
	for (j = 0; (state->deltas[j].current != UNDEF) || (state->deltas[j].next != UNDEF); j++)
		if (
				((state->deltas[j].current == state->current) || state->deltas[j].current == ANY) && 
				((state->deltas[j].event == event) || (state->deltas[j].event == ANY))
		) {
			if (state->deltas[j].predicate)
				if (!(state->deltas[j].predicate)(state->ctx, &(state->deltas[j]))) continue;
			if (state->deltas[j].callback)
				ret = (state->deltas[j].callback)(state->ctx, &(state->deltas[j]));
			else ret = 1;
			break;
		}
		
	if (ret < 0) {
		state->current = state->spurious;
		errno = EINVAL;
		goto error;
	}
	
	if (ret > 0) state->current = state->deltas[j].next;
	
	if (state->current == state->final) {
		ret = 1;
		goto error;
	}
		
error:
	return ret;
}
