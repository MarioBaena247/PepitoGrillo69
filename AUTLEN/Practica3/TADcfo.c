

#include "TADcfo.h"



struct _TADcfo {
    char **nombres; /*Conjunto del tad*/
    int tamanio; /*Tamaño del tad*/
    int ordenado;/*Ordenado=1 Desordenado=0*/
    int next; /*Siguiente posicion a insertar*/
};

int getTamanioTad(TADcfo *tad){
  return tad->tamanio;
}

TADcfo *crearTADcfo(int tamanio){

    if (tamanio<0) return NULL;

    int i;

    TADcfo *tad = (TADcfo*) malloc (sizeof(TADcfo));

    if(!tad) return NULL;

    tad->nombres=(char**)malloc(tamanio*sizeof(char*));
    if(!tad->nombres){
        libera(tad);
        return NULL;
    }
    tad->tamanio=tamanio;
    tad->next=0;
    tad->ordenado=0;
    for(i=0;i<tad->tamanio;i++)
        tad->nombres[i]=NULL;

    return tad;
}

int tadGetNext(TADcfo *tad){
  if(!tad) return 0;
  return tad->next;
}


int insertarTADcfo(TADcfo *tad, char *nombre){

    if(!nombre || !tad) return ERROR;
    tad->nombres[tad->next]=(char*)malloc((strlen(nombre)+1)*sizeof(char));
    if (!tad->nombres[tad->next]) return ERROR;
    strcpy(tad->nombres[tad->next], nombre);
    tad->next++;

    return OK;

}

int insertarTADcfo_sinrepetidos(TADcfo *tad, char *nombre){

  if(!nombre || !tad) return ERROR;
  if(buscarTADcfo(tad, nombre)!=-1 && tad->next>0) return -1;
  tad->nombres[tad->next]=(char*)malloc((strlen(nombre)+1)*sizeof(char));
  if (!tad->nombres[tad->next]) return ERROR;
  strcpy(tad->nombres[tad->next], nombre);
  tad->next++;

  return OK;
}


int buscarTADcfo(TADcfo *tad, char *nombre){

    int i;

    if (!tad || !nombre){
        return -1;
    }

    for(i=0; i<tad->next; i++){

        if(strcmp(tad->nombres[i], nombre)==0){
            return i;
        }
    }
    return -1;

}


void libera(TADcfo *tad){

    if(!tad)
        return;

    int i;

    for(i=0;i<tad->tamanio;i++)
        free(tad->nombres[i]);
    free(tad->nombres);
    free(tad);

    return;
}



int ordenaTADcfo(TADcfo *tad){

    int i=tad->next-1,j=0;
    char *num;

	if (!tad)
		return ERROR;

    for(; i >= 0; i--)
    {

        for(j = 0; j <= i-1; j++)
		{

		    if(strcmp(tad->nombres[j], tad->nombres[j+1])>0)
			{
			  num = tad->nombres[j+1];
				tad->nombres[j+1] = tad->nombres[j];
				tad->nombres[j] = num;

			}
		}

    }

    return OK;
}
char* getDato(TADcfo *tad, int pos){

    if(!tad || pos<0 || pos>=tad->tamanio || pos>=tad->next) return NULL;
    return tad->nombres[pos];

}

void imprimirTAD(FILE*pf, TADcfo *tad, int pos){
    if(!tad)
      return;
    int i;
    for(i=pos; i<tad->next; i++){
      fprintf(pf, "%s ",  tad->nombres[i]);
    }


}
