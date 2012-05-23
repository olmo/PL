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


"Y"						{printf ("OPBINARIO-Y "); yylval.lexema=strdup(yytext); return OP_AND;}
"O"						{printf ("OPBINARIO-O "); yylval.lexema=strdup(yytext); return OP_OR;}
"="        				{printf ("OPBINARIO-= "); yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_IGUALDAD;}
"<>"					{printf ("OPBINARIO-<> "); yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_IGUALDAD;}
"<"						{printf ("OPBINARIO-< "); yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
">"						{printf ("OPBINARIO-> "); yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
"<="					{printf ("OPBINARIO-<= "); yylval.atrib=2; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
">="					{printf ("OPBINARIO->= "); yylval.atrib=3; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
"*"						{printf ("OPBINARIO-* "); yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_MULDIV;}
"/"						{printf ("OPBINARIO-/ "); yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_MULDIV;}
"div"					{printf ("OPBINARIO-div "); yylval.atrib=2; yylval.lexema=strdup(yytext); return OP_MULDIV;}

"no"					{printf ("OPUNARIO-no "); yylval.atrib=0; yylval.lexema=strdup(yytext); return OPUNARIO;}
"&"						{printf ("OPUNARIO-& "); yylval.atrib=1; yylval.lexema=strdup(yytext); return OPUNARIO;}
"#"						{printf ("OPUNARIO-# "); yylval.atrib=2; yylval.lexema=strdup(yytext); return OPUNARIO;}
"!"						{printf ("OPUNARIO-! "); yylval.atrib=3; yylval.lexema=strdup(yytext); return OPUNARIO;}

"entero"			    {printf ("TIPOSIMPLE-entero "); yylval.atrib=0; yylval.tipo=entero; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"real"					{printf ("TIPOSIMPLE-real "); yylval.atrib=1; yylval.tipo=real; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"booleano"				{printf ("TIPOSIMPLE-bool "); yylval.atrib=2; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"caracter"				{printf ("TIPOSIMPLE-caracter "); yylval.atrib=3; yylval.tipo=caracter; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}

{signo}					{printf ("SIGNO "); yylval.lexema=strdup(yytext); return SIGNO;}

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
":="					{printf ("ASIGNADOR "); yylval.lexema=strdup(yytext); return ASIGNADOR;}
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
"pila"					{printf ("PILA "); yylval.lexema=strdup(yytext); return PILA;}

\"[^\"]*\"				{printf("CADENA "); yylval.tipo=cadena; yylval.lexema=strdup(yytext); return CADENA;}

"verdad"				{printf ("CONSTANTE-verdad "); yylval.atrib=0; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return CONSTANTE;}
"falso"					{printf ("CONSTANTE-falso "); yylval.atrib=1; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return CONSTANTE;}

{entero}				{printf("CONSTANTE-entero "); yylval.atrib=2; yylval.tipo=entero; yylval.lexema=strdup(yytext); return CONSTANTE;}
{real}					{printf("CONSTANTE-real "); yylval.atrib=3; yylval.tipo=real; yylval.lexema=strdup(yytext); return CONSTANTE;}
{caracter}				{printf("CONSTANTE-caracter "); yylval.atrib=4; yylval.tipo=caracter; yylval.lexema=strdup(yytext); return CONSTANTE;}


({letra}|"_")({letra}|{numero})* {printf ("IDENTIFICADOR "); yylval.lexema= strdup(yytext); return IDENTIFICADOR;}

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
