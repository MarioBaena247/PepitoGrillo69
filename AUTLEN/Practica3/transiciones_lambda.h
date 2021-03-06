#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estados.h"

typedef struct _Transiciones_lambda Transiciones_lambda;

Transiciones_lambda *crearTransicionesL(int num_estados, Estados *estados);
void liberaTransicionesL(Transiciones_lambda *trans);
int addTransicionL(Transiciones_lambda *trans, char *nombre_estado_i, char *nombre_estado_f);
void imprimeTrasicionesL(FILE *fd, Transiciones_lambda *trans);
int closeTransicionL(Transiciones_lambda *trans);
TADcfo* getTransicionL(Transiciones_lambda *trans, char *estado);
char** getPosTransicionL(Transiciones_lambda* trans, char* estado_ini);
int indiceLTransicion(Transiciones_lambda *trans, int ei, int ef);
