#include "sched.h"
#include "hw.h"

void funcA()
{
	int
	cptA = 0;
	while( cptA <=2 ) {
		cptA ++;
		ctx_switch();
	}
}

void funcB()
{
	int cptB = 1;
	while( cptB <= 4 ) {
		cptB += 2 ;
		ctx_switch();
	}
}
//------------------------------------------------------------------------


int kmain (void)
{
	init_hw();
	create_process(funcB, NULL, STACK_SIZE);
	create_process(funcA, NULL, STACK_SIZE);
	start_sched();
	ctx_switch();
	return 0;
}