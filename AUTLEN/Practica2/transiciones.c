#include "transiciones.h"
#include "estados.h"
#include "alfabeto.h"


struct _Transiciones{
  Estados *estados;
  Alfabeto *alfabeto;
  TADcfo *nombre_estado_i;
  TADcfo *simbolo_entrada;
  TADcfo **nombre_estado_f;
  int flag;
};

Transiciones *crearTransiciones(Estados *est, Alfabeto *alf){
int i;
if(!est || !alf)
  return NULL;

Transiciones *trans= (Transiciones*)malloc(sizeof(Transiciones));
trans->estados=est;
trans->alfabeto=alf;
trans->flag=0;
trans->nombre_estado_i=crearTADcfo(getTamanioEstados(est)*getTamanioAlfabeto(alf));
trans->simbolo_entrada=crearTADcfo(getTamanioEstados(est)*getTamanioAlfabeto(alf));
trans->nombre_estado_f=(TADcfo**)malloc((getTamanioAlfabeto(alf)*getTamanioEstados(est))*sizeof(TADcfo*));

for(i=0; i<(getTamanioAlfabeto(alf)*getTamanioEstados(est)); i++){
  trans->nombre_estado_f[i]=crearTADcfo(getTamanioEstados(est)*getTamanioAlfabeto(alf));
}

/*Inicializamos los datos de transiciones*/

return trans;
}

void inicializaTransiciones(Transiciones *trans){
  if(!trans) return ;
  int i, j;
  for(i=0; i<getTamanioEstados(trans->estados); i++){
    for(j=0; j<getTamanioAlfabeto(trans->alfabeto); j++){
      insertarTADcfo(trans->nombre_estado_i, getEstado(trans->estados, i));
      insertarTADcfo(trans->simbolo_entrada, getAlfabeto(trans->alfabeto, j));
    }
  }
trans->flag=1;
}

void liberaTransicion(Transiciones *trans){

  if(!trans)
    return;

  int i=0;
  for(i=0; i<(getTamanioEstados(trans->estados)*getTamanioAlfabeto(trans->alfabeto)); i++){
    libera(trans->nombre_estado_f[i]);
  }
  free(trans->nombre_estado_f);
  libera(trans->nombre_estado_i);
  libera(trans->simbolo_entrada);
  free(trans);

}

int insertaTransicion(Transiciones *trans, char *ini, char *fin, char *simbolo){

if(!ini || !fin || !simbolo || !trans) return ERROR;
int i;

if(trans->flag==0){
  inicializaTransiciones(trans);
}

for(i=0; i<(getTamanioEstados(trans->estados)*getTamanioAlfabeto(trans->alfabeto)); i++){
    if( strcmp(getDato(trans->nombre_estado_i, i), ini)==0){
      if(strcmp(getDato(trans->simbolo_entrada, i), simbolo)==0){
        return insertarTADcfo(trans->nombre_estado_f[i], fin);
      }
    }
}
  return ERROR;
}



TADcfo *getEstadoFinal(Transiciones *trans, char *inicial, char *simbolo){
  if(!inicial || !simbolo || !trans) return NULL;
  int i;

  for(i=0; i<(getTamanioEstados(trans->estados)*getTamanioAlfabeto(trans->alfabeto)); i++){
      if( strcmp(getDato(trans->nombre_estado_i, i), inicial)==0){
        if(strcmp(getDato(trans->simbolo_entrada, i), simbolo)==0){
          return trans->nombre_estado_f[i];
        }
      }
  }

return NULL;
}


void imprimirTransicion(FILE *pf,Transiciones *trans){

  int i=0;

  if(!trans || !pf)
    return;

  fprintf(pf, "\n\tFunción de transición = {\n\n");
  for(i=0;i<(getTamanioEstados(trans->estados)*getTamanioAlfabeto(trans->alfabeto));i++){
      fprintf(pf, "\t\tf(%s,%s)={", getDato(trans->nombre_estado_i, i), getDato(trans->simbolo_entrada, i));
      imprimirTAD(pf, trans->nombre_estado_f[i], 0);
      fprintf(pf, "}\n");
  }
  fprintf(pf, "\t}\n");
  return;
}
