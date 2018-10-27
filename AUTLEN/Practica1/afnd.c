#include "TADcfo.h"
#include "alfabeto.h"
#include "estados.h"
#include "palabra.h"
#include "afnd.h"


struct _TADafnd {
    char *nombres; /*Nombre del arbol finito no determinista*/
    Alfabeto *alfabeto;
    Estados *conjuntoEstados;
    Estados *conjuntoEstadosActual;
    int num_estados;
    int num_simbolos;
    Palabra *entrada;

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

  a->conjuntoEstadosActual=nuevoEstados(num_estados);

  if(!a->conjuntoEstadosActual)
    return NULL;

  a->entrada=nuevaPalabra(a->alfabeto);

  if(!a->entrada)
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
  free(p_afnd->nombres);
  free(p_afnd);
  return;

}
void AFNDImprime(FILE * fd, AFND* p_afnd){

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "%s={\n", p_afnd->nombres);
  fprintf(fd, "\tnum_simbolos=%d\n", p_afnd->num_simbolos);
  imprimeAlfabeto(fd, p_afnd->alfabeto);
  fprintf(fd, "\n\tnum_estados=%d\n", p_afnd->num_estados);
  imprimeEstados(fd, p_afnd->conjuntoEstados);
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
/*AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ){

}*/
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){

  if(!p_afnd || !letra)
    return NULL;

  addLetra(p_afnd->entrada, letra);

  return p_afnd;

}
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;

  imprimeEstados(fd, p_afnd->conjuntoEstadosActual);

  return;

}
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;

  imprimePalabra(fd, p_afnd->entrada);

  return;
}
/*AFND * AFNDInicializaEstado (AFND * p_afnd){

}
void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){

}
void AFNDTransita(AFND * p_afnd){

}*/
