#include "TADcfo.h"
#include "alfabeto.h"
#include "estados.h"
#include "palabra.h"
#include "afnd.h"
#include "transiciones.h"


struct _TADafnd {
    char *nombres; /*Nombre del arbol finito no determinista*/
    Alfabeto *alfabeto;
    Estados *conjuntoEstados;
    Estados *conjuntoEstadosActual;
    int num_estados;
    int num_simbolos;
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



  a->entrada=nuevaPalabra(a->alfabeto);

  if(!a->entrada)
    return NULL;

  a->transicion=crearTransiciones(a->conjuntoEstados, a->alfabeto);

  if(!a->transicion)
    return NULL;

  a->num_estados=num_estados;
  a->num_simbolos=num_simbolos;

  return a;

}
void AFNDElimina(AFND * p_afnd){

  if(!p_afnd)
    return;

  liberaAlfabeto(p_afnd->alfabeto);
  liberaEstado(p_afnd->conjuntoEstados);
  liberaEstado(p_afnd->conjuntoEstadosActual);
  liberaPalabra(p_afnd->entrada);
  liberaTransicion(p_afnd->transicion);

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
  imprimirTransicion(fd, p_afnd->transicion);
  fprintf(fd, "}");
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


  liberaEstado(p_afnd->conjuntoEstadosActual);
  p_afnd->conjuntoEstadosActual=nuevoEstados(p_afnd->num_estados);

  if(!p_afnd->conjuntoEstadosActual)
    return NULL;
  addEstado(p_afnd->conjuntoEstadosActual, getEstadoInicial(p_afnd->conjuntoEstados), INICIAL);
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

  int i, j;
  Estados *aux= NULL;
  TADcfo *tad=NULL;
  char *caracter=extraePalabra(p_afnd->entrada);
  if (!caracter) return;
  aux = nuevoEstados(p_afnd->num_estados);

  for(i=0; i<p_afnd->num_estados; i++){
  tad=getEstadoFinal(p_afnd->transicion, getEstado(p_afnd->conjuntoEstadosActual,i), caracter);
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
