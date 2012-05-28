/*****
** Stack.h
** - defines a generic LIFO stack structure
** - maintains a void pointer in each node only
** - does not handle memory allocation for client data
** - supports optional memory deallocation for client data
** Notes
** - this package is provided as is with no warranty.
** - the author is not responsible for any damage caused
**   either directly or indirectly by using this package.
** - anybody is free to do whatever he/she wants with this
**   package as long as this header section is preserved.
** Created on 2004-01-20 by
** - Roger Zhang (rogerz@cs.dal.ca)
** Modifications
** -
** Last compiled under Linux with gcc-3
*/

#ifndef _RZ_C_STACK_
#define _RZ_C_STACK_

#define ENTERO 1
#define CHAR 2
#define BOOLEAN 3

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct _StackNode {
    void *data;
    struct _StackNode *link;
} StackNode;

typedef struct _Stack {
    int size;
    StackNode *top;
    StackNode *bottom;
} Stack;

/*****
** initialize an empty Stack
** must be called first after a new Stack is declared
*/ void stack_init(Stack *s);

/*****
** push a new element on top of the Stack
** it's up to the client code to allocate and maintain memory of "element"
*/ void stack_push(Stack *s, void *element);

/*****
** return the top element in the Stack, or NULL when the Stack is empty
*/ void *stack_top(Stack *s);

/*****
** remove the top element (pointer) from the Stack
** set "release" to non-zero if memory deallocation is desired
*/ Stack *stack_pop(Stack *s, int release);

/*****
** remove all elements (pointers) from the Stack
** set "release" to non-zero if memory deallocation is desired
*/ void stack_clear(Stack *s, int release);

/*****
** return current number of elements in the Stack, or 0 when Stack is empty
*/ int stack_size(Stack *s);


int stack_empty(Stack *s);

void* mulPilaValor(Stack* pila, void* valor);

void* mulValorPila(void* valor, Stack* pila);

void* mulPilaPila(Stack* p1, Stack* p2);

void stack_init(Stack *s)
{
    s->size = 0;
    s->top = s->bottom = NULL;
}

int stack_size(Stack *s)
{
    return s->size;
}

void stack_push(Stack *s, void *element)
{
    if (!s->top) {
        s->top = (StackNode*)malloc(sizeof(StackNode));
        s->top->data = element;
        s->bottom = s->top;
        s->bottom->link = NULL;
    } else {
        StackNode *temp = s->top;
        s->top = (StackNode*)malloc(sizeof(StackNode));
        s->top->data = element;
        s->top->link = temp;
    }

    s->size++;
}

void *stack_top(Stack *s)
{
    return s->size ? s->top->data : NULL;
}

Stack *stack_pop(Stack *s, int release)
{
    if (s->size) {
        StackNode *temp = s->top;
        if (--(s->size)) {
            s->top = s->top->link;
        } else {
            s->top = s->bottom = NULL;
        }
        // release memory accordingly
        if (release) {
            free(temp->data);
        }
        free(temp);
    }
    return s;
}

void stack_clear(Stack *s, int release)
{
    while (s->size) {
        StackNode *temp = s->top;
        s->top = s->top->link;
        if (release) {
            free(temp->data);
        }
        free(temp);
        s->size--;
    }

    s->top = s->bottom = NULL;
}

int stack_empty(Stack* s){
	return stack_size(s) == 0;
}

void* mulPilaValor(Stack* pila, void* valor){
	StackNode* actual;
	actual = pila->top;
	while (actual != pila->bottom){
		actual->data = (void*)((int)actual->data * (int)valor);
		
		actual = actual->link;
		
	}
	actual->data = (void*)((int)actual->data * (int)valor);
	return pila;
}

void* mulValorPila(void* valor, Stack* pila){
	return mulPilaValor(pila, valor);
}

void* mulPilaPila(Stack* p1, Stack* p2){
	assert(p1->size == p2->size);
	StackNode* actual1, *actual2;
	actual1 = p1->top;
	actual2 = p2->top;
	
	while(actual1 != p1->bottom){
		
		actual1->data = (void*)((int)actual1->data * (int)actual2->data);
		
		actual1 = actual1->link;
		actual2 = actual2->link;
	}
	actual1->data = (void*)((int)actual1->data * (int)actual2->data);
	return p1;
}

void* sumPilaValor(Stack* pila, void* valor){
	StackNode* actual;
	actual = pila->top;
	while (actual != pila->bottom){
		actual->data = (void*)((int)actual->data + (int)valor);
		
		actual = actual->link;
		
	}
	actual->data = (void*)((int)actual->data + (int)valor);
	return pila;
}

void* sumValorPila(void* valor, Stack* pila){
	return mulPilaValor(pila, valor);
}

void* sumPilaPila(Stack* p1, Stack* p2){
	assert(p1->size == p2->size);
	StackNode* actual1, *actual2;
	actual1 = p1->top;
	actual2 = p2->top;
	
	while(actual1 != p1->bottom){
		
		actual1->data = (void*)((int)actual1->data + (int)actual2->data);
		
		actual1 = actual1->link;
		actual2 = actual2->link;
	}
	actual1->data = (void*)((int)actual1->data + (int)actual2->data);
	return p1;
}

void* restaPilaValor(Stack* pila, void* valor){
	StackNode* actual;
	actual = pila->top;
	while (actual != pila->bottom){
		actual->data = (void*)((int)actual->data - (int)valor);
		
		actual = actual->link;
		
	}
	actual->data = (void*)((int)actual->data - (int)valor);
	return pila;
}

void* restaPilaPila(Stack* p1, Stack* p2){
	assert(p1->size == p2->size);
	StackNode* actual1, *actual2;
	actual1 = p1->top;
	actual2 = p2->top;
	
	while(actual1 != p1->bottom){
		
		actual1->data = (void*)((int)actual1->data - (int)actual2->data);
		
		actual1 = actual1->link;
		actual2 = actual2->link;
	}
	actual1->data = (void*)((int)actual1->data - (int)actual2->data);
	return p1;
}

void* divPilaValor(Stack* pila, void* valor){
	StackNode* actual;
	actual = pila->top;
	while (actual != pila->bottom){
		actual->data = (void*)((int)actual->data / (int)valor);
		
		actual = actual->link;
		
	}
	actual->data = (void*)((int)actual->data / (int)valor);
	return pila;
}


void* divPilaPila(Stack* p1, Stack* p2){
	assert(p1->size == p2->size);
	StackNode* actual1, *actual2;
	actual1 = p1->top;
	actual2 = p2->top;
	
	while(actual1 != p1->bottom){
		
		actual1->data = (void*)((int)actual1->data / (int)actual2->data);
		
		actual1 = actual1->link;
		actual2 = actual2->link;
	}
	actual1->data = (void*)((int)actual1->data / (int)actual2->data);
	return p1;
}

void printPila(Stack* pila, int tipo){
	StackNode* actual;
	actual = pila->top;
	while(actual != pila->bottom){
		
		if(tipo == ENTERO || tipo == BOOLEAN)
			printf("%d, ", (int)actual->data);
		else if(tipo == CHAR)
			printf("%c, ", (char)actual->data);
		
		actual = actual->link;
	}
	if(tipo == ENTERO || tipo == BOOLEAN)
			printf("%d", (int)actual->data);
	else if(tipo == CHAR)
		printf("%c", (char)actual->data);

}
		

#endif /* _RZ_C_STACK_ */
