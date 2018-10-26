

struct _Estados{
TADcfo *estados;
int *tipos;
int next;
};


Estados *nuevoEstados(int num_estados){

  Estados *est=(Estados*)malloc(sizeof(Estados*));
  if(!est)return NULL;
  est->tipos=(int*)malloc(num_estados*sizeof(int));
  if(!est->tipos) return NULL;
  est->estados=crearTADcfo(num_estados);
  if(!est->estados) return NULL;
  est->next=0;
  return est;

}

void liberaEstado(Estados *est){
  free(est->tipos);
  libera(est->estados);
  free(est);
}


int addEstado(Estados *est ,char *nombre, int tipo){

 if(!est || !nombre || tipo<0 || tipo >3) return -1;
 insertarTADcfo(est->estados, nombre);
 est->tipos[ext->next]=tipo;
 est->next++;
 return OK;
}
