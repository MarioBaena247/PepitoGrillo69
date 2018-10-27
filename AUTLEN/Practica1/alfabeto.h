#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADcfo.h"





typedef struct _Alfabeto Alfabeto;


Alfabeto *nuevoAlfabeto(int num_simbolos);
void liberaAlfabeto(Alfabeto *alf);
int addsimbolo(Alfabeto *alf ,char *nombre);
int buscarAlfabeto(Alfabeto *alf, char *nombre);
void imprimeAlfabeto(FILE *pf, Alfabeto *alf);
