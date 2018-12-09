#ifndef _TADCFO_H
#define _TADCFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************** CONSTANTES ****************/
#define OK 1
#define ERROR 0

typedef struct _TADcfo TADcfo;

TADcfo *crearTADcfo(int tamanio);
int insertarTADcfo(TADcfo *tad, char *nombre);
int insertarTADcfo_sinrepetidos(TADcfo *tad, char *nombre);
int buscarTADcfo(TADcfo *tad, char *nombre);
void libera(TADcfo *tad);
int ordenaTADcfo(TADcfo *tad);
char* getDato(TADcfo *tad, int pos);
void imprimirTAD(FILE *pf,TADcfo *tad, int pos);
int tadGetNext(TADcfo *tad);
int getTamanioTad(TADcfo *tad);

#endif /*_TADCFO_H*/
