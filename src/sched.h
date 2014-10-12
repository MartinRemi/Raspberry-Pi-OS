#ifndef SCHED_H
#define SCHED_H

#include "types.h"

#define EXTRA_SPACE 512
#define NULL 0
#define STACK_SIZE (2 * sizeof(uint32) + EXTRA_SPACE)

typedef void (*func_t) (void*);

struct ctx_s {
	uint32* sp;
	uint32* link_register;
	func_t f;
};

struct ctx_s* current_ctx;
struct ctx_s* saved_ctx;

typedef enum {
	NEW, READY, WAITING, RUNNING, TERMINATED
} process_state;

struct pcb_s {
	struct ctx_s* ctx;
	uint32 size;
	void* args;
	struct pcb_s* next;
	process_state state;
};

void idle();

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);

void init_pcb( struct pcb_s* pcb, func_t f, unsigned int stack_size, void* args);

void create_process(func_t f, void* args, unsigned int stack_size);

void start_current_process();

void elect();

void start_sched();

void __attribute__ ((naked)) ctx_switch();

void ctx_switch_from_irq();

#endif
