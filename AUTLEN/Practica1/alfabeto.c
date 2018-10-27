#include "TADcfo.h"
#include "alfabeto.h"

struct _Alfabeto{
  TADcfo *simbolos;
  int tamanio;
};

Alfabeto *nuevoAlfabeto(int num_simbolos){
  if(num_simbolos<0)
    return NULL;
  Alfabeto *alf= (Alfabeto*)malloc(sizeof(Alfabeto));
  if (!alf)return NULL;
  alf->simbolos=crearTADcfo(num_simbolos);
  if(!alf->simbolos) return NULL;
  alf->tamanio=num_simbolos;
  return alf;
}


void liberaAlfabeto(Alfabeto *alf){
  if(!alf)
    return;
  libera(alf->simbolos);
  free(alf);
}


int addsimbolo(Alfabeto *alf ,char *nombre){

 if(!alf || !nombre) return ERROR;
 insertarTADcfo(alf->simbolos, nombre);
 return OK;
}

int buscarAlfabeto(Alfabeto *alf, char *nombre){

  if(!alf || !nombre)
    return ERROR;

  return buscarTADcfo(alf->simbolos, nombre);
}

void imprimeAlfabeto(FILE *pf, Alfabeto *alf){

  if(!alf || !pf)
    return;

  fprintf(pf, "\n\tA={ ");
  imprimirTAD(pf, alf->simbolos);
  fprintf(pf, "}\n");
  return;
}
