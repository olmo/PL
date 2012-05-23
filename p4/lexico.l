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

(\(\*[^\*\)]*\*\))		{ linea_actual+=countNewLines(yytext);}


"Y"						{ yylval.lexema=strdup(yytext); return OP_AND;}
"O"						{ yylval.lexema=strdup(yytext); return OP_OR;}
"="        				{ yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_IGUALDAD;}
"<>"					{ yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_IGUALDAD;}
"<"						{ yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
">"						{ yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
"<="					{ yylval.atrib=2; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
">="					{ yylval.atrib=3; yylval.lexema=strdup(yytext); return OP_RELACIONAL;}
"*"						{ yylval.atrib=0; yylval.lexema=strdup(yytext); return OP_MULDIV;}
"/"						{ yylval.atrib=1; yylval.lexema=strdup(yytext); return OP_MULDIV;}
"div"					{ yylval.atrib=2; yylval.lexema=strdup(yytext); return OP_MULDIV;}

"no"					{ yylval.atrib=0; yylval.lexema=strdup(yytext); return OPUNARIO;}
"&"						{ yylval.atrib=1; yylval.lexema=strdup(yytext); return OPUNARIO;}
"#"						{ yylval.atrib=2; yylval.lexema=strdup(yytext); return OPUNARIO;}
"!"						{ yylval.atrib=3; yylval.lexema=strdup(yytext); return OPUNARIO;}

"entero"			    { yylval.atrib=0; yylval.tipo=entero; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"real"					{ yylval.atrib=1; yylval.tipo=real; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"booleano"				{ yylval.atrib=2; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}
"caracter"				{ yylval.atrib=3; yylval.tipo=caracter; yylval.lexema=strdup(yytext); return TIPOSIMPLE;}

{signo}					{ yylval.lexema=strdup(yytext); return SIGNO;}

"("						{return PARIZQ;}
")"						{return PARDER;}
"{"						{return LLAVEIZQ;}
"}"						{return LLAVEDER;}

","						{return COMA;}
";"						{return PUNTOCOMA;}
"."						{return PUNTO;}
":"						{return DOSPUNTOS;}
"si"					{return SI;}
"sino"					{return SINO;}
"mientras"				{return MIENTRAS;}
"leer"					{return LEER;}
"escribir"				{return ESCRIBIR;}
":="					{ yylval.lexema=strdup(yytext); return ASIGNADOR;}
"programa"				{return PROGRAMA;}
"inicio"				{return INICIO;}
"fin"					{return FIN;}
"hacer"					{return HACER;}
"procedimiento"			{return PROCEDIMIENTO;}
"inivar"				{return INIVAR;}
"finvar"				{return FINVAR;}
"caso"					{return CASO;}
"de"					{return DE;}
"entonces"				{return ENTONCES;}
"pila"					{ yylval.lexema=strdup(yytext); return PILA;}

\"[^\"]*\"				{yylval.tipo=cadena; yylval.lexema=strdup(yytext); return CADENA;}

"verdad"				{ yylval.atrib=0; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return CONSTANTE;}
"falso"					{ yylval.atrib=1; yylval.tipo=booleano; yylval.lexema=strdup(yytext); return CONSTANTE;}

{entero}				{yylval.atrib=2; yylval.tipo=entero; yylval.lexema=strdup(yytext); return CONSTANTE;}
{real}					{yylval.atrib=3; yylval.tipo=real; yylval.lexema=strdup(yytext); return CONSTANTE;}
{caracter}				{yylval.atrib=4; yylval.tipo=caracter; yylval.lexema=strdup(yytext); return CONSTANTE;}


({letra}|"_")({letra}|{numero})* { yylval.lexema= strdup(yytext); return IDENTIFICADOR;}

\n						{++linea_actual;}
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
