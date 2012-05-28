programa
inicio
inivar
	n, curr: entero;
finvar

escribir "Introduce un numero: ";
leer n;
escribir " ",n," == ";

mientras curr <= n hacer
inicio
	inivar
		d: entero := n/curr;
	finvar
	
	si d*curr = n entonces
		inicio
			escribir "* ",curr," ";
			n := n/curr;
		fin
	sino
		curr := curr+1;
	fin
fin

escribir "\n";

fin.