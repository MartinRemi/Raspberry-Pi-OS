#include "sched.h"

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size) {
	ctx->sp = phyAlloc_alloc(stack_size);
	ctx->link_register = f;
	ctx->f = f;
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx) {
	// Sauvegarde du contexte courant
	__asm("mov %0, sp" : "=r"(current_ctx->sp));
	__asm("mov %0, lr" : "=r"(current_ctx->link_register));

	// Changer contexte courant
	current_ctx = ctx;

	// Restauration du contexte
	__asm("mov sp, %0" : : "r"(ctx->sp));
	__asm("mov lr, %0" : : "r"(ctx->link_register));

	// Sauter à l'adresse de retour
	__asm("bx lr");
}
