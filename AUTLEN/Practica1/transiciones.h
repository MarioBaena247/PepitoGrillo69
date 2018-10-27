#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct _Transiciones Transiciones;


Transiciones *crearTransiciones(Estados *est);
int insertaTransicion(Transiciones *trans, char *ini, char *fin, char *simbolo);
char *getEstadoFinal(Transiciones *trans, char *incial, char *simbolo);
