#ifndef HW_H
#define HW_H

#include "types.h"

#define EXTRA_SPACE 512
#define STACK_SIZE() (2 * sizeof(uint32) + EXTRA_SPACE)

typedef void (*func_t) (void);

struct ctx_s {
	uint32* sp;
	uint32* lr;
	func_t f;
};

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);

#endif
