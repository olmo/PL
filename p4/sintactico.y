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
***  invocar a flex con la opción '-l' (compatibilidad con lex).
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

dtipo tipo_pila;		//Diferente al nombre del procedimiento tipoPila
int pilaError = 0;
%}

/**  Para uso de mensajes de error sintáctico con BISON.
***  La siguiente declaración provoca que 'bison' produzca mensajes de error con 
***  indicacion de los tokens que se esperaban en el lugar del que produjo el 
***  error (SÓLO FUNCIONA CON BISON>=2.1). Para Bison<2.1 es mediante
***  #define YYERROR_VERBOSE
**/

%error-verbose


/**  A continuación declaramos los nombres simbólicos de los tokens.
***  byacc se encarga de asociar a cada uno un código
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


%left OP_OR
%left OP_AND
%left OP_IGUALDAD
%left OP_RELACIONAL
%left SIGNO
%left OP_MULDIV
%right OPUNARIO

%%

/** Sección de producciones que definen la gramática.
**/


programa : PROGRAMA bloque PUNTO {imprimeTS();};

bloque : INICIO {IntroIniBloq();} declar_de_variables_locales {imprimeTS();}  declar_de_subprogs {imprimeTS();} sentencias FIN {IntroFinBloq();};
		
declar_de_subprogs : | declar_de_subprogs declar_subprog;

declar_subprog : cabecera_subprograma bloque PUNTOCOMA;

declar_de_variables_locales : | INIVAR  variables_locales FINVAR;

variables_locales : cuerpo_declar_variables variables_locales | cuerpo_declar_variables;

cuerpo_declar_variables : lista_variables DOSPUNTOS tipo opcion_asign_variable PUNTOCOMA | error; 

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

llamada_procedimiento : PARIZQ lista_expresiones_o_cadena PARDER PUNTOCOMA;

sentencia_asignacion : ASIGNACION expresion PUNTOCOMA {
		
		if(existe (iden)==0){
			printf ("\nError Semantico en la linea %d: Identificador %s no esta declarado\n", yylineno, $1.lexema);}
		else{
			iden_tipo=get_tipo (iden);
			
			if(iden_tipo!=$3.tipo)
				printf ("\nError Semantico en la linea %d: Asignacion de tipos incompatibles, no se puede asignar un %s a un %s\n", yylineno,MostrarTipo($3.tipo),MostrarTipo(iden_tipo));
			else{
				//Comprobar pila
			}
		}
	};

sentencia_if : SI expresion {
					if($2.tipo!=booleano){
						printf ("\nError Semantico en la linea %d: Se esperaba una sentencia de tipo boolean, no de tipo %s \n", yylineno,MostrarTipo($2.tipo));
					}
				}
			ENTONCES sentencia SINO sentencia 
			| SI expresion {
					if($2.tipo!=booleano){
						printf ("\nError Semantico en la linea %d: Se esperaba una sentencia de tipo boolean, no de tipo %s \n", yylineno,MostrarTipo($2.tipo));
					}
				}
			ENTONCES sentencia;

sentencia_switch : CASO IDENTIFICADOR DE lista_variables_switch DOSPUNTOS sentencia lista_variables_switch DOSPUNTOS sentencia lista_sentencia_switch opcion_switch_sino FIN;

opcion_switch_sino : |  SINO sentencia;

lista_sentencia_switch : | lista_variables_switch DOSPUNTOS sentencia lista_sentencia_switch; 

lista_variables_switch: lista_variables | CONSTANTE lista_constantes;

sentencia_while : MIENTRAS expresion {
						if($2.tipo!=booleano){
							printf ("\nError Semantico en la linea %d: Se esperaba una sentencia de tipo boolean, no de tipo %s \n", yylineno,MostrarTipo($2.tipo));
						}
					}
				HACER sentencia FIN;

sentencia_entrada : LEER lista_variables PUNTOCOMA;

sentencia_salida : ESCRIBIR lista_expresiones_o_cadena PUNTOCOMA;

lista_expresiones_o_cadena : expresion_cad | expresion_cad COMA lista_expresiones_o_cadena ;

expresion_cad : expresion | CADENA;

expresion : PARIZQ expresion PARDER {
				$$.tipo=$2.tipo;
			}
		
		| OPUNARIO expresion {
				if($2.tipo!=booleano && ($2.atrib==0 || $2.atrib==1)){
					printf ("\nError Semantico en la linea %d: El operador %s incompatible con tipo: %s, se esperaba boolean. \n", yylineno, $1.lexema, MostrarTipo($2.tipo));}
				else{
					$$.tipo=$2.tipo;
				}
			}
		
		| expresion OP_MULDIV expresion {
				if($1.tipo==booleano || $3.tipo==booleano){
					printf("\nError Semantico en la linea %d: El operador %s se esta utilizando un booleano. \n", yylineno, $2.lexema);
					correcto = 0;
				}
			}
			
		| expresion SIGNO expresion 
		
		| expresion OP_RELACIONAL expresion 
		
		| expresion OP_OR expresion 
		
		| expresion OP_AND expresion 
		
		| expresion OP_IGUALDAD expresion {
			if(($1.tipo==cadena)||($3.tipo==cadena)){
				printf("\nError Semantico en la linea %d: El operador %s se esta utilizando como: %s%s%s,  operador incompatible con cadenas y . \n", yylineno, $2.lexema, MostrarTipo($1.tipo),$2.lexema, MostrarTipo($3.tipo));
				correcto = 0;
			}
			else{
				if(($1.tipo)!=($3.tipo)){
					printf ("\nError Semantico en la linea %d: Tipos incompatibles: %s incompatible con %s. \n", yylineno, MostrarTipo($1.tipo), MostrarTipo($3.tipo));
					correcto = 0;
				}
			}
			if(correcto==1)
				$$.tipo=booleano;	
			else
				correcto=1;
			}
		
		| IDENTIFICADOR 
		
		| CONSTANTE 
		
		| agregado 
		
		| SIGNO expresion %prec OPUNARIO {
				if($2.tipo!=entero && $2.tipo!=real){
					printf ("\nError Semantico en la linea %d: El operador %s incompatible con tipo: %s, se esperaba entero o real. \n", yylineno, $1.lexema, MostrarTipo($2.tipo));}
				else{
					$$.tipo=$2.tipo;
				}
			}
		
		| error;

agregado : LLAVEIZQ CONSTANTE lista_constantes LLAVEDER;

lista_constantes : | COMA CONSTANTE lista_constantes;

tipo : TIPOSIMPLE {asignarTipoCascada($1.tipo);} | PILA TIPOSIMPLE {esPila();asignarTipoCascada($2.tipo);};

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
