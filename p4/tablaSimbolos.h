typedef enum {
	marca, 						/* marca comienzo bloque */
	procedimiento, 				/* si es subprograma */
	variable, 					/* si es variable */
	parametro, 					/* si es parametro formal */
} tipoEntrada ;

typedef enum {
	entero,
	real,
	caracter,
	booleano,
	pila_entero,
	pila_real,
	pila_booleano,
	pila_caracter,
	cadena, // No se si hay que poner esto
	desconocido,
	no_asignado
} dtipo ;

typedef struct {
	tipoEntrada entrada ;
	char *nombre ;
	dtipo tipoDato ;
	unsigned int parametros ;
	unsigned int dimensiones ;
	int pila;
	int TamDimen1 ; 			/* Tamaño Dimensión 1 */ //No se usa
	int TamDimen2 ; 			/* Tamaño Dimensión 2 */ //No se usa
} entradaTS ;

typedef struct {
	int atrib ; 				/* Atributo del símbolo (si tiene) */
	char *lexema ; 				/* Nombre del lexema */
	dtipo tipo ; 				/* Tipo del símbolo */
} atributos ;

#define YYSTYPE atributos 		/* A partir de ahora, cada símbolo tiene una estructura de tipo atributos*/
#define MAX_TS 1000

unsigned int TOPE=0 ; 			/* TOPE de la pila */
unsigned int Subprog ; 			/* Indicador de comienzo de bloque de un subprog */
entradaTS TS[MAX_TS] ; 			/* Pila de la tabla de símbolos */

int yylineno;					/* Numero de linea */


/*********************  FUNCIONES TABLA SIMBOLOS  ****************************************+
*******************************************************************************************/

void imprimeTS () {
	/* Imprime por pantalla la tabla de simbolos */
	int i;
	int letra;
	for (i=0; i<TOPE; i++) {
		if (TS[i].entrada==marca)
			printf ("\n<<< MARCA >>>");
		if (TS[i].entrada==procedimiento){
			printf ("\nProcedimiento: %s, Tipo: %d, Num. Parametros: %d", TS[i].nombre, TS[i].tipoDato, TS[i].parametros);
		}
		if (TS[i].entrada == variable){
			printf ("\nVariable: %s, Tipo: %d", TS[i].nombre, TS[i].tipoDato);
		}
			
		if (TS[i].entrada==parametro)
			printf ("\nParametro: %s, Tipo: %d", TS[i].nombre, TS[i].tipoDato);
	}

	printf ("\n");
	getchar();

	printf("********************************************************************");
}



char* MostrarTipo(dtipo tipo){
	if(tipo == 0)
		return(" entero ");

	if(tipo == 1)
		return(" real ");
	
	if(tipo == 2)
		return(" caracter ");

	if(tipo == 3)
		return(" booleano ");	

	if(tipo == 4)
		return(" pila entero");
	if(tipo == 5)
		return(" pila real");

	if(tipo == 6)
		return(" pila booleano ");
	if(tipo == 7)
		return(" pila caracter ");
	if(tipo == 8)
		return(" cadena ");
	if(tipo == 9)
		return(" desconocido ");

	if(tipo == 10)
		return(" no asignado ");

}

void buscar_repetidas(char *lexema) {
	/* Miramos que en la tabla de simbolos no exista otro identificador con el mismo lexema*/
	int i;

	/* Comprobar si ya existe otro con el mismo lexema en el mismo
	   bloque */
	for (i=TOPE; TS[i].entrada != marca; i--){

		if (!strcmp(TS[i].nombre, lexema) && TS[i].entrada != parametro) {
			printf ("\nError Semantico en la linea %d: Identificador %s ya esta declarado\n", yylineno, lexema);
			return;
		}
	}
}

int es_repetida(char *lexema) {
	/* Miramos que en la tabla de simbolos no exista otro identificador con el mismo lexema*/
	int i;

	/* Comprobar si ya existe otro con el mismo lexema en el mismo
	   bloque */
	for (i=TOPE; TS[i].entrada != marca; i--){

		if (!strcmp(TS[i].nombre, lexema) && TS[i].entrada != parametro) {
			printf ("\nError Semantico en la linea %d: Identificador %s ya esta declarado\n", yylineno, lexema);
			return 1;
		}
	}

	return 0;
}


int existe (char *lexema) {
	/* Devuelve 1 si esta definida una variable con el nombre especificado
	   o 0 en caso contrario */
	long int temp;

	for (temp=TOPE; temp>0;temp--)
		if (TS[temp].entrada == variable && !strcmp(TS[temp].nombre, lexema))
			return 1;

	return 0;
}



void InsertarElemento(tipoEntrada tipo, char* lexema){
	//Insertamos un elemento en la tabla de simbolos

	TOPE++;
	TS[TOPE].entrada = tipo;
	TS[TOPE].nombre = strdup(lexema);
	TS[TOPE].tipoDato = no_asignado;
	TS[TOPE].pila=0;
	if( tipo == procedimiento)
		TS[TOPE].parametros = 0;
	
}

void esPila(){
	TS[TOPE].pila=1;
}

void asignarTipo (dtipo tipo, int posicion) {
	/* Asigna el tipo especificado al elemento en la posicion posicion de la TS*/
	TS[posicion].tipoDato = tipo;
	if((tipo==entero)&&(TS[posicion].pila==1)){
		TS[posicion].tipoDato = pila_entero;
	}
	if((tipo==real)&&(TS[posicion].pila==1)){
		TS[posicion].tipoDato = pila_real;
	}
	if((tipo==booleano)&&(TS[posicion].pila==1)){
		TS[posicion].tipoDato = pila_booleano;
	}
	if((tipo==caracter)&&(TS[posicion].pila==1)){
		TS[posicion].tipoDato = pila_caracter;
	}		
	
}

void asignarTipoCascada(dtipo tipo) {
	/* Asigna el tipo especificado a todas las entradas consecutivas
		que sean variables de tipo no_asignado */

	int i;

	for (i=TOPE; TS[i].entrada==variable && TS[i].tipoDato == no_asignado; i--){
		asignarTipo(tipo, i);
		
	}
		
}

int BuscarParametroRepetido (char *lexema) {
	/* Comprobar si ya existe otro parametro con el mismo lexema en el mismo
	   procedimiento*/

	int i;

	for (i=TOPE; TS[i].entrada != procedimiento; i--){
		if (!strcmp(TS[i].nombre, lexema)) {
			printf ("\nError Semantico en la linea %d: Parametro %s esta duplicado\n", yylineno, lexema);
			return 1;
		}
	}
	return 0;
}

void CuentaParametros () {
	/* Asigna el tipo desconocido al procedimiento y  cuenta cuantos parametros tiene */
	int i;
	int parametros=0;

	for (i=TOPE;TS[i].entrada != procedimiento;i--)
		if (TS[i].entrada == parametro) {
			parametros++;
		}

	TS[i].tipoDato=desconocido;
	TS[i].parametros=parametros;
	//printf("parametrillos %d",parametros);
	//MostrarTS();
}

void IntroIniBloq() {
	/* Inserta una entrada en la tabla de simbolo con una marca */
	/* y copia los parametros del procedimiento como si
	   fueran variables */
	int i;
	TOPE++;

	TS[TOPE].entrada = marca;
	for (i=TOPE-1; i>=0 && TS[i].entrada != marca && TS[i].entrada != procedimiento; i--)
		if (TS[i].entrada == parametro) {
			/* Hemos encontrado un parametro, incluirlo como variable */
			InsertarElemento (variable, TS[i].nombre);
			asignarTipo (TS[i].tipoDato, TOPE);
			if(TS[i].pila==1){
				esPila();
			}
		}
	//MostrarTS();
}

void IntroFinBloq () {
	/* Elimina entradas de la tabla de simbolos hasta que se encuentra
	    una marca de comienzo de bloque o se queda la tabla de simbolos
	   vacia */

	for (;TOPE>=0 && TS[TOPE].entrada!=marca;TOPE--);
	if (TOPE!=0)
		TOPE--;
	//MostrarTS();
	
}

void existeProc (char *lexema) {
/*comprueba que existe un nombre de procedimiento declarado igual al que se le pasa*/
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0 && TS[TOPE].entrada!=marca; i--)
		if (TS[i].entrada == procedimiento && !strcmp(TS[i].nombre, lexema))
			igual=1;

	if(igual==0)
		printf ("\nError Semantico en la linea %d: El procedimiento %s no est?declarado\n", yylineno, lexema);
}


int existeProc2 (char *lexema) {
/*comprueba que existe un nombre de procedimiento declarado igual al que se le pasa*/
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0 && TS[TOPE].entrada!=marca; i--)
		if (TS[i].entrada == procedimiento && !strcmp(TS[i].nombre, lexema))
			igual=1;
	return igual;
}

int numParametros(char* lexema){
/*Devuelve el numero de parametros de un procedimiento*/
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0; i--)
		if (TS[i].entrada == procedimiento && !strcmp(TS[i].nombre, lexema)){
			return TS[i].parametros;
		}


}

dtipo tipoParametro(int num, char* lexema){
/*Devuelve el tipo de parametro del procedimiento llamado lexema en la posicion num*/
	int i;
	int igual=0;
	for (i=TOPE; i>=0; i--)
		if (TS[i].entrada == procedimiento && !strcmp(TS[i].nombre, lexema)){
			return (TS[i+num].tipoDato);
		}
} 


dtipo get_tipo (char *lexema) {
	/* Devuelve el tipo si esta definida una variable con el nombre lexema
	    */
	long int temp;

	for (temp=TOPE; temp>0 && TS[TOPE].entrada!=marca;temp--)
		if (TS[temp].entrada == variable && !strcmp(TS[temp].nombre, lexema))
			return TS[temp].tipoDato;
}

dtipo tipoPila (dtipo p) {
	/* Devuelve el tipo de la pila
	    */
	if(es_pila(p)==1){
		if(p==pila_booleano)
			return booleano;
		if(p==pila_entero)
			return entero;
		if(p==pila_real)
			return real;
		if(p==pila_caracter)
			return caracter;
	}
	else{
		return p;
	}	

}

int es_pila(dtipo p){
	if((p==pila_booleano)||(p==pila_entero)||(p==pila_real)||(p==pila_caracter))
		return 1;
	else
		return 0;
}

dtipo tipoEnPila(dtipo p){
		
		if(p==booleano)
			return pila_booleano;
		if(p==entero)
			return pila_entero;
		if(p==real)
			return pila_real;
		if(p==caracter)
			return pila_caracter;

		return p;

}