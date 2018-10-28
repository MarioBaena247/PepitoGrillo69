#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estados.h"
#include "alfabeto.h"


typedef struct _Transiciones Transiciones;


Transiciones *crearTransiciones(Estados *est, Alfabeto *alf);
void liberaTransicion(Transiciones *trans);
int insertaTransicion(Transiciones *trans, char *ini, char *fin, char *simbolo);
char *getEstadoFinal(Transiciones *trans, char *incial, char *simbolo);
void imprimirTransicion(FILE*pf,Transiciones *trans);
