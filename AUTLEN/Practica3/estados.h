#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADcfo.h"

#define INICIAL 0
#define FINAL 1
#define NORMAL 2
#define INICIAL_Y_FINAL 3

typedef struct _Estados Estados;

Estados *nuevoEstados(int num_estados);
void liberaEstado(Estados *est);
int addEstado(Estados *est ,char *nombre, int tipo);
int buscarEstados(Estados *est, char *nombre);
void imprimeEstados(FILE*pf, Estados *estado);
char *getEstado(Estados *est, int pos);
char* getEstadoInicial(Estados *est);
int getPosTipo(Estados *est, int tipo);
int getTamanioEstados(Estados *est);
int getTipoEstado(Estados *est, char *nombre);
void estadoNormal(Estados *est, int pos);
