#include "transiciones_lambda.h"
#include "estados.h"

struct _Transiciones_lambda{
  Estados *estados;
  int **matriz_l;
  int flag;
  TADcfo **matriz_l_cerrada;
};

Transiciones_lambda *crearTransicionesL(int num_estados, Estados *estados){

  int i, j;
  Transiciones_lambda *trans= (Transiciones_lambda*)malloc(sizeof(Transiciones_lambda));
  trans->matriz_l=(int **)malloc(num_estados*sizeof(int*));
  if(!trans->matriz_l) return NULL;
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

  trans->matriz_l_cerrada=(TADcfo**)malloc((getTamanioEstados(estados)*sizeof(TADcfo*)));

  if(!trans->matriz_l_cerrada) return NULL;

  return trans;
}


void liberaTransicionesL(Transiciones_lambda *trans){

  if(!trans) return;

  int i;

  for(i=0;i<getTamanioEstados(trans->estados);i++){
      free(trans->matriz_l[i]);
  }

  for(i=0; i<(getTamanioEstados(trans->estados)); i++){

    if(trans->matriz_l_cerrada[i]==NULL) break;

    libera(trans->matriz_l_cerrada[i]);
  }

  free(trans->matriz_l_cerrada);
  free(trans->matriz_l);
  free(trans);

  return;
}

int addTransicionL(Transiciones_lambda *trans, char *nombre_estado_i, char *nombre_estado_f){

if(!trans || !nombre_estado_i || !nombre_estado_f){
  return ERROR;
}

if(buscarEstados(trans->estados, nombre_estado_i)<0 || buscarEstados(trans->estados, nombre_estado_f)<0) return -1;

  trans->matriz_l[buscarEstados(trans->estados, nombre_estado_i)][buscarEstados(trans->estados, nombre_estado_f)]=1;
return OK;
}


void imprimeTrasicionesL(FILE * fd, Transiciones_lambda *trans){
  int i, j;

  if(!fd || !trans) return;
fprintf(fd, "\n\tRL++*={\n");
fprintf(fd, "\t");
  for(i=-1; i<getTamanioEstados(trans->estados); i++){
    for(j=-1; j<getTamanioEstados(trans->estados); j++){
      if(i<0){
      	if(j>(getTamanioEstados(trans->estados)-2)) break;
	      else fprintf(fd, "\t[%s]", getEstado(trans->estados, j+1));
	}else{
        	if(j<0) fprintf(fd, "\t[%s]", getEstado(trans->estados, i));
        	else{fprintf(fd, "\t%d", trans->matriz_l[i][j]);}
      	}
    }
    fprintf(fd, "\n");
  }

  return;
}

int closeTransicionL(Transiciones_lambda *trans){

  if(!trans) return ERROR;

  int i, j;


  for(i=0;i<getTamanioEstados(trans->estados);i++){
    for(j=0;j<getTamanioEstados(trans->estados);j++){
      if(trans->matriz_l[i][j]==1){
        if(!trans->matriz_l_cerrada[i]){
          trans->matriz_l_cerrada[i]=crearTADcfo(getTamanioEstados(trans->estados));
          char c=j+'0';
          insertarTADcfo(trans->matriz_l_cerrada[i], &c);
        }
        else{
          char c=j+'0';
          insertarTADcfo(trans->matriz_l_cerrada[i], &c);
        }
        
      }

    }
  }

  return OK;

}

void imprimir(FILE *fd, Transiciones_lambda *trans){

  imprimirTAD(fd, trans->matriz_l_cerrada[0], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[0], 1);
  imprimirTAD(fd, trans->matriz_l_cerrada[0], 2);
  imprimirTAD(fd, trans->matriz_l_cerrada[1], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[1], 1);
  imprimirTAD(fd, trans->matriz_l_cerrada[2], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[3], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[4], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[4], 1);
  imprimirTAD(fd, trans->matriz_l_cerrada[5], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[6], 0);
  imprimirTAD(fd, trans->matriz_l_cerrada[7], 0);
      
}