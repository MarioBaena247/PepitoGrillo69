#include "TADcfo.h"
#include "alfabeto.h"
#include "palabra.h"


struct _Palabra{
 Alfabeto *alfabeto;
 TADcfo *cadena;
 int tamanio;
 int next_rm;

};

Palabra *nuevaPalabra(Alfabeto *alf){

  if(!alf)
    return NULL;
  Palabra *word= (Palabra*)malloc(sizeof(Palabra));
  if(!word)
    return NULL;
  word->cadena=crearTADcfo(100);
  if(!word->cadena)
    return NULL;
  word->alfabeto=alf;
  word->next_rm=0;
  word->tamanio=0;
  return word;

}

void liberaPalabra(Palabra* word){
  if(!word)
    return;

  libera(word->cadena);
  free(word);
}

int getTamanioPalabra(Palabra *word){
  if(!word) return -1;
  return word->tamanio;
}

int addLetra(Palabra *word, char *letra){
  if(!word || !letra)
    return ERROR;
  if (buscarAlfabeto(word->alfabeto, letra)>=0){
    insertarTADcfo(word->cadena, letra);
    word->tamanio++;
    return OK;
  }
  return ERROR;
}

char* extraePalabra(Palabra *word){
  if(!word)
    return NULL;
  word->next_rm++;
  word->tamanio--;
  return getDato(word->cadena, word->next_rm-1);

}

void imprimePalabra(FILE *pf, Palabra *palabra){

  if(!palabra || !pf)
    return;

  fprintf(pf, "[ (%d) ", palabra->tamanio);
  imprimirTAD(pf, palabra->cadena, palabra->next_rm);
  fprintf(pf, "]\n");

  return;

}
