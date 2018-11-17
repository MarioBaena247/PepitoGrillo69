#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estados.h"

typedef struct _Transiciones_lambda Transiciones_lambda;

Transiciones_lambda *crearTransicionesL(int num_estados, Estados *estados);
int addTransicionL(Transiciones_lambda *trans, char *nombre_estado_i, char *nombre_estado_f);
void imprimeTrasicionesL(FILE *fd, Transiciones_lambda *trans);
