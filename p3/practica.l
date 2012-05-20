%{
#include <stdlib.h>
#include <string.h>
#include "tabla.h"

//int linea_actual=1 ;
int countNewLines(char*s){
	int i, nNL;
	nNL = 0;
	for(i=0; i<strlen(s); i++)
		nNL += s[i]=='\n'?1:0;
	
	return nNL;
}
%}


letra			[a-zA-Z]
numero    		[0-9]
signo			"+"|"-"
alfanum    		[a-zA-Z0-9_]
cadena			\"[^\"]*\"   	/*Cualquier cosa entrecomillada menos las propias comillas (eso significa [^\"])*/
caracter		\'[^\']\'
booleano		"verdad"|"falso"						
real			{numero}+"."{numero}+("e"{signo}?{numero}+)?
entero			{numero}+
tabulador		\t
espacio 		" "
otros    		.


/*Esto indica a Flex que lea sólo un fichero de entrada*/
%option noyywrap
%option yylineno
%%


" "					{}
\t					{}

(\(\*[^\*\)]*\*\))		{printf ("COMENTARIOS "); linea_actual+=countNewLines(yytext);}


"Y"						{printf ("OPBINARIO-Y "); return OP_AND;}
"O"						{printf ("OPBINARIO-O "); return OP_OR;}
"="        				{printf ("OPBINARIO-= "); return OP_IGUALDAD;}
"<>"					{printf ("OPBINARIO-<> "); return OP_IGUALDAD;}
"<"						{printf ("OPBINARIO-< "); return OP_RELACIONAL;}
">"						{printf ("OPBINARIO-> "); return OP_RELACIONAL;}
"<="					{printf ("OPBINARIO-<= "); return OP_RELACIONAL;}
">="					{printf ("OPBINARIO->= "); return OP_RELACIONAL;}
"*"						{printf ("OPBINARIO-* "); return OP_MULDIV;}
"/"						{printf ("OPBINARIO-/ "); return OP_MULDIV;}
"div"					{printf ("OPBINARIO-div "); return OP_MULDIV;}

"no"					{printf ("OPUNARIO-no "); return OPUNARIO;}
"&"						{printf ("OPUNARIO-& "); return OPUNARIO;}
"#"						{printf ("OPUNARIO-# "); return OPUNARIO;}
"!"						{printf ("OPUNARIO-! "); return OPUNARIO;}

"entero"			    {printf ("TIPOSIMPLE-entero "); return TIPOSIMPLE;}
"real"					{printf ("TIPOSIMPLE-real "); return TIPOSIMPLE;}
"booleano"				{printf ("TIPOSIMPLE-bool "); return TIPOSIMPLE;}
"caracter"				{printf ("TIPOSIMPLE-caracter "); return TIPOSIMPLE;}

{signo}					{printf ("SIGNO "); return SIGNO;}

"("						{printf ("PARIZQ ");return PARIZQ;}
")"						{printf ("PARDER ");return PARDER;}
"{"						{printf ("LLAVEIZQ ");return LLAVEIZQ;}
"}"						{printf ("LLAVEDER ");return LLAVEDER;}

","						{printf ("COMA ");return COMA;}
";"						{printf ("PUNTOCOMA ");return PUNTOCOMA;}
"."						{printf("PUNTO\n"); return PUNTO;}
":"						{printf ("DOSPUNTOS ");return DOSPUNTOS;}
"si"					{printf ("SI ");return SI;}
"sino"					{printf ("SINO ");return SINO;}
"mientras"				{printf ("MIENTRAS ");return MIENTRAS;}
"leer"					{printf ("LEER ");return LEER;}
"escribir"				{printf ("ESCRIBIR ");return ESCRIBIR;}
":="					{printf ("ASIGNADOR ");return ASIGNADOR;}
"programa"				{printf ("PROGRAMA ");return PROGRAMA;}
"inicio"				{printf ("INICIO ");return INICIO;}
"fin"					{printf ("FIN ");return FIN;}
"hacer"					{printf ("HACER ");return HACER;}
"procedimiento"			{printf ("PROCEDIMIENTO ");return PROCEDIMIENTO;}
"inivar"				{printf ("INIVAR ");return INIVAR;}
"finvar"				{printf ("FINVAR ");return FINVAR;}
"caso"					{printf ("CASO ");return CASO;}
"de"					{printf ("DE ");return DE;}
"entonces"				{printf ("ENTONCES ");return ENTONCES;}
"pila"					{printf ("PILA "); return PILA;}

\"[^\"]*\"				{printf("CADENA "); return CADENA;}

"verdad"				{printf ("CONSTANTE-verdad "); return CONSTANTE;}
"falso"					{printf ("CONSTANTE-falso "); return CONSTANTE;}

{entero}				{printf("CONSTANTE-entero "); return CONSTANTE;}
{real}					{printf("CONSTANTE-real "); return CONSTANTE;}
{caracter}				{printf("CONSTANTE-caracter "); return CONSTANTE;}


({letra}|"_")({letra}|{numero})* {printf ("IDENTIFICADOR "); return IDENTIFICADOR;}

\n						{++linea_actual; printf ("\n%d  ", linea_actual);}
{otros}					{printf("\n(Linea %d) Error léxico: token %s\n", yylineno, yytext);}

%%

/*main (int argc, char *argv[]) {
	if (argc == 2){
		yyin = fopen (argv[1], "rt");
		if (yyin == NULL){
			printf ("El fichero %s no se puede abrir\n", argv[1]);
			exit (-1);
		}
	}
	
	int val;
	val= yylex() ;
	  
	while (val != 0) {
		printf ("(%d) ", val); val= yylex() ;
	}
	exit (1);
}*/
