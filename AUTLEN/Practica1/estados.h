#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADcfo.h"


typedef struct _Estados Estados;

Estados *nuevoEstados(int num_estados);
void liberaEstado(Estados *est);
int addEstado(Estados *est ,char *nombre, int tipo);
int buscarEstados(Estados *est, char *nombre);
void imprimeEstados(FILE*pf, Estados *estado);
