#include "Stack.h"
#include "dec_dat.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

	Stack pe, *pe1;
	PilaFloat pilaFloat, *pFloat1;
	init(&pilaFloat);
	stack_init(&pe);
	stack_push(&pe, (void*)10);
	stack_push(&pe, (void*)9);
	stack_push(&pe, (void*)8);
	stack_push(&pe, (void*)7);
	stack_push(&pe, (void*)6);
	stack_push(&pe, (void*)5);
	stack_push(&pe, (void*)4);
	stack_push(&pe, (void*)3);
	stack_push(&pe, (void*)2);
	stack_push(&pe, (void*)1);
	stack_push(&pe, (void*)0);
	stack_push(&pe, (void*)-1);
	stack_push(&pe, (void*)-2);
	stack_push(&pe, (void*)-3);
	stack_push(&pe, (void*)-4);
	push(&pilaFloat, 10.0);
	push(&pilaFloat, 9.0);
	push(&pilaFloat, 8.0);
	push(&pilaFloat, 7.0);
	push(&pilaFloat, 6.0);
	push(&pilaFloat, 5.0);
	push(&pilaFloat, 4.0);
	push(&pilaFloat, 3.0);
	push(&pilaFloat, 2.0);
	push(&pilaFloat, 1.0);
	push(&pilaFloat, 0.0);
	push(&pilaFloat, -1.0);
	push(&pilaFloat, -2.0);
	push(&pilaFloat, -3.0);
	push(&pilaFloat, -4.0);
	pe1 = &pe;
	pFloat1 = &pilaFloat;
	mulPilaPila(&pe, pe1);
	mulPilaPilaF(&pilaFloat, pFloat1);
	while(!stack_empty(&pe)){
		
		printf("%d %f\n", (int)stack_top(&pe), tope(&pilaFloat));
		//printf("OK\n");
		pop(&pilaFloat);
		stack_pop(&pe, 0);
	}
	
	
	
	return 0;
}
