
#include "transiciones_lambda.h"
#include "estados.h"
#include "alfabeto.h"

struct _Transiciones_lambda{
  Estados *estados;
  int **matriz_l;
  int flag;
};

Transiciones_lambda *crearTransicionesL(int num_estados, Estados *estados){
  int i, j;
  Transiciones_lambda *trans= (Transiciones_lambda*)malloc(sizeof(Transiciones_lambda));
  trans->matriz_l=(int **)malloc(num_estados*sizeof(int*));
  if(!trans->matriz_l)
    return NULL;
  for(i=0; i<num_estados; i++){
    trans->matriz_l[i]=(int *)malloc(num_estados*sizeof(int));
    if(!trans->matriz_l[i]){
      free(trans->matriz_l);
      return NULL;
    }
    for(j=0; j<num_estados; j++){
      trans->matriz_l[i][j]=0;
    }
    trans->matriz_l[i][i]=1;
  }
  trans->estados=estados;
  trans->flag=0;
}

int addTransicionL(Transiciones_lambda *trans, char *nombre_estado_i, char *nombre_estado_f){

if(!trans || !nombre_estado_i || !nombre_estado_f){
  return -1;
}

if(buscarEstados(trans->estado, nombre_estado_i)<0 || buscarEstados(trans->estado, nombre_estado_f)<0) return -1;

  trans->matriz_l[buscarEstados(trans->estado, nombre_estado_i)][buscarEstados(trans->estado, nombre_estado_f)]=1;
return 1;
}


void imprimeTrasicionesL(FILE * fd, Transiciones_lambda *trans){
  int i, j;
printf("\nRL++*={\n");
printf("\t\t");
  for(i=-1; i<getTamanioEstados(trans->Estados); i++){
    for(j=-1; j<getTamanioEstados(trans->Estados); j++){
      if(i<0) printf("[%s]\t", getEstado(trans->estados, j+1));
      else{
        if(j<0) printf("[%s]\t", getEstado(trans->estados, i));
        else{printf("%d\t", trans->matriz_l[i][j]);}
      }
    }
    printf("\n")
  }
}
