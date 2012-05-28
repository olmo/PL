#include <stdlib.h>
typedef struct _pila_float{
	float* pila;
	int numElems;
	int max;
} PilaFloat;

void init(PilaFloat* p){
	p->max = 100;
	p->numElems = 0;
	p->pila = (float*)malloc(p->max*sizeof(float));
}

float tope(PilaFloat* p){
	return p->pila[p->numElems];
}

PilaFloat* push(PilaFloat* p, float el){
	if(p->numElems >= p->max){
		p->pila = (float*)realloc(p->pila, (p->max+100)*sizeof(float));
		p->max+=100;
	}
	p->pila[++p->numElems] = el;
	return p;
}

PilaFloat* pop(PilaFloat* p){
	p->numElems--;
	return p;
}

int vacia(PilaFloat* p){
	return p->numElems == 0;
}
