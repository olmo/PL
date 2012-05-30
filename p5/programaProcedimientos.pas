programa
inicio
	inivar
		n: entero;
	finvar
	
procedimiento proc1(x1: entero; x2: entero)
inicio
inivar
	aux : entero;
finvar
	n := x1*x2;
	
fin;

procedimiento proc2(x1: entero; x2: entero)
inicio
inivar
	aux : entero;
finvar
	procedimiento proc3()
	inicio
		aux := 3;
	fin;

	proc3();
	n := x1-x2+aux;
	
fin;


	leer n;
	proc1(2,n);
	escribir n;
	proc2(2,n);
	escribir n;
	proc3();
	
fin.