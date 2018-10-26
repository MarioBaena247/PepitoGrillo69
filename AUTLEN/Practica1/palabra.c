


struct _Palabra{
 TADcfo *alfabeto;
 TAcfo *cadena;
 int next_rm;

};

Palabra *nuevaPalabra(Alfabeto *alf){
    Palabra *word= (Palabra*)malloc(sizeof(Palabra*));
    word->cadena=crearTADcfo(100);
    word->alfabeto=alf;
    next_add=0;
    next_rm=0;
    return word;

}

void liberaPalabra(Palabra* word){
  free(word);
}

int addLetra(Palabra *word, char *letra){
  if (buscarTADcfo(word->alfabeto, letra)>1){
    insertarTADcfo(word->cadena, letra);
    return OK;
  }
  return ERROR;
}

char* extraePalabra(Palabra *word){
  word->next_rm--;
  return getDato(word->cadena, next_rm+1);

}
