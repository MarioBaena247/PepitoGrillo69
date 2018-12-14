#include "transiciones_lambda.h"
#include "estados.h"

struct _Transiciones_lambda{
  Estados *estados;
  int **matriz_l;
  int flag;
  int num_estados;
  TADcfo *matriz_l_cerrada;
  TADcfo **matriz_l_cerrada_hor;
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
  trans->num_estados=num_estados;
  trans->flag=0;

  trans->matriz_l_cerrada=crearTADcfo(getTamanioEstados(estados));
  trans->matriz_l_cerrada_hor=(TADcfo**)malloc(getTamanioEstados(estados)*sizeof(TADcfo*));
  for(i=0; i<num_estados; i++){
    trans->matriz_l_cerrada_hor[i]=crearTADcfo(num_estados);
  }

  if(!trans->matriz_l_cerrada) return NULL;

  return trans;
}


void liberaTransicionesL(Transiciones_lambda *trans){

  if(!trans) return;

  int i;

  for(i=0;i<trans->num_estados;i++){
      free(trans->matriz_l[i]);
  }

  for(i=0; i<trans->num_estados; i++){
      libera(trans->matriz_l_cerrada_hor[i]);
  }
  free(trans->matriz_l_cerrada_hor);
  libera(trans->matriz_l_cerrada);
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
  for(i=-1; i<trans->num_estados; i++){
    for(j=-1; j<trans->num_estados; j++){
      if(i<0){
      	if(j>(trans->num_estados-2)) break;
	      else fprintf(fd, "\t[%s]", getEstado(trans->estados, j+1));
	}else{
        	if(j<0) fprintf(fd, "\t[%s] |", getEstado(trans->estados, i));
        	else{fprintf(fd, "\t%d", trans->matriz_l[i][j]);}
      	}
    }
    fprintf(fd, "\n");
  }
  fprintf(fd, "\n\t}\n");

  return;
}

int closeTransicionL(Transiciones_lambda *trans){

  if(!trans) return ERROR;

  int i, j;


  for(i=0;i<trans->num_estados;i++){
    insertarTADcfo(trans->matriz_l_cerrada, getEstado(trans->estados, i));
    for(j=0;j<trans->num_estados;j++){
      if(trans->matriz_l[i][j]==1){
        insertarTADcfo(trans->matriz_l_cerrada_hor[i], getEstado(trans->estados, j));
      }
    }
  }

  return OK;

}


TADcfo* getTransicionL(Transiciones_lambda *trans, char *estado){

  if(!trans || !estado) return NULL;
  int aux=0;

  aux=buscarTADcfo(trans->matriz_l_cerrada, estado);
  if(aux<0) return NULL;

  return trans->matriz_l_cerrada_hor[aux];

}


char** getPosTransicionL(Transiciones_lambda* trans, char* estado_ini){

  char**aux=(char**)malloc(trans->num_estados*sizeof(char*));
  int i=buscarTADcfo(trans->matriz_l_cerrada, estado_ini);
  int j=0;

  while(getDato(trans->matriz_l_cerrada_hor[i], j)){
    aux[j]=getDato(trans->matriz_l_cerrada_hor[i], j);
    j++;
  }
  return aux;
}

int indiceLTransicion(Transiciones_lambda *trans, int ei, int ef){

  int i=0;
  if(ei==ef) return 0;
  char *aux=getEstado(trans->estados, ef);
  return trans->matriz_l[ei][ef];


}
