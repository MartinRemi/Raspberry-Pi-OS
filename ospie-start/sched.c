#include "sched.h"

struct pcb_s* first = NULL;
struct pcb_s* last = NULL;
struct pcb_s* current_process = NULL;

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size) {
	ctx->sp = phyAlloc_alloc(stack_size) + stack_size - 14 * 4;
	ctx->link_register = f;
	ctx->f = f;
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx) {
	// Sauvegarde des registres
	__asm("push {r0-r12}");	

	// Sauvegarde du contexte courant
	__asm("mov %0, sp" : "=r"(current_ctx->sp));
	__asm("mov %0, lr" : "=r"(current_ctx->link_register));

	// Changer contexte courant
	current_ctx = ctx;

	// Restauration du contexte
	__asm("mov sp, %0" : : "r"(ctx->sp));
	__asm("mov lr, %0" : : "r"(ctx->link_register));

	// Restauration des registres
	__asm("pop {r0-r12}");	

	// Sauter à l'adresse de retour
	__asm("bx lr");
}

void init_pcb(struct pcb_s* pcb, func_t f, unsigned int stack_size, void* args) {	
	pcb->state = NEW;	
	pcb->ctx = phyAlloc_alloc(sizeof(struct ctx_s));
	init_ctx(pcb->ctx, f, stack_size);
	pcb->size = stack_size;
	pcb->args = args;
}

void create_process(func_t f, void* args, unsigned int stack_size) {
	struct pcb_s* pcb = phyAlloc_alloc(sizeof(struct pcb_s));
	if(first == NULL) {
		first = pcb;
		last = pcb;
	} else {
		last->next = pcb;
	}
	pcb->next = first;
	init_pcb(pcb, f, stack_size, args);
}

void start_current_process() {
	current_process->ctx->f(current_process->args);
}

void elect() {
	if(current_process != NULL) {
		current_process = current_process->next;
	} else {
		current_process = first;
	}
}

void start_sched() {
}

void __attribute__ ((naked)) ctx_switch() {
	int i = 9;

	// Sauvegarde des registres
	__asm("push {r0-r12}");	


	// Sauvegarde du contexte du current_process
	if(current_process != NULL) {
		__asm("mov %0, sp" : "=r"(current_process->ctx->sp));
		__asm("mov %0, lr" : "=r"(current_process->ctx->link_register));
	}

	// Demande au scheduler d'élire un nouveau processus
	elect();

	// Restaure le contexte du processus elu
	if(current_process != NULL) {
		__asm("mov sp, %0" : : "r"(current_process->ctx->sp));
		__asm("mov lr, %0" : : "r"(current_process->ctx->link_register));
	}
	
	// Restauration des registres
	__asm("pop {r0-r12}");	

	// Sauter à l'adresse de retour
	__asm("bx lr");
}
