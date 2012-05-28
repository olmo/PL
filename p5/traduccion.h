
#include "tablaSimbolos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char * nombre_fichero = "out.c";
FILE *fichOut;
FILE *fichProc;
FILE *actual;
char  temp[11];
char  temp1[11];
char temp2[11];
unsigned int etiqueta_base =0;
unsigned int temporal_base =0;
unsigned int almacena_temp=0;
int main_llave=0;
int en_fun=0;
char** listaConstantes;
int utilConstantes;

typedef struct{
char EtiquetaEntrada[15];
char EtiquetaSalida[15];
char EtiquetaElse[15];
char NombreVarControl[15];
}DescriptorControl;


DescriptorControl tablaControl[200];
int indice=-1;
int temporales[500];
int indTemp=-1;



void libera_temporales(){
	temporal_base=0;
}

void inserta_marcatemp(){
	indTemp++;
	
	temporales[indTemp] = temporal_base;
	libera_temporales();
}

void elimina_marcatemp(){
	indTemp--;
	temporal_base=	temporales[indTemp];
}

int get_temp(){
	
	return temporales[indTemp];

}

void initListaConstantes(){
	listaConstantes = (char**)malloc(100*sizeof(char*));
	utilConstantes = 0;
}

void addConstante(char* c){
	listaConstantes[utilConstantes] = (char*)malloc((strlen(c)+1)*sizeof(char));
	strcpy(listaConstantes[utilConstantes++], c);
}	

void anadirDescriptor(DescriptorControl d){
	indice++;
	tablaControl[indice]=d;

}

void borrarDescriptor(){
	indice--;
}

DescriptorControl get_descriptor(){
	DescriptorControl d=tablaControl[indice];
	return d;
}



char *tipo_actual;

void abrir_ficheros(){

	fichOut = fopen(nombre_fichero,"w");
	fichProc = fopen("dec_fun","w");
	fprintf(fichProc,"#include <stdio.h>\n\n");
	actual = fichOut;
}

void cerrar_fichero(){

	fclose(fichOut);
	fclose(fichProc);
}

void seleccionar_fOut(){
	
	temporal_base=almacena_temp;
	fichProc=actual;
	actual = fichOut;
	
}

void seleccionar_fProc(){
	
	almacena_temp=temporal_base;

	fichOut=actual;
	actual = fichProc;
}

void escribir_cabecera(){//hay que añadir los includes
	fprintf(fichOut,"#include \"dec_fun\"\n#include \"Stack.h\"\n#include \"dec_dat.h\"\n#include <stdio.h>\n\nint main()");

}

void cerrar_main(){
	fprintf(fichOut,"return 0;\n");
}
char* TraducirValor(char* valor){
	if(!strcmp(valor, "verdad"))
		return "1";
	else if(!strcmp(valor, "falso"))
		return "0";
	else return valor;
}
char* TraducirTipo(dtipo tipo, int isPila){
	char* tipoChar = (char*)malloc(15*sizeof(char));
	if(isPila){
		if(tipo == 0 || tipo == 2 || tipo == 3)
			strcpy(tipoChar, "Stack");
		else if(tipo == 1)
			strcpy(tipoChar, "PilaFloat");
	}else{
		if(tipo == 0 || tipo == 3){
			strcpy(tipoChar, "int");
		}else if(tipo == 1){
			strcpy(tipoChar, "float");
		}else if(tipo == 2){
			strcpy(tipoChar, "char");
		}else if(tipo == 4 || tipo == 7 || tipo == 6){
			strcpy(tipoChar, "Stack");
		}else if(tipo == 5){
			strcpy(tipoChar, "PilaFloat");
		}
	}
	
	return tipoChar;
}

void escribir_llaveA(FILE* fich){

	fprintf(fich,"{\n");
	
	main_llave++;
	
}

void escribir_llaveC(FILE* fich){
	fprintf(fich,"}\n");
}

void escribir_1var(FILE* fich, char *nombre){
	
	fprintf(fich,"%s %s;\n",tipo_actual,nombre);
	
	if(!strcmp(tipo_actual, "Stack"))
		fprintf(fich, "stack_init(&%s);\n", nombre);
	else if(!strcmp(tipo_actual, "PilaFloat"))
		fprintf(fich, "init(&%s);\n", nombre);
}


void actualiza_tipo(char* tipo){
	tipo_actual = tipo;
}

void escribir(FILE* fich, char *el){
	fprintf(fichOut,"%s",el);

}

void escribe_coma(){
	fprintf(fichOut,", ");
}

void escribirListaParametros(FILE* fich, char* tipo, int num){
	actualiza_tipo(tipo);
	int i;
	
	for(i=TOPE; TS[i].entrada == parametro && num >0; i--, num--)
		fprintf(fich,"%s %s, ", tipo, TS[i].nombre);
		
	fseek(fich, -2, SEEK_CUR); 	//Nos colocamos sobre la ultima coma (,)
	
	fprintf(fich, " ");				//Se borra la ultima coma
	
} 
void escribirVariables(FILE* fich, char *tipo){
	actualiza_tipo(tipo);
	int i;
	
	for (i=TOPE; TS[i].entrada==variable && TS[i].tipoDato == no_asignado; i--){
			escribir_1var(fich, TS[i].nombre);
	}

	fprintf(fich,"\n");
}

copiaTo(char *m1, char* m2,int n){
int i=0;
	for(i;i<=n;i++){
		m2[i]=m1[i];
	}
}

void copiaToTemp(char *m){
	copiaTo(m, temp, 11);
}
void genera_temporal(){
	char aux[11]= "temp";
	char aux2[3];
	sprintf(aux2,"%d",temporal_base);
	
	strcat(aux,aux2);

	temporal_base++;
	copiaToTemp(aux);
}

void generaEtiqueta(char * m){
	char aux[15]= "etiqueta";
	char aux2[5];
	sprintf(aux2,"%d",etiqueta_base);
	
	strcat(aux,aux2);

	etiqueta_base++;
	copiaTo(aux,m,15);
}


void copiaToTemp1(char *m){
	copiaTo(m, temp1, 11);
}

void copiaToTemp2(char *m){
	copiaTo(m, temp2, 11);
}


escribirExpresionBinaria(FILE* fich, char *elem1, char*elem2, char*op, char*tip){
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	genera_temporal();

	fprintf(fich,"%s %s;\n",tip,temp);
	fprintf(fich,"%s = %s %s ",temp, temp1, op);	
	fprintf(fich,"%s;\n",temp2);
}

void escribirAsignacion(FILE* fich, char *elem1, char*elem2){
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	
	fprintf(fich,"%s = ",temp1);
	fprintf(fich,"%s;\n\n",elem2);
}

void escribirIf(FILE* fich, char *elem1){
	DescriptorControl d;
	
	generaEtiqueta(d.EtiquetaSalida);
	generaEtiqueta(d.EtiquetaElse);	
	anadirDescriptor(d);
	copiaToTemp1(elem1);
	
	fprintf(fich,"if(!%s)goto %s;\n\n",temp1,d.EtiquetaElse);	
}

void escribirElse(FILE* fich){
	DescriptorControl d=get_descriptor();

	fprintf(fich,"goto %s;\n\n",d.EtiquetaSalida);
	fprintf(fich,"%s:\n;\n",d.EtiquetaElse);
}

void escribe_case(FILE *fich, char *elem){
	DescriptorControl d;
	
	//generaEtiqueta(d.EtiquetaSalida);
	
	int h=strlen(elem);
	copiaTo(elem,d.NombreVarControl, h);
	
	anadirDescriptor(d);
	genera_temporal();
	copiaToTemp1(elem);
}

escribirExpresionUnariaPila(FILE* fich, char *pila, char*operacion, dtipo tipo){
	copiaToTemp1(pila);
	genera_temporal();
	
	if(!strcmp(operacion, "pop")){
		fprintf(fich,"%s %s;\n",TraducirTipo(tipoEnPila(tipo), 0),temp);
		if(tipo == real || tipo == pila_real)
			fprintf(fich,"%s = *pop(&%s);\n",temp,temp1);
		else if (tipo == caracter || tipo == pila_caracter)
			fprintf(fich,"%s = *stack_pop(&%s, 0);\n",temp,temp1);
		else fprintf(fich,"%s = *stack_pop(&%s, 0);\n",temp,temp1);
	}else if(!strcmp(operacion, "top")){
		fprintf(fich,"%s %s;\n",TraducirTipo(tipoPila(tipo), 0),temp);
		if(tipo == real || tipo == pila_real)
			fprintf(fich,"%s = tope(&%s);\n",temp,temp1);
		else if (tipo == caracter || tipo == pila_caracter)
			fprintf(fich,"%s = (char)stack_top(&%s);\n",temp,temp1);
		else fprintf(fich,"%s = (int)stack_top(&%s);\n",temp,temp1);
	}else if(!strcmp(operacion, "empty")){
		fprintf(fich,"%s %s;\n",TraducirTipo(entero, 0),temp);
		if(tipo == real || tipo == pila_real)
			fprintf(fich,"%s = vacia(&%s);\n",temp,temp1);
		else if (tipo == caracter || tipo == pila_caracter)
			fprintf(fich,"%s = stack_empty(&%s);\n",temp,temp1);
		else fprintf(fich,"%s = stack_empty(&%s);\n",temp,temp1);
	}
}

escribirExpresionUnaria(FILE* fich, char *elem1,char*op,char *tip){
	copiaToTemp1(elem1);
	genera_temporal();

	fprintf(fich,"%s %s;\n",tip,temp);
	fprintf(fich,"%s = %s %s;\n",temp,  op,temp1);
}

void escribirWhile(FILE* fich, char *elem1){
	DescriptorControl d;
	etiqueta_base--;
	generaEtiqueta(d.EtiquetaEntrada);
	generaEtiqueta(d.EtiquetaSalida);
	anadirDescriptor(d);
	copiaToTemp1(elem1);
	fprintf(fich,"\nif(!%s)goto %s;\n\n",temp1,d.EtiquetaSalida);
}

void finWhile(FILE* fich){

	DescriptorControl d=get_descriptor();

	fprintf(fich,"goto %s;\n",d.EtiquetaEntrada);
	fprintf(fich,"%s:\n;\n",d.EtiquetaSalida);
	
	borrarDescriptor();
}

void escribirEtiqueta(FILE* fich, char* etiqueta){
	fprintf(fich, "%s :\n;\n", etiqueta);
}

void escribirInicializacion(FILE* fich, int num, char* valor){
	
	int i;
	
	if(TS[TOPE].pila){
		for(i=0; i<num; i++){
			int j;
			for(j=utilConstantes-1; j>=0; j--){
				if(TS[TOPE-i].tipoDato == real || TS[TOPE-i].tipoDato ==pila_real)
					fprintf(actual, "push(&%s, %s);\n", TS[TOPE-i].nombre, listaConstantes[j]);
				else fprintf(actual, "stack_push(&%s, (void*)%s);\n", TS[TOPE-i].nombre, listaConstantes[j]);
				
			}
		}
	}else{
		for(i=0; i<num; i++){
			fprintf(fich, "%s = %s;\n", TS[TOPE-i].nombre, valor);
		}
	}

}
void escribe_void_vacio(char *iden ){
	fprintf(fichOut,"\nvoid %s()",iden ); 
}

void escribe_argumentos(char* nomProc){

	int args = numParametros(nomProc);
	int i=0;
	int primero = 1;
	for(i=TOPE-args+1;i<=TOPE;i++){
		if(primero==1){
			fprintf(fichOut,"%s",MostrarTipo(TS[i].tipoDato)); 
			primero =0;
		}
		else{
			fprintf(fichOut,", %s",MostrarTipo(TS[i].tipoDato)); 	
		}
		fprintf(fichOut," %s",TS[i].nombre); 
		if(TS[i].pila==1){//si es pila
			//Acciones para escribir pilas
			;
		}
	}
	fprintf(fichOut,")"); 

}


void escribirProc(char *iden){
	fprintf(fichOut,"\nvoid %s(",iden); 
}



void escribe_cout(FILE* fich, dtipo tip, char *elem){
	if((tip==0)||(tip==3)){
		fprintf(fich,"printf(\"%%d\",%s);\n",elem);
	}
	if(tip==1){
		fprintf(fich,"printf(\"%%f\",%s);\n",elem);
	}
	if(tip==2){
		fprintf(fich,"printf(\"%%c\",%s);\n",elem);
	}
	if((tip==4)||(tip==6)){
		fprintf(fich,"printPila(&%s, ENTERO);\n",elem);
	}
	if(tip==5){
		fprintf(fich,"printPilaF(&%s);\n",elem);
	}
	if(tip==7){
		fprintf(fich,"printPila(&%s, CHAR);\n",elem);
	}
	if(tip==8){
		fprintf(fich,"printf(\%s);\n",elem);
	}

}


void escribe_cin(FILE* fich, dtipo tip, char *elem){
	if((tip==0)||(tip==3)){
		fprintf(fich,"scanf(\"%%i\",&%s);\n",elem);
	}
	if(tip==1){
		fprintf(fich,"scanf(\"%%f\",&%s);\n",elem);
	}
	if(tip==2){
		fprintf(fich,"scanf(\"%%c\",&%s);\n",elem);		
	}
	
	

}
