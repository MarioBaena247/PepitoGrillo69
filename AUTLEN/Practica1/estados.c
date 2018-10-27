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

void imprimeEstados(FILE*pf, Estados *estado){
  if(!estado || !pf)
    return;
  fprintf(pf, "\n\tQ={-> ");
  imprimirTAD(pf, estado->estados);
  fprintf(pf, "* }\n");
  return;
}
