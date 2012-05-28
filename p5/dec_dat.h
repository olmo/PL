
#ifndef PILA_DEC_DAT
#define PILA_DEC_DAT

#include <stdlib.h>
#include <assert.h>

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
	assert(p->numElems >= 0);
	return p->pila[p->numElems-1];
}

PilaFloat* push(PilaFloat* p, float el){
	if(p->numElems >= p->max){
		p->pila = (float*)realloc(p->pila, (p->max+100)*sizeof(float));
		p->max+=100;
	}
	p->pila[p->numElems++] = el;
	return p;
}

PilaFloat* pop(PilaFloat* p){
	assert(p->numElems > 0);
	p->numElems--;
	return p;
}

int vacia(PilaFloat* p){
	return p->numElems == 0;
}

PilaFloat* mulPilaValorF(PilaFloat* pila, float valor){
	int i;
	for(i=0; i<=pila->numElems; i++)
		pila->pila[i] *= valor;
	return pila;
}

PilaFloat* mulValorPilaF(float valor, PilaFloat* pila){
	return mulPilaValorF(pila, valor);
}

PilaFloat* mulPilaPilaF(PilaFloat* p1, PilaFloat* p2){
	assert(p1->numElems == p2->numElems);
	
	int i;
	for(i=0; i<=p1->numElems; i++)
		p1->pila[i] *= p2->pila[i];
		
	return p1;
}

PilaFloat* sumPilaValorF(PilaFloat* pila, float valor){
	int i;
	for(i=0; i<=pila->numElems; i++)
		pila->pila[i] += valor;
	return pila;
}

PilaFloat* sumValorPilaF(float valor, PilaFloat* pila){
	return mulPilaValorF(pila, valor);
}

PilaFloat* sumPilaPilaF(PilaFloat* p1, PilaFloat* p2){
	assert(p1->numElems == p2->numElems);
	
	int i;
	for(i=0; i<=p1->numElems; i++)
		p1->pila[i] += p2->pila[i];
		
	return p1;
}

PilaFloat* restaPilaValorF(PilaFloat* pila, float valor){
	int i;
	for(i=0; i<=pila->numElems; i++)
		pila->pila[i] -= valor;
	return pila;
}

PilaFloat* restaPilaPilaF(PilaFloat* p1, PilaFloat* p2){
	assert(p1->numElems == p2->numElems);
	
	int i;
	for(i=0; i<=p1->numElems; i++)
		p1->pila[i] -= p2->pila[i];
		
	return p1;
}

PilaFloat* divPilaValorF(PilaFloat* pila, float valor){
	int i;
	for(i=0; i<=pila->numElems; i++)
		pila->pila[i] /= valor;
	return pila;
}

PilaFloat* divPilaPilaF(PilaFloat* p1, PilaFloat* p2){
	assert(p1->numElems == p2->numElems);
	
	int i;
	for(i=0; i<=p1->numElems; i++)
		p1->pila[i] /= p2->pila[i];
		
	return p1;
}

void printPilaF(PilaFloat* pila){
	int i;
	for(i=pila->numElems-1; i>=0; i--)
		if(i==0)
			printf("%f", pila->pila[i]);
		else printf("%f, ", pila->pila[i]);
}



#endif
