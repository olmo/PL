.SUFFIXES:
prueba: main.o y.tab.o
	gcc -o prueba main.o y.tab.o
y.tab.o: y.tab.c
	gcc -c y.tab.c
main.o: main.c
	gcc -c main.c
y.tab.c: sintactico.y lex.yy.c
	bison -v --report=all --report-file=reportBison.txt -o y.tab.c sintactico.y 
lex.yy.c: practica.l
	flex practica.l
limpia:
	rm -f prueba main.o y.tab.o y.tab.c lex.yy.c
todo:
	make limpia
	make prueba
