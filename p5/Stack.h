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

#include "dec_dat.h"

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
*/ void stack_pop(Stack *s, int release);

/*****
** remove all elements (pointers) from the Stack
** set "release" to non-zero if memory deallocation is desired
*/ void stack_clear(Stack *s, int release);

/*****
** return current number of elements in the Stack, or 0 when Stack is empty
*/ int stack_size(Stack *s);


int stack_empty(Stack *s);

#endif /* _RZ_C_STACK_ */
