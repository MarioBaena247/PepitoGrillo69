#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _TADafnd AFND;

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);
void AFNDElimina(AFND * p_afnd);
void AFNDImprime(FILE * fd, AFND* p_afnd);
AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);
AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);
AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f );
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);
AFND * AFNDInicializaEstado (AFND * p_afnd);
void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);
void AFNDTransita(AFND * p_afnd);
Palabra *getEntrada(AFND *p_afnd);
AFND * AFNDInsertaLTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_estado_f );
AFND *AFNDCierraLTransicion(AFND *p_afnd);
AFND * AFNDInicializaCadenaActual (AFND * p_afnd );
int getNumSimbolos(AFND* p_afnd);
int getNumEstados(AFND* p_afnd);
AFND * AFND1ODeSimbolo( char * simbolo);
AFND * AFND1ODeLambda();
AFND * AFND1ODeVacio();
int AFNDTipoEstadoEn(AFND * p_afnd, int pos);
char * AFNDNombreEstadoEn(AFND * p_afnd, int pos);
char *AFNDSimboloEn(AFND *p_afnd, int pos);
AFND * AFNDAAFND1O(AFND * p_afnd);
AFND * AFND1OConcatena(AFND * p_afnd1O_1, AFND * p_afnd1O_2);
AFND * AFND1OUne(AFND * p_afnd_origen1, AFND * p_afnd_origen2);
AFND * AFND1OEstrella(AFND * p_afnd_origen);

