#include"sm.h"

void sm_contructor(sm_t *sm, int state, int final, int spurious, delta_t * deltas, void *ctx) {
	sm->state = state;
	sm->final = final;
	sm->spurious = spurious;
	sm->deltas = deltas;
	sm->ctx = ctx;
}

int sm_exec(sm_t *sm, int event) {
	int j;
	int ret =  UNDEF;
	
	if (sm->state == sm->final) {
		ret = 0;
		goto error;
	}
	
	for (j = 0; sm->deltas[j].state != UNDEF && sm->deltas[j].next != UNDEF; j++)
		if (sm->state == sm->deltas[j].state && event == sm->deltas[j].event) {
			if(sm->deltas[j].predicate)
				if(sm->deltas[j].predicate(sm->ctx, &sm->deltas[j]))
					continue;
			if(sm->deltas[j].callback) {
				sm->deltas[j].callback(sm->ctx, &sm->deltas[j]);
				ret = 0;
			}
			else 	
				ret = 1;
				break;
		}
	
	if (ret < 0) {
		sm->state = sm->spurious;
		goto error;
	}
		
	if (ret == 0) {
		sm->state = sm->final;
		goto error;
	}
	
	if (ret > 0) {
		sm->state = sm->deltas[j].next;
		goto error;
	}
	
	if (sm->state == sm->final) {
		ret = 0;
		goto error;
	}
		
	error:	
	return ret;
}
