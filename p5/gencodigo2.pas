(* 	Escribe la descomposición de un número entero en sus factores primos,
	usa exclusivamente: multiplicación, división y suma de enteros 
*)
programa
inicio
	inivar
		n, curr, ultim, cuenta, primero: entero;
	finvar
	
	curr := 2;
	ultim := 0;
	cuenta := 0;
	primero := 1;

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
				si curr <> ultim entonces
					inicio
						ultim := curr;
						cuenta := 1;
					fin
				sino
					cuenta := cuenta +1;
				
				n := n/curr;
			fin
		sino
			inicio
				si cuenta > 0 entonces
					inicio
						si primero = 0 entonces
							escribir " *";
						primero := 0;
						escribir " ",curr;
						si cuenta > 1 entonces
							escribir "^",cuenta;
					fin
				
				curr := curr+1;
				cuenta := 0;
			fin
	fin
	
	si cuenta > 0 entonces
		inicio
			si primero = 0 entonces
				escribir " *";
			primero := 0;
			escribir " ",curr;
			si cuenta > 1 entonces
				escribir "^",cuenta;
		fin

	escribir "\n";
fin.