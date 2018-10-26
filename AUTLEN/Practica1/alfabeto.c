

struct _Alfabeto{
  TADcfo *simbolos;
};

Alfabeto *nuevoAlfabeto(char *nombre, int num_simbolos){
  Alfabeto alf= (Alfabeto*)malloc(sizeof(Alfabeto));
  if (!alf)return NULL;
  alf->simbolos=crearTADcfo(num_simbolos);
  if(!alf->simbolos) return NULL;


  return alf;
}


void liberaAlfabeto(Alfabeto *alf){
  libera(alf->simbolos);
  free(alf);
}


int addsimbolo(Alfabeto *alf ,char *nombre){

 if(!alf || !nombre) return -1;
 insertarTADcfo(alf->simbolos, nombre);
 return OK;
}
