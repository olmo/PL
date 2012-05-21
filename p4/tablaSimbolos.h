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
	pila,
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
	int TamDimen1 ; 			/* Tamaño Dimensión 1 */
	int TamDimen2 ; 			/* Tamaño Dimensión 2 */
} entradaTS ;

typedef struct {
	int atrib ; 				/* Atributo del símbolo (si tiene) */
	char *lexema ; 				/* Nombre del lexema */
	dtipo tipo ; 				/* Tipo del símbolo */
} atributos ;

#define YYSTYPE atributos 		/* A partir de ahora, cada símbolo tiene una estructura de tipo atributos*/
#define MAX_TS 500

unsigned int TOPE=0 ; 			/* TOPE de la pila */
unsigned int Subprog ; 			/* Indicador de comienzo de bloque de un subprog */
entradaTS TS[MAX_TS] ; 			/* Pila de la tabla de símbolos */


/*********************  FUNCIONES TABLA SIMBOLOS  ****************************************+
*******************************************************************************************/

void imprimeTS () {
	/* Imprime por pantalla la tabla de simbolos */
	int i;
	int letra;
	for (i=0; i<TOPE; i++) {
		if (TS[i].tipoentrada==marca)
			printf ("\n<<< MARCA >>>");
		if (TS[i].tipoentrada==procedimiento){
			printf ("\nProcedimiento: %s, Tipo: %d, Num. Parametros: %d", TS[i].lexema, TS[i].tipodato, TS[i].parametros);
		}
		if (TS[i].tipoentrada == variable){
			printf ("\nVariable: %s, Tipo: %d", TS[i].lexema, TS[i].tipodato);
			if(TS[i].vector==1){
					printf (", dimensiones: %d", TS[i].ndim);
				if(TS[i].ndim==1)
					printf (", dimension1: %d", TS[i].dim1);
				if(TS[i].ndim==2)
					printf (", dimension1: %d, dimension2: %d", TS[i].dim1,TS[i].dim2);
				
			}
		}	
		if (TS[i].tipoentrada==parametro)
			printf ("\nParametro: %s, Tipo: %d", TS[i].lexema, TS[i].tipodato);
	}

	printf ("\n");
	getchar();

	printf("********************************************************************");
}



char* MostrarTipo(dTipo tipo){
	if(tipo == 0)
		return(" boolean ");

	if(tipo == 1)
		return(" int ");
	
	if(tipo == 2)
		return(" float ");

	if(tipo == 3)
		return(" char ");	

	if(tipo == 4)
		return(" vector boolean");
	if(tipo == 5)
		return(" vector int");

	if(tipo == 6)
		return(" vector float ");
	if(tipo == 7)
		return(" vector char ");
	if(tipo == 8)
		return(" void ");
	if(tipo == 9)
		return(" desconocido ");

	if(tipo == 10)
		return(" no asignado ");
	if(tipo == 11)
		return(" cadena ");

}

void buscar_repetidas(char *lexema) {
	/* Miramos que en la tabla de simbolos no exista otro identificador con el mismo lexema*/
	int i;
	int igual = 0;

	/* Comprobar si ya existe otro con el mismo lexema en el mismo
	   bloque */
	for (i=TOPE; TS[i].tipoentrada != marca; i--){
		igual = strcmp(TS[i].lexema, lexema);

		if (!igual && TS[i].tipoentrada != parametro) {
			printf ("\nError Semantico en la linea %d: Identificador %s ya esta declarado\n", yylineno, lexema);
			return;
		}
	}
}

int es_repetida(char *lexema) {
	/* Miramos que en la tabla de simbolos no exista otro identificador con el mismo lexema*/
	int i;
	int igual = 0;

	/* Comprobar si ya existe otro con el mismo lexema en el mismo
	   bloque */
	for (i=TOPE; TS[i].tipoentrada != marca; i--){
		igual = strcmp(TS[i].lexema, lexema);

		if (!igual && TS[i].tipoentrada != parametro) {
			printf ("\nError Semantico en la linea %d: Identificador %s ya esta declarado\n", yylineno, lexema);
			return 1;
		}
	}

	return 0;
}


int existe (char *lexema) {
	/* Devuelve 1 si existe definida una variable con el nombre especificado
	   o 0 en caso contrario */
	long int temp;

	for (temp=TOPE; temp>0;temp--)
		if (TS[temp].tipoentrada == variable && !strcmp(TS[temp].lexema, lexema))
			return 1;

	return 0;
}

void actualizaVector(int d1, int d2, int dim){
	if(dim==1){
		TS[TOPE].ndim=1;
		if(d1<=0){
			printf ("\nError Semantico en la linea %d: el vector %s debe tener dimensiones positivas\n", yylineno, TS[TOPE].lexema);
			TOPE--;
			return;
		}
		else{
			TS[TOPE].dim1=d1;
		}
	
	}
	if(dim ==2){
		if((d1<=0)||(d2<=0)){
			printf ("\nError Semantico en la linea %d: el vector %s debe tener dimensiones positivas\n", yylineno, TS[TOPE].lexema);
			TOPE--;
			return;
		}
		else{
			TS[TOPE].ndim=2;
			TS[TOPE].dim1=d1;
			TS[TOPE].dim2=d2;
		}
	}

}

void InsertarElemento(TipoEntrada tipo, char* lexema){
	//Insertamos un elemento en la tabla de simbolos

	TOPE++;
	TS[TOPE].tipoentrada = tipo;
	TS[TOPE].lexema = strdup(lexema);
	TS[TOPE].tipodato = no_asignado;
	TS[TOPE].vector=0;
	TS[TOPE].ndim=0;
	if( tipo == procedimiento)
		TS[TOPE].parametros = 0;
	
}

void esVector(){
	TS[TOPE].vector=1;
}

void asignarTipo (dTipo tipo) {
	/* Asigna el tipo especificado al elemento en el TOPE de la TS*/
	TS[TOPE].tipodato = tipo;
	if((tipo==entero)&&(TS[TOPE].vector==1)){
		TS[TOPE].tipodato = vector_entero;
	}
	if((tipo==flotante)&&(TS[TOPE].vector==1)){
		TS[TOPE].tipodato = vector_flotante;
	}
	if((tipo==boleano)&&(TS[TOPE].vector==1)){
		TS[TOPE].tipodato = vector_boleano;
	}
	if((tipo==caracter)&&(TS[TOPE].vector==1)){
		TS[TOPE].tipodato = vector_caracter;
	}		
	
}

void asignarTipoCascada(dTipo tipo) {
	/* Asigna el tipo especificado a todas las entradas consecutivas
		que sean variables de tipo no_asignado */

	int i;

	for (i=TOPE; TS[i].tipoentrada==variable && TS[i].tipodato == no_asignado; i--){
		TS[i].tipodato = tipo;
		if((tipo==entero)&&(TS[i].vector==1)){
			TS[i].tipodato = vector_entero;
		}
		if((tipo==flotante)&&(TS[i].vector==1)){
			TS[i].tipodato = vector_flotante;
		}
		if((tipo==boleano)&&(TS[i].vector==1)){
			TS[i].tipodato = vector_boleano;
		}
		if((tipo==caracter)&&(TS[i].vector==1)){
			TS[i].tipodato = vector_caracter;
		}
		
	}
		
}

void BuscarParametroRepetido (char *lexema) {
	/* Comprobar si ya existe otro parametro con el mismo lexema en el mismo
	   procedimiento*/

	int i;
	int igual= 0;

	for (i=TOPE; TS[i].tipoentrada != procedimiento; i--){
		igual = strcmp(TS[i].lexema, lexema);
		if (!igual) {
			printf ("\nError Semantico en la linea %d: Parametro %s esta duplicado\n", yylineno, lexema);
			return;
		}
	}
}

int BuscarParametroRepetido2 (char *lexema){
	 /* Comprobar si ya existe otro parametro con el mismo lexema en el mismo
	   procedimiento*/

	int i;
	int igual= 0;

	for (i=TOPE; TS[i].tipoentrada != procedimiento; i--){
		igual = strcmp(TS[i].lexema, lexema);
		if (!igual) {
			printf ("\nError Semantico en la linea %d: Parametro %s esta duplicado\n", yylineno, lexema);
			return 1;
		}
	}
	return 0;
}


void CuentaParametros () {
	/* Asigna el tipo vacio al procedimiento y  cuenta cuantos parametros tiene */
	int i;
	int parametros=0;

	for (i=TOPE;TS[i].tipoentrada != procedimiento;i--)
		if (TS[i].tipoentrada == parametro) {
			parametros++;
		}

	TS[i].tipodato=vacio;
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

	TS[TOPE].tipoentrada = marca;
	for (i=TOPE-1; i>=0 && TS[i].tipoentrada != marca && TS[i].tipoentrada != procedimiento; i--)
		if (TS[i].tipoentrada == parametro) {
			/* Hemos encontrado un parametro, incluirlo como variable */
			InsertarElemento (variable, TS[i].lexema);
			asignarTipo (TS[i].tipodato);
			if(TS[i].vector==1){
				esVector();
				actualizaVector(TS[i].dim1,TS[i].dim2,TS[i].ndim);
			}
		}
	//MostrarTS();
}

void IntroFinBloq () {
	/* Elimina entradas de la tabla de simbolos hasta que se encuentra
	    una marca de comienzo de bloque o se queda la tabla de simbolos
	   vacia */

	for (;TOPE>=0 && TS[TOPE].tipoentrada!=marca;TOPE--);
	if (TOPE!=0)
		TOPE--;
	//MostrarTS();
	
}

void existeProc (char *lexema) {
/*comprueba que existe un nombre de procedimiento declarado igual al que se le pasa*/
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0 && TS[TOPE].tipoentrada!=marca; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema))
			igual=1;

	if(igual==0)
		printf ("\nError Semantico en la linea %d: El procedimiento %s no est?declarado\n", yylineno, lexema);
}


int existeProc2 (char *lexema) {
/*comprueba que existe un nombre de procedimiento declarado igual al que se le pasa*/
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0 && TS[TOPE].tipoentrada!=marca; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema))
			igual=1;
	return igual;
}

int numParametros(char* lexema){
	int i;
	int igual=0;
	for (i=TOPE+1; i>=0; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema)){
			return TS[i].parametros;
		}


}

dTipo tipoParametro(int num, char* lexema){
	int i;
	int igual=0;
	for (i=TOPE; i>=0; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema)){
			return (TS[i+num].tipodato);
		}
} 

int dimParametro(int num, char* lexema){
	int i;
	int igual=0;
	for (i=TOPE; i>=0; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema)){
			return (TS[i+num].ndim);
		}
} 

int d1Parametro(int num, char* lexema){
	int i;
	int igual=0;
	for (i=TOPE; i>=0; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema)){
			return (TS[i+num].dim1);
		}
} 

int d2Parametro(int num, char* lexema){
	int i;
	int igual=0;
	for (i=TOPE; i>=0; i--)
		if (TS[i].tipoentrada == procedimiento && !strcmp(TS[i].lexema, lexema)){
			return (TS[i+num].dim2);
		}
} 


int get_dim (char *lexema) {
	/* Devuelve el tipo si existe definida una variable con el nombre especificado
	    */
	long int temp;

	for (temp=TOPE; temp>0 && TS[TOPE].tipoentrada!=marca;temp--)
		if (TS[temp].tipoentrada == variable && !strcmp(TS[temp].lexema, lexema))
			return TS[temp].ndim;

	return 0;
}

int get_d1 (char *lexema) {
	/* Devuelve el tipo si existe definida una variable con el nombre especificado
	    */
	long int temp;

	for (temp=TOPE; temp>0 && TS[TOPE].tipoentrada!=marca;temp--)
		if (TS[temp].tipoentrada == variable && !strcmp(TS[temp].lexema, lexema))
			return TS[temp].dim1;

	return 0;
}

int get_d2 (char *lexema) {
	/* Devuelve el tipo si existe definida una variable con el nombre especificado
	    */
	long int temp;

	for (temp=TOPE; temp>0 && TS[TOPE].tipoentrada!=marca;temp--)
		if (TS[temp].tipoentrada == variable && !strcmp(TS[temp].lexema, lexema))
			return TS[temp].dim2;

	return 0;
}


dTipo get_tipo (char *lexema) {
	/* Devuelve el tipo si existe definida una variable con el nombre especificado
	    */
	long int temp;

	for (temp=TOPE; temp>0 && TS[TOPE].tipoentrada!=marca;temp--)
		if (TS[temp].tipoentrada == variable && !strcmp(TS[temp].lexema, lexema))
			return TS[temp].tipodato;
}

dTipo tipoVector (dTipo v) {
	/* Devuelve el tipo del vector
	    */
	if(es_vector(v)==1){
		if(v==vector_boleano)
			return boleano;
		if(v==vector_entero)
			return entero;
		if(v==vector_flotante)
			return flotante;
		if(v==vector_caracter)
			return caracter;
	}
	else{
		return v;
	}	

}

int es_vector(dTipo v){
	if((v==vector_boleano)||(v==vector_entero)||(v==vector_flotante)||(v==vector_caracter))
		return 1;
	else
		return 0;
}

dTipo tipoEnVector(dTipo v){
		
		if(v==boleano)
			return vector_boleano;
		if(v==entero)
			return vector_entero;
		if(v==flotante)
			return vector_flotante;
		if(v==caracter)
			return vector_caracter;

		return v;

}