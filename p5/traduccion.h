
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
	
	temporales[indTemp].num = temporal_base;
	libera_temporales();
}

void elimina_marcatemp(){
	indTemp--;
	temporal_base=	temporales[indTemp].num;
}

int get_temp(){
	
	return temporales[indTemp].num;

}



void add_descriptor(DescriptorControl d){
	indice++;
	tablaControl[indice]=d;

}

void delete_descriptor(){
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
	fprintf(fichero2,"#include <stdio.h>\n\n");
	actual = fichOut;
}

void cerrar_fichero(){

	fclose(fichero1);
	fclose(fichero2);
}

void seleccionar_fOut(){
	
	temporal_base=almacena_temp;
	fichero2=actual;
	actual = fichero1;
	
}

void seleccionar_fTemp(){
	
	almacena_temp=temporal_base;

	fichero1=actual;
	actual = fichero2;
}

void escribir_cabecera(){//hay que añadir los includes
	fprintf(fichero1,"#include \"dec_fun\"\n#include \"dec_dat\"\n#include <stdio.h>\n\nint main()");

}

void cerrar_main(){
	fprintf(fichero1,"return 0;\n");
}

void escribir_llaveA(FILE* fich){

	fprintf(fich,"{\n");
	
	main_llave++;
	
}

void escribir_llaveC(){
	main_llave--;
	if(en_fun==0){
		if(main_llave==0)
			fprintf(fichero1,"return 0;\n}\n");
		else
			fprintf(fichero1,"}\n");
	}
	else
		fprintf(fichero2,"}\n");
}

void escribir_1var(FILE* fich, char *nombre){
	fprintf(fich,"%s %s;\n",tipo_actual,nombre);
}


void actualiza_tipo(char* tipo){
	tipo_actual = tipo;
}

void escribe_elemento(char *el){
	fprintf(fichOut,el);

}

void escribe_coma(){
	fprintf(fichOut,", ");
}

void escribir_variables(FILE* fich, char *tipo){
	actualiza_tipo(tipo);
	int i;
	
	for (i=tope; TS[i].tipoentrada==variable && TS[i].tipodato == no_asignado; i--){
		
		escribir_1var(fich, TS[i].lexema);
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
	copiaTo(m, temp, 10);


}
void genera_temporal(){
	char aux[11]= "temp";
	char aux2[3];
	sprintf(aux2,"%d",temporal_base);
	
	strcat(aux,aux2);

	temporal_base++;
	copiaToTemp(aux);
}

void genera_etiqueta(char * m){
	char aux[15]= "etiqueta";
	char aux2[5];
	sprintf(aux2,"%d",etiqueta_base);
	
	strcat(aux,aux2);

	etiqueta_base++;
	copiaTo(aux,m,15);
}


void copiaToTemp1(char *m){
	copiaTo(m, temp1, 10);
}

void copiaToTemp2(char *m){
	copiaTo(m, temp2, 10);
}


escribe_expresionBin(FILE* fich, char *elem1, char*elem2, char*op, char*tip){
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	genera_temporal();

	fprintf(fich,"%s %s;\n",tip,temp);
	fprintf(fich,"%s = %s %s ",temp, temp1, op);	
	fprintf(fich,"%s;\n",temp2);
		
	
}

void escribe_asignacion(FILE* fich, char *elem1, char*elem2){
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	

	fprintf(fich,"%s = ",temp1);

	fprintf(fich,"%s;\n\n",elem2);



}

void escribe_if(FILE* fich, char *elem1){
	DescriptorControl d;
	
	genera_etiqueta(d.EtiquetaSalida);
	genera_etiqueta(d.EtiquetaElse);	
	add_descriptor(d);
	copiaToTemp1(elem1);
	

	fprintf(fich,"if(!%s)goto %s;\n\n",temp1,d.EtiquetaElse);

	
}

void escribe_else(FILE* fich){
	DescriptorControl d=get_descriptor();

	fprintf(fich,"goto %s;\n\n",d.EtiquetaSalida);
	fprintf(fich,"%s:\n\n",d.EtiquetaElse);
}

escribe_expresionUna(FILE* fich, char *elem1,char*op,char *tip){
	copiaToTemp1(elem1);
	genera_temporal();

	fprintf(fich,"%s %s;\n",tip,temp);
	fprintf(fich,"%s = %s %s;\n",temp,  op,temp1);
}

void escribe_while(FILE* fich, char *elem1){
	DescriptorControl d;
	etiqueta_base--;
	genera_etiqueta(d.EtiquetaEntrada);
	genera_etiqueta(d.EtiquetaSalida);
	add_descriptor(d);
	copiaToTemp1(elem1);
	fprintf(fich,"\nif(!%s)goto %s;\n\n",temp1,d.EtiquetaSalida);
}

void fin_while(FILE* fich){

	DescriptorControl d=get_descriptor();

	fprintf(fich,"goto %s;\n",d.EtiquetaEntrada);
	fprintf(fich,"%s:\n\n",d.EtiquetaSalida);
	
	delete_descriptor();
}

/** Hay que hacer las funciones para escribir el case**/
void escribe_case(char *elem1, char *elem2, char* ident){
	
	DescriptorControl d;
	genera_etiqueta(d.EtiquetaEntrada);
	genera_etiqueta(d.EtiquetaSalida);
	int h=strlen(ident);
	copiaTo(ident,d.NombreVarControl, h);
	add_descriptor(d);
	genera_temporal();
	copiaToTemp1(elem1);
	copiaToTemp2(elem2);
	if(en_fun==0){
		fprintf(fichero1,"%s = %s;\n",d.NombreVarControl,elem1);
		fprintf(fichero1,"%s %s;\n","int",temp);
		fprintf(fichero1,"%s:\n%s= %s<=%s;\n",d.EtiquetaEntrada,temp,d.NombreVarControl,elem2);
		fprintf(fichero1,"if(!%s)goto %s;\n\n",temp,d.EtiquetaSalida);
	}
	else{
		fprintf(fichero2,"%s = %s;\n",d.NombreVarControl,elem1);
		fprintf(fichero2,"%s %s;\n","int",temp);
		fprintf(fichero2,"%s:\n%s= %s<=%s;\n",d.EtiquetaEntrada,temp,d.NombreVarControl,elem2);
		fprintf(fichero2,"if(!%s)goto %s;\n\n",temp,d.EtiquetaSalida);
	}
}

void fin_for(){
	
	DescriptorControl d=get_descriptor();
	if(en_fun==0){
		fprintf(fichero1,"%s = %s + 1;\n", d.NombreVarControl,d.NombreVarControl); 
		fprintf(fichero1,"goto %s;\n",d.EtiquetaEntrada);
		fprintf(fichero1,"%s:\n\n",d.EtiquetaSalida);
	}
	else{
		fprintf(fichero2,"%s = %s + 1;\n", d.NombreVarControl,d.NombreVarControl); 
		fprintf(fichero2,"goto %s;\n",d.EtiquetaEntrada);
		fprintf(fichero2,"%s:\n\n",d.EtiquetaSalida);
	}
	delete_descriptor();
}
/******************************************************************/

void escribe_void_vacio(char *iden ){
	fprintf(fichero1,"\nvoid %s()",iden ); 
}

void escribe_argumentos(){

	int args = tope-indiceProc();
	int i=0;
	int primero = 1;
	for(i=indiceProc()+1;i<=tope;i++){
		if(primero==1){
			fprintf(fichero1,"%s",TipoEscrito(TS[i].tipodato)); 
			primero =0;
		}
		else{
			fprintf(fichero1,", %s",TipoEscrito(TS[i].tipodato)); 	
		}
		fprintf(fichero1," %s",TS[i].lexema); 
		if(TS[i].vector==1){//si es vector
			if(TS[i].ndim==1){
				fprintf(fichero1,"[%d]",TS[i].dim1); 
			}
			else{
				fprintf(fichero1,"[%d][%d]",TS[i].dim1,TS[i].dim2); 
			}
		}
	}
	fprintf(fichero1,")"); 

}


void escribe_void_argumentos(char *iden){
	fprintf(fichero1,"\nvoid %s(",iden); 
	escribe_argumentos();
}



void escribe_cout(FILE* fich, dtipo tip, char *elem, int dim, int d1, int d2){
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
		fprintf(fich,"imprime_pila_int(%d, %s);\n",d1,elem);
	}
	if(tip==6){
		fprintf(fich,"imprime_pila_float(%d, %s);\n",d1,elem);
	}
	if(tip==7){
		fprintf(fich,"imprime_pila_char(%d, %s);\n",d1,elem);
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
