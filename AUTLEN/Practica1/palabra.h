#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADcfo.h"
#include "alfabeto.h"

typedef struct _Palabra Palabra;

Palabra *nuevaPalabra(Alfabeto *alf);
void liberaPalabra(Palabra* palabra);
int addLetra(Palabra *word, char *letra);
char* extraePalabra(Palabra *word);
void imprimePalabra(FILE*pf, Palabra *palabra);
