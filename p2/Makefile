.SUFFIXES:
prueba: lex.yy.o
	gcc -o prueba lex.yy.o
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: lexico.l
	flex lexico.l
limpia:
	rm -f prueba main.o lex.yy.o lex.yy.c
todo:
	make limpia
	make prueba
