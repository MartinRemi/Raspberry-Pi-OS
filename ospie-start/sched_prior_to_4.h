#ifndef HW_H
#define HW_H

#include "types.h"

#define EXTRA_SPACE 512
#define STACK_SIZE (2 * sizeof(uint32) + EXTRA_SPACE)

typedef void (*func_t) (void);

struct ctx_s {
	uint32* sp;
	uint32* link_register;
	func_t f;
};

struct ctx_s* current_ctx;
struct ctx_s* saved_ctx;

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);

#endif
