#include "TADcfo.h"
#include "estados.h"

struct _Estados{
TADcfo *estados;
int *tipos;
int next;
int tamanio;
};


Estados *nuevoEstados(int num_estados){

  if(num_estados<0)
    return NULL;
  Estados *est=(Estados*)malloc(num_estados*sizeof(Estados*));
  if(!est)return NULL;
  est->tipos=(int*)malloc(num_estados*sizeof(int));
  if(!est->tipos) return NULL;
  est->estados=crearTADcfo(num_estados);
  if(!est->estados) return NULL;
  est->next=0;
  est->tamanio=num_estados;
  return est;

}

int getTamanioEstados(Estados *est){
  return est->tamanio;
}

void liberaEstado(Estados *est){
  if(!est)
    return;
  free(est->tipos);
  libera(est->estados);
  free(est);
}




int addEstado(Estados *est ,char *nombre, int tipo){

 if(!est || !nombre || tipo<0 || tipo >3) return ERROR;
 insertarTADcfo(est->estados, nombre);
 est->tipos[est->next]=tipo;
 est->next++;
 return OK;
}

/*Devuelve la posición de un estado en la tabla de tipos*/
int getPosTipo(Estados *est, int tipo){
  int i;
  if(!est || tipo<0 || tipo >3) return -1;

  for(i=0; i<est->tamanio; i++){
    if(est->tipos[i]==tipo) return i;
  }
  return -1;
}


char* getEstadoInicial(Estados *est){

  if(!est) return NULL;
  if(getPosTipo(est, INICIAL)==-1) return NULL;

  return getDato(est->estados, getPosTipo(est, INICIAL));

}

char *getEstado(Estados *est, int pos){
  return getDato(est->estados, pos);
}


int buscarEstados(Estados *est, char *nombre){

  if(!est || !nombre)
    return ERROR;

  return buscarTADcfo(est->estados, nombre);
}

void imprimeEstados(FILE*pf, Estados *estado){
  int i=0;
  if(!estado || !pf)
    return;
fprintf(pf, "\n{");
while(i<estado->next){
  switch (estado->tipos[i]) {
    case INICIAL:
      fprintf(pf, "->%s ", getDato(estado->estados, i));
      break;
    case FINAL:
      fprintf(pf, "%s* ", getDato(estado->estados, i));
      break;
    case NORMAL:
      fprintf(pf, "%s ", getDato(estado->estados, i));
      break;
    case INICIAL_Y_FINAL:
      fprintf(pf, "->%s* ", getDato(estado->estados, i));
      break;
  }
  i++;
}
fprintf(pf, "}\n");

  return;
}
