
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
	fprintf(fichOut,"#include \"dec_fun.h\"\n#include \"dec_dat.h\"\n#include <stdio.h>\n\nint main()");

}

void cerrar_main(){
	fprintf(fichOut,"return 0;\n");
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

void escribir_llaveC(){
	main_llave--;
	if(en_fun==0){
		if(main_llave==0)
			fprintf(fichOut,"return 0;\n}\n");
		else
			fprintf(fichOut,"}\n");
	}
	else
		fprintf(fichProc,"}\n");
}

void escribir_1var(FILE* fich, char *nombre){
	fprintf(fich,"%s %s;\n",tipo_actual,nombre);
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
	fprintf(fich,"%s:\n\n",d.EtiquetaElse);
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
	fprintf(fich,"%s:\n\n",d.EtiquetaSalida);
	
	borrarDescriptor();
}

void escribirEtiqueta(FILE* fich, char* etiqueta){
	fprintf(fich, "%s :\n;\n", etiqueta);
}

/** Hay que hacer las funciones para escribir el case**/
void escribe_case(char *elem1, char *elem2, char* ident){
	
	DescriptorControl d;
	generaEtiqueta(d.EtiquetaEntrada);
	generaEtiqueta(d.EtiquetaSalida);
	int h=strlen(ident);
	copiaTo(ident,d.NombreVarControl, h);
	anadirDescriptor(d);
	genera_temporal();
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	if(en_fun==0){
		fprintf(fichOut,"%s = %s;\n",d.NombreVarControl,elem1);
		fprintf(fichOut,"%s %s;\n","int",temp);
		fprintf(fichOut,"%s:\n%s= %s<=%s;\n",d.EtiquetaEntrada,temp,d.NombreVarControl,elem2);
		fprintf(fichOut,"if(!%s)goto %s;\n\n",temp,d.EtiquetaSalida);
	}
	else{
		fprintf(fichProc,"%s = %s;\n",d.NombreVarControl,elem1);
		fprintf(fichProc,"%s %s;\n","int",temp);
		fprintf(fichProc,"%s:\n%s= %s<=%s;\n",d.EtiquetaEntrada,temp,d.NombreVarControl,elem2);
		fprintf(fichProc,"if(!%s)goto %s;\n\n",temp,d.EtiquetaSalida);
	}
}

void fin_for(){
	
	DescriptorControl d=get_descriptor();
	if(en_fun==0){
		fprintf(fichOut,"%s = %s + 1;\n", d.NombreVarControl,d.NombreVarControl); 
		fprintf(fichOut,"goto %s;\n",d.EtiquetaEntrada);
		fprintf(fichOut,"%s:\n\n",d.EtiquetaSalida);
	}
	else{
		fprintf(fichProc,"%s = %s + 1;\n", d.NombreVarControl,d.NombreVarControl); 
		fprintf(fichProc,"goto %s;\n",d.EtiquetaEntrada);
		fprintf(fichProc,"%s:\n\n",d.EtiquetaSalida);
	}
	borrarDescriptor();
}
/******************************************************************/

void escribirInicializacion(FILE* fich, int num, char* valor){
	
	int i;
	for(i=0; i<num; i++){
		fprintf(fich, "%s = %s;\n", TS[TOPE-i].nombre, valor);
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
	if((tip==4)||(tip==5)){
		fprintf(fich,"imprime_pila_int(%d, %s);\n",elem);
	}
	if(tip==6){
		fprintf(fich,"imprime_pila_float(%d, %s);\n",elem);
	}
	if(tip==7){
		fprintf(fich,"imprime_pila_char(%d, %s);\n",elem);
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
