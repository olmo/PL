programa
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
	
fin.
