programa
inicio
	inivar
		n, curr, ultim, cuenta, primero: entero;
	finvar
	
procedimiento proc1(x: entero)
inicio
inivar
	pe: pila entero := {5, 4, 3, 2, 1};
	pf: pila real;
	pc: pila caracter;
	pb: pila booleano;
finvar
	pe := pe++10;
	pf := pf++10.56;
	pc := pc++'a';
	pb := pb++verdad;
	
	escribir "Tope de pe ", #pe, "\n";
	escribir "Tope de pf ", #pf, "\n";
	escribir "Tope de pc ", #pc, "\n";
	escribir "Tope de pb ", #pb, "\n";
	
	pe := &pe;
	pf := &pf;
	pc := &pc;
	pb := &pb;
	
	pe := pe++50;
	pe := pe++8;
	escribir "Tope de pe ", #pe, "\n";
	escribir pe-5, "\n";
	
	si #(pe++20)=20 entonces
		escribir "Pila pe vacia\n";
		
	si !pf entonces
		escribir "Pila pf vacia\n";
		
	si !pc entonces
		escribir "Pila pc vacia\n";
		
	si !pb entonces
		escribir "Pila pb vacia\n";
	
fin;
	
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
	
	leer n;
	
	caso n de
		1: proc1(n);
		2: escribir "wasa";
		3: escribir "nueve u once";
		sino escribir "fin";
	fin
	
	
fin.