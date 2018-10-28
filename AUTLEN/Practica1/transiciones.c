#include "transiciones.h"
#include "estados.h"
#include "alfabeto.h"


struct _Transiciones{
  Estados *estados;
  Alfabeto *alfabeto;
  char **nombre_estado_i;
  char **simbolo_entrada;
  char **nombre_estado_f;
  int next;
};

Transiciones *crearTransiciones(Estados *est, Alfabeto *alf){

if(!est || !alf)
  return NULL;

Transiciones *trans= (Transiciones*)malloc(sizeof(Transiciones));
trans->estados=est;
trans->alfabeto=alf;
trans->next=0;
trans->nombre_estado_f=(char**)malloc(sizeof(char*));
trans->nombre_estado_i=(char**)malloc(sizeof(char*));
trans->simbolo_entrada=(char**)malloc(sizeof(char*));

return trans;
}

void liberaTransicion(Transiciones *trans){

  if(!trans)
    return;

  int i=0;

  for(i=0;i<trans->next;i++){

    free(trans->nombre_estado_i[i]);
    free(trans->nombre_estado_f[i]);
    free(trans->simbolo_entrada[i]);
  }

  free(trans->nombre_estado_i);
  free(trans->nombre_estado_f);
  free(trans->simbolo_entrada);
  free(trans);
  return;
}

int insertaTransicion(Transiciones *trans, char *ini, char *fin, char *simbolo){

if(!ini || !fin || !simbolo || !trans) return ERROR;

if(trans->next>0){
  trans->nombre_estado_f=(char**)realloc(trans->nombre_estado_f, (trans->next+1)*sizeof(char*));
  trans->nombre_estado_i=(char**)realloc(trans->nombre_estado_i, (trans->next+1)*sizeof(char*));
  trans->simbolo_entrada=(char**)realloc(trans->simbolo_entrada, (trans->next+1)*sizeof(char*));
}

if (buscarEstados(trans->estados, fin)>=0){
  trans->nombre_estado_f[trans->next]=(char*)malloc((strlen(fin)+1)*sizeof(char));
  if(!trans->nombre_estado_f[trans->next]) return ERROR;
  strcpy(trans->nombre_estado_f[trans->next], fin);
}


if (buscarEstados(trans->estados, ini)>=0){
  trans->nombre_estado_i[trans->next]=(char*)malloc((strlen(ini)+1)*sizeof(char));
  if(!trans->nombre_estado_i[trans->next]) return ERROR;
  strcpy(trans->nombre_estado_i[trans->next], ini);
}


if (buscarAlfabeto(trans->alfabeto, simbolo)>=0){
  trans->simbolo_entrada[trans->next]=(char*)malloc((strlen(simbolo)+1)*sizeof(char));
  if(!trans->simbolo_entrada[trans->next]) return ERROR;
  strcpy(trans->simbolo_entrada[trans->next], simbolo);
}

trans->next++;
return OK;
}

char *getEstadoFinal(Transiciones *trans, char *inicial, char *simbolo){

if(!trans || !inicial || !simbolo)
  return NULL;

int i, j;

for(i=0; i<trans->next; i++){
  for(j=0; j<trans->next; j++){
    if(strcmp(trans->nombre_estado_i[i], inicial)==0){
      if(strcmp(trans->simbolo_entrada[j], simbolo)==0)
      return trans->nombre_estado_f[trans->next];
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
  for(i=0;i<trans->next;i++){
      fprintf(pf, "\t\tf(%s,%s)={ %s }\n", trans->nombre_estado_i[i], trans->simbolo_entrada[i], trans->nombre_estado_f[i]);
  }
  fprintf(pf, "\t}\n");
  return;
}
