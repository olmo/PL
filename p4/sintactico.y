%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tablaSimbolos.h"

/**  La siguiente declaracion permite que 'yyerror' se pueda invocar desde el 
***  fuente de lex (prueba.l)
**/

void yyerror( char * msg ) ;

/**  La siguiente variable se usar? para conocer el numero de la l?nea
***  que se esta leyendo en cada momento. Tambi?n es posible usar la variable 
***  'yylineno' que tambi?n nos muestra la l?nea actual. Para ello es necesario
***  invocar a flex con la opci�n '-l' (compatibilidad con lex).
**/

int linea_actual = 1 ;
dtipo tipoPorAsignar;
int pila=0;
int argumentos =1;
int correcto=1;
char *nomproc;
int numparam=0;
char *iden;
dtipo iden_tipo;
dtipo tipoAux;

dtipo tipo_pila;		//Diferente al nombre del procedimiento tipoPila
int pilaError = 0;
%}

/**  Para uso de mensajes de error sint�ctico con BISON.
***  La siguiente declaraci�n provoca que 'bison' produzca mensajes de error con 
***  indicacion de los tokens que se esperaban en el lugar del que produjo el 
***  error (S�LO FUNCIONA CON BISON>=2.1). Para Bison<2.1 es mediante
***  #define YYERROR_VERBOSE
**/

%error-verbose


/**  A continuaci�n declaramos los nombres simb�licos de los tokens.
***  byacc se encarga de asociar a cada uno un c�digo
**/

%token IDENTIFICADOR
%token CONSTANTE 
%token OPUNARIO 
%token OP_MULDIV
%token SIGNO 
%token INIVAR 
%token FINVAR 
%token TIPOSIMPLE
%token PILA 
%token MIENTRAS 
%token PUNTO 
%token CADENA 
%token ESCRIBIR 
%token LEER 
%token CASO
%token DE 
%token SI 
%token HACER 
%token ENTONCES 
%token SINO 
%token PARIZQ 
%token PARDER 
%token INICIO 
%token FIN 
%token ASIGNACION 
%token PROGRAMA 
%token DOSPUNTOS 
%token PUNTOCOMA 
%token COMA 
%token LLAVEIZQ 
%token LLAVEDER 
%token PROCEDIMIENTO
%token OP_OR
%token OP_AND 
%token OP_RELACIONAL 
%token OP_IGUALDAD 
%token OPMASMAS

%left OPMASMAS
%left OP_OR
%left OP_AND
%left OP_IGUALDAD
%left OP_RELACIONAL
%left SIGNO
%left OP_MULDIV
%right OPUNARIO

%%

/** Secci�n de producciones que definen la gram�tica.
**/


programa : PROGRAMA bloque PUNTO;

bloque : INICIO {IntroIniBloq();} declar_de_variables_locales declar_de_subprogs sentencias FIN {IntroFinBloq();};
		
declar_de_subprogs : | declar_de_subprogs declar_subprog;

declar_subprog : cabecera_subprograma bloque PUNTOCOMA;

declar_de_variables_locales : | INIVAR  variables_locales FINVAR;

variables_locales : cuerpo_declar_variables variables_locales | cuerpo_declar_variables;

cuerpo_declar_variables : asdf PUNTOCOMA | error; 

asdf : lista_variables DOSPUNTOS tipo opcion_asign_variable | error;

opcion_asign_variable : ASIGNACION expresion | ;

lista_variables : IDENTIFICADOR lista_identificador {if(es_repetida($1.lexema)==0){
														InsertarElemento(variable, $1.lexema);} 
													}
				| error;

lista_identificador : | COMA IDENTIFICADOR lista_identificador {if(es_repetida($2.lexema)==0){
														InsertarElemento(variable, $2.lexema);} 
													};

cabecera_subprograma : PROCEDIMIENTO IDENTIFICADOR PARIZQ { 
								if(es_repetida($2.lexema)==0){
									InsertarElemento(procedimiento,$2.lexema);asignarTipo (desconocido, TOPE);
								} 
							}
						variables_subprograma {CuentaParametros ();} PARDER;

variables_subprograma : | lista_parametros DOSPUNTOS tipo lista_variables_subprograma | error;

													
lista_variables_subprograma : | PUNTOCOMA lista_parametros DOSPUNTOS tipo lista_variables_subprograma | error;

lista_parametros : IDENTIFICADOR lista_identificador_parametros {if(es_repetida($1.lexema)==0){
														InsertarElemento(parametro, $1.lexema);} 
													}
				| error;

lista_identificador_parametros : | COMA IDENTIFICADOR lista_identificador_parametros {if(es_repetida($2.lexema)==0){
														InsertarElemento(parametro, $2.lexema);} 
													};

sentencias : | sentencias sentencia;

sentencia : bloque | asignacion_procedimiento | sentencia_if | sentencia_switch | sentencia_while | sentencia_entrada | sentencia_salida | error;

asignacion_procedimiento: IDENTIFICADOR {iden = $1.lexema;} procedimientoOasignacion ;

procedimientoOasignacion: llamada_procedimiento | sentencia_asignacion;

llamada_procedimiento : PARIZQ {
	if(existeProc(iden) == 0){
		printf("\nError Semantico en la linea %d: no existe ningun procedimiento llamado %s\n", yylineno, iden);
	}else{
		numparam = numParametros(iden);
	}
	} lista_parametros_procedimiento PARDER PUNTOCOMA{
	if(existeProc(iden)){
		if(numParametros(iden) != argumentos){
			printf("\nError Semantico en la linea %d: El procedimiento %s admite %d parametros, pero se le pasaron %d\n", yylineno, iden, numParametros(iden), argumentos);
		}
	}
	argumentos = 1;
	numparam = 0;
	}

lista_parametros_procedimiento: expresion {
	$$.numArgumentos = 1;
	
	if($$.numArgumentos <= numparam){
		if(tipoParametro($$.numArgumentos, iden) != $1.tipo){
			printf("\nError Semantico en la linea %d: El parametro %s ha de ser de tipo %s.\n", yylineno, $1.lexema, MostrarTipo(tipoParametro($$.numArgumentos, iden)));}
		}
		} | lista_parametros_procedimiento COMA expresion{
			$$.numArgumentos = 1+$1.numArgumentos;
			if($$.numArgumentos <= numparam){
				if(tipoParametro($$.numArgumentos, iden) != $3.tipo){ 
					printf("\nError Semantico en la linea %d: El parametro %s ha de ser de tipo %s.\n", yylineno, $1.lexema, MostrarTipo(tipoParametro($$.numArgumentos, iden)));
				}
			}
			argumentos++;
		};		

sentencia_asignacion : ASIGNACION expresion PUNTOCOMA {
		
		if(existe (iden)==0){
			printf ("\nError Semantico en la linea %d: Identificador %s no esta declarado\n", yylineno, $1.lexema);
		}else{
			iden_tipo=get_tipo (iden);
			
			if(iden_tipo!=$2.tipo)
				printf ("\nError Semantico en la linea %d: Asignacion de tipos incompatibles, no se puede asignar un %s a un %s\n", yylineno,MostrarTipo($2.tipo),MostrarTipo(iden_tipo));
			
		}
	};

sentencia_if : SI expresion {
					if($2.tipo!=booleano){
						printf ("\nError Semantico en la linea %d: Se esperaba una sentencia de tipo boolean, no de tipo %s \n", yylineno,MostrarTipo($2.tipo));
					}
				}
			ENTONCES sentencia sentencia_else;
			
sentencia_else : | SINO sentencia;

sentencia_switch : CASO IDENTIFICADOR {
	if ($2.tipo != entero || $2.tipo != real || $2.tipo != caracter || $2.tipo != booleano)
		printf("\nError Semantico en la linea %d: El tipo de %s es incompatible con la estructura CASO.\n", yylineno, $2.lexema);
	else{ tipoAux = $2.tipo;};
	}DE lista_variables_switch{
	if( $4.tipo != $2.tipo ){
		printf("\nError Semantico en la linea %d: El tipo de la lista de variables es incompatible con el tipo de %s\n", yylineno, $2.lexema);
	}
	}DOSPUNTOS sentencia lista_variables_switch{
	if( $7.tipo != $2.tipo ){
		printf("\nError Semantico en la linea %d: El tipo de la lista de variables es incompatible con el tipo de %s\n", yylineno, $2.lexema);
	}
	} DOSPUNTOS sentencia lista_sentencia_switch opcion_switch_sino FIN;

opcion_switch_sino : |  SINO sentencia;

lista_sentencia_switch : | lista_variables_switch{
		if( tipoAux != $1.tipo){
			printf("\nError Semantico en la linea %d: El tipo de la lista de variables es incompatible\n", yylineno);
		}
		} DOSPUNTOS sentencia lista_sentencia_switch; 

lista_variables_switch: lista_variables_en_switch {$$.tipo = $1.tipo;} | CONSTANTE {$$.tipo = $1.tipo;} lista_constantes{
						if($2.tipo != $1.tipo){
							printf("\nError Semantico en la linea %d: Se esperaba una constante del mismo tipo que %s\n", yylineno, $1.lexema);
						}
					};
					
lista_variables_en_switch : IDENTIFICADOR lista_identificador_en_switch {
	if(existe($1.lexema)==0){
		printf("\nError Semantico en la linea %d: La variable %s no esta definida\n", yylineno, $1.lexema);} 
	else if($1.tipo != $2.tipo){
		printf("\nError Semantico en la linea %d: La variable %s no es del mismo tipo que el resto de la lista\n", yylineno, $1.lexema);} 
	else{
		$$.tipo = $2.tipo;
		}
	}
				| error;

lista_identificador_en_switch : | COMA IDENTIFICADOR lista_identificador_en_switch {
				if(existe($2.lexema)==0){
					printf("\nError Semantico en la linea %d: La variable %s no esta definida\n", yylineno, $2.lexema);} 
				else if($2.tipo != $3.tipo){
					 printf("\nError Semantico en la linea %d: La variable %s no es del mismo tipo que el resto de la lista\n", yylineno, $2.lexema);} 
				else{
					$$.tipo = $2.tipo;}
				}

sentencia_while : MIENTRAS expresion {
						if($2.tipo!=booleano){
							printf ("\nError Semantico en la linea %d: Se esperaba una sentencia de tipo boolean, no de tipo %s \n", yylineno,MostrarTipo($2.tipo));
						}
					}
				HACER sentencia FIN;

sentencia_entrada : LEER lista_variables_entrada PUNTOCOMA;

lista_variables_entrada: IDENTIFICADOR lista_identificador_entrada {
		if(existe($1.lexema)==0){
			printf("\nError Semantico en la linea %d: La variable %s no esta definida\n", yylineno, $1.lexema);} 
		}
				| error;
				
lista_identificador_entrada : | COMA IDENTIFICADOR lista_identificador_entrada {
		if(existe($2.lexema)==0){
			printf("\nError Semantico en la linea %d: La variable %s no esta definida\n", yylineno, $2.lexema);} 
		};

sentencia_salida : ESCRIBIR lista_expresiones_o_cadena PUNTOCOMA;

lista_expresiones_o_cadena : expresion_cad | expresion_cad COMA lista_expresiones_o_cadena ;

expresion_cad : expresion | CADENA;

expresion : PARIZQ expresion PARDER {
				$$.tipo=$2.tipo;
			}
		
		| OPUNARIO expresion {
				if($1.atrib == 0){		//Operador no
					if($2.tipo!=booleano && ($2.atrib==0 || $2.atrib==1)){
						printf ("\nError Semantico en la linea %d: El operador %s incompatible con tipo: %s, se esperaba boolean. \n", yylineno, $1.lexema, MostrarTipo($2.tipo));
					}else{
						$$.tipo=$2.tipo;
					}
				}else if($1.atrib != 0){
					if(es_pila($2.tipo) ==0 ){
						printf ("\nError Semantico en la linea %d: El operador %s incompatible con tipo: %s, se esperaba pila. \n", yylineno, $1.lexema, MostrarTipo($2.tipo));
					}else{
						if($1.atrib == 1)
							$$.tipo = $2.tipo;
						else if($1.atrib == 2)
							$$.tipo = tipoPila($2.tipo);
						else if($1.atrib == 3)
							$$.tipo = booleano;
					}
				}
			}
		
		| expresion OP_MULDIV expresion {
				if(($1.tipo==booleano || $3.tipo==booleano) || ($1.tipo == cadena || $1.tipo == cadena)){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando con un booleano o cadena. \n", yylineno, $2.lexema);
					correcto = 0;
				}else if(es_pila($1.tipo) ==1 && es_pila($3.tipo) ==1 &&  $1.tipo != $3.tipo){
						printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos de pila incompatibles. \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo) ==1 && es_pila($3.tipo)==0 && tipoPila($1.tipo) != $3.tipo){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos incompatibles . \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo) == 0 && es_pila($3.tipo) ==1 && $1.tipo != tipoPila($3.tipo)){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos incompatibles. \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo)==0 && es_pila($3.tipo)==0 && $1.tipo!=$3.tipo){
					printf("\nError Semantico en la linea %d: Tipo %s incompatible con tipo %s. \n", yylineno, MostrarTipo($1.tipo), MostrarTipo($3.tipo));
				}
			}
			
		| expresion SIGNO expresion {
			if(($1.tipo==booleano || $3.tipo==booleano) || ($1.tipo == cadena || $1.tipo == cadena)){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando con un booleano o cadena. \n", yylineno, $2.lexema);
					correcto = 0;
				}else if(es_pila($1.tipo) ==1 && es_pila($3.tipo) ==1 &&  $1.tipo != $3.tipo){
						printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos de pila incompatibles. \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo) ==1 && es_pila($3.tipo)==0 && tipoPila($1.tipo) != $3.tipo){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos incompatibles . \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo) == 0 && es_pila($3.tipo) ==1 && $1.tipo != tipoPila($3.tipo)){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando entre tipos incompatibles. \n", yylineno, $2.lexema);
						correcto = 0;
				}else if(es_pila($1.tipo)==0 && es_pila($3.tipo)==0 && $1.tipo!=$3.tipo){
					printf("\nError Semantico en la linea %d: Tipo %s incompatible con tipo %s. \n", yylineno, MostrarTipo($1.tipo), MostrarTipo($3.tipo));
				}
			}
		
		| expresion OP_RELACIONAL expresion 
			{ if($1.tipo==caracter || $1.tipo==booleano || $1.tipo==cadena || es_pila($1.tipo) || $3.tipo==caracter || $3.tipo==booleano || $3.tipo==cadena || es_pila($3.tipo)){
				printf("\nError Semantico en la linea %d: El operador %s se esta utilizando como: %s%s%s,  operador incompatible . \n", yylineno, $2.lexema, MostrarTipo($1.tipo),$2.lexema, MostrarTipo($3.tipo));
				correcto = 0;
			}
			else{
				if(($1.tipo)!=($3.tipo)){
					printf ("\nError Semantico en la linea %d: Tipos incompatibles: %s incompatible con %s. \n", yylineno, MostrarTipo($1.tipo), MostrarTipo($3.tipo));
					correcto = 0;
				}else $$.tipo = booleano;
			}
		}
		| expresion OPMASMAS expresion
			{ if(es_pila($1.tipo) == 0){		//La primera expresion no es una pila
				printf("\nError Semantico en la linea %d: %s ha de ser de tipo pila.\n", yylineno, $1.lexema);
				correcto = 0;
			  }else if(tipoPila($1.tipo) != $3.tipo){
			  	printf("\nError Semantico en la linea %d: El tipo de la pila es incompatible con el tipo de %s\n", yylineno, $3.lexema);
			  	correcto = 0;
			  }else $$.tipo = $1.tipo;
			  
			}
		| expresion OP_OR expresion 
			{if(($1.tipo!=booleano)||($3.tipo!=booleano)){
				printf("\nError Semantico en la linea %d: El operador %s se ha de utilizar con tipos booleanos . \n", yylineno, $2.lexema);
				correcto = 0;
			}
			if(correcto==1)
				$$.tipo=booleano;	
			else
				correcto=1;
			}
		| expresion OP_AND expresion 
			{if(($1.tipo!=booleano)||($3.tipo!=booleano)){
				printf("\nError Semantico en la linea %d: El operador %s se ha de utilizar con tipos booleanos . \n", yylineno, $2.lexema);
				correcto = 0;
			}
			if(correcto==1)
				$$.tipo=booleano;	
			else
				correcto=1;
			}
		
		| expresion OP_IGUALDAD expresion {
			if(($1.tipo==cadena)||($3.tipo==cadena)){
				printf("\nError Semantico en la linea %d: El operador %s se esta utilizando como: %s%s%s,  operador incompatible con cadenas y . \n", yylineno, $2.lexema, MostrarTipo($1.tipo),$2.lexema, MostrarTipo($3.tipo));
				correcto = 0;
			}
			else{
				if(($1.tipo)!=($3.tipo)){
					printf ("\nError Semantico en la linea %d: Tipos incompatibles: %s incompatible con %s. \n", yylineno, MostrarTipo($1.tipo), MostrarTipo($3.tipo));
					correcto = 0;
				}else{ $$.tipo = booleano;}
			}
			
		}
		| IDENTIFICADOR {
			if(existe($1.lexema)==0)
				printf("\nError Semantico en la linea %d: El identificador %s no esta declarado\n", yylineno, $1.lexema);
			else $$.tipo = get_tipo($1.lexema);}
		
		| CONSTANTE {$$.tipo = $1.tipo;}
		
		| agregado {$$.tipo = tipoEnPila($1.tipo);}
		
		| SIGNO expresion %prec OPUNARIO {
				if($2.tipo!=entero && $2.tipo!=real){
					printf ("\nError Semantico en la linea %d: El operador %s es incompatible con tipo: %s, se esperaba entero o real. \n", yylineno, $1.lexema, MostrarTipo($2.tipo));}
				else{
					$$.tipo=$2.tipo;
				}
			}
		
		| error;

agregado : LLAVEIZQ CONSTANTE {tipo_pila = $2.tipo;} lista_constantes LLAVEDER {$$.tipo = tipo_pila;};

lista_constantes : | COMA CONSTANTE{ 
	if($2.tipo != tipo_pila)
		printf("Error Semantico en la linea %d: La constante %s tiene un tipo diferente al resto de constantes\n", yylineno, $2.lexema);
	}lista_constantes
	
	;

tipo : TIPOSIMPLE {asignarTipoCascada($1.tipo);} | PILA TIPOSIMPLE {esPila();asignarTipoCascada($2.tipo);} | error{asignarTipoCascada(desconocido);};

%%
/** aqui incluimos el fichero generado por el 'lex'
*** que implementa la funci?n 'yylex'
**/

#ifdef DOSWINDOWS          /* Variable de entorno que indica la plataforma */
#include "lexyy.c"
#else
#include "lex.yy.c"
#endif

/**  se debe implementar la funci?n yyerror. En este caso
***  simplemente escribimos el mensaje de error en pantalla
**/

void yyerror( char *msg )
{
  fprintf(stderr,"\n[Linea %d]: %s\n", linea_actual, msg) ;
}
