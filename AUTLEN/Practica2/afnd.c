#include "TADcfo.h"
#include "alfabeto.h"
#include "estados.h"
#include "palabra.h"
#include "afnd.h"
#include "transiciones.h"
#include "transiciones_lambda.h"


struct _TADafnd {
    char *nombres; /*Nombre del arbol finito no determinista*/
    Alfabeto *alfabeto;
    Estados *conjuntoEstados;
    Estados *conjuntoEstadosActual;
    int num_estados;
    int num_simbolos;
    Transiciones_lambda *transicionesL;
    Palabra *entrada;
    Transiciones *transicion;

};


AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){


  if(!nombre || num_estados<0 || num_simbolos<0)
    return NULL;

  AFND *a=(AFND*)malloc(sizeof(AFND));
  if(!a)
    return NULL;

  a->nombres=(char*)malloc((strlen(nombre)+1)*sizeof(char));
  if(!a->nombres)
    return NULL;
  strcpy(a->nombres, nombre);

  a->alfabeto=nuevoAlfabeto(num_simbolos);

  if(!a->alfabeto)
    return NULL;

  a->conjuntoEstados=nuevoEstados(num_estados);

  if(!a->conjuntoEstados)
    return NULL;

  a->conjuntoEstadosActual=NULL;

  a->entrada=nuevaPalabra(a->alfabeto);

  if(!a->entrada)
    return NULL;

  a->transicion=crearTransiciones(a->conjuntoEstados, a->alfabeto);

  if(!a->transicion)
    return NULL;

  a->num_estados=num_estados;
  a->num_simbolos=num_simbolos;
  a->transicionesL=crearTransicionesL(a->num_estados, a->conjuntoEstados);

  if(!a->transicionesL)
    return NULL;

  return a;

}
void AFNDElimina(AFND * p_afnd){

  if(!p_afnd)
    return;

  liberaTransicionesL(p_afnd->transicionesL);
  liberaTransicion(p_afnd->transicion);
  liberaAlfabeto(p_afnd->alfabeto);
  liberaPalabra(p_afnd->entrada);
  liberaEstado(p_afnd->conjuntoEstados);
  liberaEstado(p_afnd->conjuntoEstadosActual);


  free(p_afnd->nombres);
  free(p_afnd);
  return;

}
void AFNDImprime(FILE * fd, AFND* p_afnd){

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "%s={\n", p_afnd->nombres);
  fprintf(fd, "\tnum_simbolos=%d\n", p_afnd->num_simbolos);
  fprintf(fd, "\n\tA=");
  imprimeAlfabeto(fd, p_afnd->alfabeto);
  fprintf(fd, "\n\tnum_estados=%d\n", p_afnd->num_estados);
  fprintf(fd, "\n\tQ=");
  imprimeEstados(fd, p_afnd->conjuntoEstados);
  imprimeTrasicionesL(fd, p_afnd->transicionesL);
  imprimirTransicion(fd, p_afnd->transicion);
  fprintf(fd, "}\n");
  return;

}
AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){

  if(!p_afnd || !simbolo)
    return NULL;

  addsimbolo(p_afnd->alfabeto, simbolo);

  return p_afnd;
}
AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){

  if(!p_afnd || !nombre || tipo>3 || tipo<0)
    return NULL;

  addEstado(p_afnd->conjuntoEstados, nombre, tipo);

  return p_afnd;

}
AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ){

  if(!p_afnd || !nombre_estado_i || !nombre_simbolo_entrada || !nombre_estado_f)
    return NULL;

  insertaTransicion(p_afnd->transicion, nombre_estado_i, nombre_estado_f, nombre_simbolo_entrada);

  return p_afnd;
}
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){

  if(!p_afnd || !letra)
    return NULL;

  addLetra(p_afnd->entrada, letra);

  return p_afnd;

}
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "\nACTUALMENTE EN ");
  imprimeEstados(fd, p_afnd->conjuntoEstadosActual);

  return;

}
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;
  imprimePalabra(fd, p_afnd->entrada);

  return;
}
AFND * AFNDInicializaEstado (AFND * p_afnd){
if(!p_afnd) return NULL;
TADcfo *tad=NULL;
int j;
  liberaEstado(p_afnd->conjuntoEstadosActual);
  p_afnd->conjuntoEstadosActual=nuevoEstados(p_afnd->num_estados);

  if(!p_afnd->conjuntoEstadosActual)
    return NULL;
  addEstado(p_afnd->conjuntoEstadosActual, getEstadoInicial(p_afnd->conjuntoEstados), INICIAL);



   tad=getTransicionL(p_afnd->transicionesL, getEstado(p_afnd->conjuntoEstadosActual, 0));
       for(j=0; j<p_afnd->num_estados; j++){
         addEstado(p_afnd->conjuntoEstadosActual, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
       }




  return p_afnd;
}





void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){

  int i, aux_contador;

  if(!fd || !p_afnd) return;
    aux_contador=getTamanioPalabra(p_afnd->entrada);

  for(i=0; i<aux_contador; i++){
    AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
    AFNDImprimeCadenaActual(fd, p_afnd);
    AFNDTransita(p_afnd);
  }
  AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
  AFNDImprimeCadenaActual(fd, p_afnd);
}




void AFNDTransita(AFND * p_afnd){

  int i, j, k;
  Estados *aux= NULL;
  TADcfo *tad=NULL;
  char *caracter=extraePalabra(p_afnd->entrada);
  if (!caracter) return;
  aux = nuevoEstados(p_afnd->num_estados);

  /*Aquí añadimos las transiciones "normales"*/
  for(i=0; i<p_afnd->num_estados; i++){
  tad=getEstadoFinal(p_afnd->transicion, getEstado(p_afnd->conjuntoEstadosActual,i), caracter);
    if(tadGetNext(tad)!=0){/*Comprobación para ver si el TAD no está vacío*/
      for(j=0; j<p_afnd->num_estados; j++){
        addEstado(aux, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
      }
    }
  }
  /*Aquí añadimos las transiciones lambda*/
 for(k=0; k<i; k++){
  tad=getTransicionL(p_afnd->transicionesL, getEstado(aux, k));
    if(tadGetNext(tad)!=0){
      for(j=0; j<p_afnd->num_estados; j++){
        addEstado(aux, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
      }
    }
  }

liberaEstado(p_afnd->conjuntoEstadosActual);
p_afnd->conjuntoEstadosActual=aux;
return;
}



Palabra *getEntrada(AFND *p_afnd){

  return p_afnd->entrada;
}

AFND * AFNDInsertaLTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_estado_f ){

  if(!p_afnd || !nombre_estado_i || !nombre_estado_f){
    return NULL;
  }

  addTransicionL(p_afnd->transicionesL, nombre_estado_i, nombre_estado_f);

  return p_afnd;

}

AFND *AFNDCierraLTransicion(AFND *p_afnd){

  if(!p_afnd) return NULL;

  closeTransicionL(p_afnd->transicionesL);

  return p_afnd;

}


AFND * AFNDInicializaCadenaActual (AFND * p_afnd ){

  while(extraePalabra(p_afnd->entrada)!=NULL);

  return p_afnd;

}
