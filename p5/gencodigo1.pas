programa
inicio
inivar
	n, curr: entero := 2;
finvar

escribir "Introduce un numero: ";
leer n;
escribir " ",n," == ";
curr := 2;

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

escribir "\n";

fin.
