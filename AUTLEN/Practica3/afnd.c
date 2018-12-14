#include "TADcfo.h"
#include "alfabeto.h"
#include "estados.h"
#include "palabra.h"
#include "afnd.h"
#include "transiciones.h"
#include "transiciones_lambda.h"


struct _TADafnd {
    char *nombres; /*Nombre del arbol finito no determinista*/
    Alfabeto *alfabeto;
    Estados *conjuntoEstados;
    Estados *conjuntoEstadosActual;
    int num_estados;
    int num_simbolos;
    Transiciones_lambda *transicionesL;
    Palabra *entrada;
    Transiciones *transicion;

};

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){


  if(!nombre || num_estados<0 || num_simbolos<0)
    return NULL;

  AFND *a=(AFND*)malloc(sizeof(AFND));
  if(!a)
    return NULL;

  a->nombres=(char*)malloc((strlen(nombre)+1)*sizeof(char));
  if(!a->nombres)
    return NULL;
  strcpy(a->nombres, nombre);

  a->alfabeto=nuevoAlfabeto(num_simbolos);

  if(!a->alfabeto)
    return NULL;

  a->conjuntoEstados=nuevoEstados(num_estados);

  if(!a->conjuntoEstados)
    return NULL;

  a->conjuntoEstadosActual=NULL;

  a->entrada=nuevaPalabra(a->alfabeto);

  if(!a->entrada)
    return NULL;

  a->transicion=crearTransiciones(a->conjuntoEstados, a->alfabeto);

  if(!a->transicion)
    return NULL;

  a->num_estados=num_estados;
  a->num_simbolos=num_simbolos;
  a->transicionesL=crearTransicionesL(a->num_estados, a->conjuntoEstados);

  if(!a->transicionesL)
    return NULL;

  return a;

}
void AFNDElimina(AFND * p_afnd){

  if(!p_afnd)
    return;

  liberaTransicion(p_afnd->transicion);
  liberaTransicionesL(p_afnd->transicionesL);
  liberaAlfabeto(p_afnd->alfabeto);
  liberaPalabra(p_afnd->entrada);
  liberaEstado(p_afnd->conjuntoEstados);
  liberaEstado(p_afnd->conjuntoEstadosActual);


  free(p_afnd->nombres);
  free(p_afnd);
  return;

}

int getNumEstados(AFND* p_afnd){

  return p_afnd->num_estados;
}

int getNumSimbolos(AFND* p_afnd){

  return p_afnd->num_simbolos;
}


void AFNDImprime(FILE * fd, AFND* p_afnd){

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "%s={\n", p_afnd->nombres);
  fprintf(fd, "\tnum_simbolos=%d\n", p_afnd->num_simbolos);
  fprintf(fd, "\n\tA=");
  imprimeAlfabeto(fd, p_afnd->alfabeto);
  fprintf(fd, "\n\tnum_estados=%d\n", p_afnd->num_estados);
  fprintf(fd, "\n\tQ=");
  imprimeEstados(fd, p_afnd->conjuntoEstados);
  imprimeTrasicionesL(fd, p_afnd->transicionesL);
  imprimirTransicion(fd, p_afnd->transicion);
  fprintf(fd, "}\n");
  return;

}
AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){

  if(!p_afnd || !simbolo)
    return NULL;

  addsimbolo(p_afnd->alfabeto, simbolo);

  return p_afnd;
}
AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){

  if(!p_afnd || !nombre || tipo>3 || tipo<0)
    return NULL;

  addEstado(p_afnd->conjuntoEstados, nombre, tipo);

  return p_afnd;

}
AFND * AFNDInsertaTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_simbolo_entrada, char * nombre_estado_f ){

  if(!p_afnd || !nombre_estado_i || !nombre_simbolo_entrada || !nombre_estado_f)
    return NULL;

  insertaTransicion(p_afnd->transicion, nombre_estado_i, nombre_estado_f, nombre_simbolo_entrada);

  return p_afnd;
}
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){

  if(!p_afnd || !letra)
    return NULL;

  addLetra(p_afnd->entrada, letra);

  return p_afnd;

}
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;

  fprintf(fd, "\nACTUALMENTE EN ");
  imprimeEstados(fd, p_afnd->conjuntoEstadosActual);

  return;

}
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){

  if(!fd || !p_afnd)
    return;
  imprimePalabra(fd, p_afnd->entrada);

  return;
}
AFND * AFNDInicializaEstado (AFND * p_afnd){
if(!p_afnd) return NULL;
TADcfo *tad=NULL;
int j, i;
  liberaEstado(p_afnd->conjuntoEstadosActual);
  p_afnd->conjuntoEstadosActual=nuevoEstados(p_afnd->num_estados);

  if(!p_afnd->conjuntoEstadosActual)
    return NULL;
  addEstado(p_afnd->conjuntoEstadosActual, getEstadoInicial(p_afnd->conjuntoEstados), INICIAL);


  for(i=0; i<p_afnd->num_estados; i++){
   tad=getTransicionL(p_afnd->transicionesL, getEstado(p_afnd->conjuntoEstadosActual, i));
   if(tad){
       for(j=0; j<p_afnd->num_estados; j++){
         addEstado(p_afnd->conjuntoEstadosActual, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
       }
     }
     }

  return p_afnd;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){

  int i, aux_contador;

  if(!fd || !p_afnd) return;
    aux_contador=getTamanioPalabra(p_afnd->entrada);

  for(i=0; i<aux_contador; i++){
    AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
    AFNDImprimeCadenaActual(fd, p_afnd);
    AFNDTransita(p_afnd);
  }
  AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
  AFNDImprimeCadenaActual(fd, p_afnd);
}




void AFNDTransita(AFND * p_afnd){

  int i, j, k;
  Estados *aux= NULL;
  TADcfo *tad=NULL;
  char *caracter=extraePalabra(p_afnd->entrada);
  if (!caracter) return;
  aux = nuevoEstados(p_afnd->num_estados);

  /*Aquí añadimos las transiciones "normales"*/
  for(i=0; i<p_afnd->num_estados; i++){
  tad=getEstadoFinal(p_afnd->transicion, getEstado(p_afnd->conjuntoEstadosActual,i), caracter);
    if(tadGetNext(tad)!=0){/*Comprobación para ver si el TAD no está vacío*/
      for(j=0; j<p_afnd->num_estados; j++){
        addEstado(aux, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
      }
    }
  }

  /*Aquí añadimos las transiciones lambda*/
 for(k=0; k<i; k++){
  tad=getTransicionL(p_afnd->transicionesL, getEstado(aux, k));
    if(tadGetNext(tad)!=0){
      for(j=0; j<p_afnd->num_estados; j++){
        addEstado(aux, getDato(tad, j), getTipoEstado(p_afnd->conjuntoEstados, getDato(tad, j)));
      }
    }
  }

liberaEstado(p_afnd->conjuntoEstadosActual);
p_afnd->conjuntoEstadosActual=aux;
return;
}



Palabra *getEntrada(AFND *p_afnd){

  return p_afnd->entrada;
}

AFND * AFNDInsertaLTransicion(AFND * p_afnd, char * nombre_estado_i, char * nombre_estado_f ){

  if(!p_afnd || !nombre_estado_i || !nombre_estado_f){
    return NULL;
  }

  addTransicionL(p_afnd->transicionesL, nombre_estado_i, nombre_estado_f);

  return p_afnd;

}

AFND *AFNDCierraLTransicion(AFND *p_afnd){

  if(!p_afnd) return NULL;

  closeTransicionL(p_afnd->transicionesL);

  return p_afnd;

}


AFND * AFNDInicializaCadenaActual (AFND * p_afnd ){

  while(extraePalabra(p_afnd->entrada)!=NULL);

  return p_afnd;

}

AFND * AFND1ODeSimbolo(char * simbolo){

  AFND * aut= AFNDNuevo(simbolo, 2, 1);
  AFNDInsertaSimbolo(aut, simbolo);
  AFNDInsertaEstado(aut, "q1", INICIAL);
  AFNDInsertaEstado(aut, "q2", FINAL);
  AFNDInsertaTransicion(aut, "q1", simbolo, "q2");
  return aut;

}

AFND * AFND1ODeLambda(){
  AFND * aut= AFNDNuevo("lambda", 2, 0);
  AFNDInsertaEstado(aut, "q1", INICIAL);
  AFNDInsertaEstado(aut, "q2", FINAL);
  AFNDInsertaLTransicion(aut, "q1", "q2");
  return aut;
}

AFND * AFND1ODeVacio(){
  AFND * aut= AFNDNuevo("vacio", 2, 0);
  AFNDInsertaEstado(aut, "q1", INICIAL);
  AFNDInsertaEstado(aut, "q2", FINAL);
  return aut;

}



int AFNDTipoEstadoEn(AFND * p_afnd, int pos){

    return getTipoEstado(p_afnd->conjuntoEstados, getEstado(p_afnd->conjuntoEstados, pos));

}


char * AFNDNombreEstadoEn(AFND * p_afnd, int pos){

return  getEstado(p_afnd->conjuntoEstados, pos);

}

char *AFNDSimboloEn(AFND *p_afnd, int pos){

  return getAlfabeto(p_afnd->alfabeto, pos);
}


AFND * AFNDAAFND1O(AFND * p_afnd){
  int i, j=0, k=0, aux_final_uno[100], aux_inicial_uno=0, contador=0;
  char nombre_estado_i[100], nombre_estado_f[100];
  char concatena[]="_X";;



  AFND *nuevo=AFNDNuevo("Estrella", getNumEstados(p_afnd)+2, getNumSimbolos(p_afnd));

  /*Añadimos los símbolos*/
  for(i=0; i<getNumSimbolos(p_afnd); i++){
  AFNDInsertaSimbolo(nuevo, getAlfabeto(p_afnd->alfabeto, i));
  }

  /*Añadimos los nuevos estados*/
  /*Estados del primer automata*/
  AFNDInsertaEstado(nuevo, "inicial", INICIAL);
    for(i=0; i< (getNumEstados(p_afnd)); i++){

      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd, i));
      strcat(nombre_estado_i, concatena);

      if(AFNDTipoEstadoEn(p_afnd, i)==FINAL){
      aux_final_uno[contador]=i;
      contador++;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd, i)==INICIAL){
      aux_inicial_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd, i));
      }
    }

    AFNDInsertaEstado(nuevo, "final", FINAL);


    /*Añadimos las transiciones*/

    /*Primero las del primer automata*/
    for(i=0; i<(getNumEstados(p_afnd)); i++){
      for(k=0; k<(getNumSimbolos(p_afnd)); k++){
        for(j=0; j<getNumEstados(p_afnd); j++){
            if(indiceTransicion(p_afnd->transicion, i, k, j)==1){
              strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd, i));
              strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd, j));
              strcat(nombre_estado_i, concatena);
              strcat(nombre_estado_f, concatena);
              AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd, k), nombre_estado_f);
            }
          }
      }
  }


/*Añadimos ahora las transiciones lambda*/

/*Primero las del primer automata*/
for(i=0; i<(getNumEstados(p_afnd)); i++){
  for(k=0; k<(getNumEstados(p_afnd)); k++){
      if(indiceLTransicion(p_afnd->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd, k));
        strcat(nombre_estado_i, concatena);
        strcat(nombre_estado_f, concatena);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
}

/*Ahora añadimos las transiciones de los estados finales/inicial antiguos a los
estados inicial/final nuevos.*/


strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd, aux_inicial_uno));
strcat(nombre_estado_i, concatena);
AFNDInsertaLTransicion(nuevo, "inicial",  nombre_estado_i);

for(i=0; i<contador; i++){
  strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd, aux_final_uno[i]));
  strcat(nombre_estado_f, concatena);
  AFNDInsertaLTransicion(nuevo, nombre_estado_f, "final");
}


return nuevo;


}


AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2){

  int i, j=0, k=0, aux_final_uno=0, aux_final_dos=0, aux_inicial_uno=0, aux_inicial_dos=0;
  char nombre_estado_i[100], nombre_estado_f[100];
  char concatena[]="_U1", concatenaa[]="_U2";

    TADcfo *aux=crearTADcfo(getNumSimbolos(p_afnd1O_1)+getNumSimbolos(p_afnd1O_2));
    //Calculamos el número de símbolos que tienen ambos estados
  for(i=0; i<getNumSimbolos(p_afnd1O_1); i++){
    insertarTADcfo_sinrepetidos(aux, AFNDSimboloEn(p_afnd1O_1, i));

  }

  for(i=0; i<getNumSimbolos(p_afnd1O_2); i++){
    insertarTADcfo_sinrepetidos(aux, AFNDSimboloEn(p_afnd1O_2, i));

  }

  AFND *nuevo=AFNDNuevo("Unión", getNumEstados(p_afnd1O_1)+getNumEstados(p_afnd1O_2)+2, tadGetNext(aux));

  /*Añadimos los símbolos*/
  for(i=0; i<tadGetNext(aux); i++){
  AFNDInsertaSimbolo(nuevo, getDato(aux, i));
  }

  /*Añadimos los nuevos estados*/
  /*Estados del primer automata*/
  AFNDInsertaEstado(nuevo, "inicial", INICIAL);
    for(i=0; i< (getNumEstados(p_afnd1O_1)); i++){

      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_1, i));
      strcat(nombre_estado_i, concatena);

      if(AFNDTipoEstadoEn(p_afnd1O_1, i)==FINAL){
      aux_final_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd1O_1, i)==INICIAL){
      aux_inicial_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd1O_1, i));
      }
    }


    /*Estados del segundo automata*/
    i=0;
    for(i=0; i< (getNumEstados(p_afnd1O_2)); i++){
      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_1, i));
      strcat(nombre_estado_i, concatenaa);

      if(AFNDTipoEstadoEn(p_afnd1O_1, i)==INICIAL){
      aux_inicial_dos=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd1O_1, i)==FINAL){
      aux_final_dos=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }
    else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd1O_1, i));
      }
    }
    AFNDInsertaEstado(nuevo, "final", FINAL);


    /*Añadimos las transiciones*/

    /*Primero las del primer automata*/
    for(i=0; i<(getNumEstados(p_afnd1O_1)); i++){
      for(k=0; k<(getNumSimbolos(p_afnd1O_1)); k++){
        for(j=0; j<getNumEstados(p_afnd1O_1); j++){
            if(indiceTransicion(p_afnd1O_1->transicion, i, k, j)==1){
              strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_1, i));
              strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_1, j));
              strcat(nombre_estado_i, concatena);
              strcat(nombre_estado_f, concatena);
              AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd1O_1, k), nombre_estado_f);
            }
          }
      }
  }

  /*Por último las del segundo automata*/
  for(i=0; i<(getNumEstados(p_afnd1O_2)); i++){
    for(k=0; k<(getNumSimbolos(p_afnd1O_2)); k++){
      for(j=0; j<getNumEstados(p_afnd1O_2); j++){
          if(indiceTransicion(p_afnd1O_2->transicion, i, k, j)==1){
            strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_2, i));
            strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_2, j));
            strcat(nombre_estado_i, concatenaa);
            strcat(nombre_estado_f, concatenaa);
            AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd1O_2, k), nombre_estado_f);
          }
        }
    }
  }

  /*Añadimos ahora las transiciones lambda*/

  /*Primero las del primer automata*/
  for(i=0; i<(getNumEstados(p_afnd1O_1)); i++){
  for(k=0; k<(getNumEstados(p_afnd1O_1)); k++){
      if(indiceLTransicion(p_afnd1O_1->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_1, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_1, k));
        strcat(nombre_estado_i, concatena);
        strcat(nombre_estado_f, concatena);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
  }
  /*Y ahora las del segundo automata*/
  for(i=0; i<(getNumEstados(p_afnd1O_2)); i++){
  for(k=0; k<(getNumEstados(p_afnd1O_2)); k++){
      if(indiceLTransicion(p_afnd1O_2->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_2, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_2, k));
        strcat(nombre_estado_i, concatenaa);
        strcat(nombre_estado_f, concatenaa);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
  }
  /*Por último hacemos las transiciones lambda propias de la operacion de unión (1+1)
  nos ayudaremos de los indices aux para crear transiciones del nuevo estado inicial
  a los antiguos y de los antiguos estados finales al nuevo*/


  /*Transicion lambda del nuevo estado inicial al antiguo estado inicial del automata 1*/
  strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_1, aux_inicial_uno));
  strcat(nombre_estado_i, concatena);
  AFNDInsertaLTransicion(nuevo, "inicial", nombre_estado_i);

  /*Transicion lambda del nuevo estado inicial al antiguo estado inicial del automata 2*/
  strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd1O_2, aux_inicial_dos));
  strcat(nombre_estado_i, concatenaa);
  AFNDInsertaLTransicion(nuevo, "inicial", nombre_estado_i);

  /*Transicion lambda del antiguo estado final del automata 1 al nuevo estado final*/

  strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_1, aux_final_uno));
  strcat(nombre_estado_f, concatena);
  AFNDInsertaLTransicion(nuevo, nombre_estado_f, "final");

  strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd1O_2, aux_final_dos));
  strcat(nombre_estado_f, concatenaa);
  AFNDInsertaLTransicion(nuevo, nombre_estado_f, "final");

  libera(aux);
  return nuevo;
}



AFND * AFND1OConcatena(AFND * p_afnd_origen1, AFND * p_afnd_origen2){

  int i, j=0, k=0, aux_final_uno=0, aux_final_dos=0, aux_inicial_uno=0, aux_inicial_dos=0;
  char nombre_estado_i[1000], nombre_estado_f[1000];
  char concatena[]="_K1", concatenaa[]="_K2";

    TADcfo *aux=crearTADcfo(getNumSimbolos(p_afnd_origen1)+getNumSimbolos(p_afnd_origen2));
    //Calculamos el número de símbolos que tienen ambos estados
  for(i=0; i<getNumSimbolos(p_afnd_origen1); i++){
    insertarTADcfo_sinrepetidos(aux, AFNDSimboloEn(p_afnd_origen1, i));

  }

  for(i=0; i<getNumSimbolos(p_afnd_origen2); i++){
    insertarTADcfo_sinrepetidos(aux, AFNDSimboloEn(p_afnd_origen2, i));

  }

  AFND *nuevo=AFNDNuevo("Concatenacion", getNumEstados(p_afnd_origen1)+getNumEstados(p_afnd_origen2)+2, tadGetNext(aux));

  /*Añadimos los símbolos*/
  for(i=0; i<tadGetNext(aux); i++){
  AFNDInsertaSimbolo(nuevo, getDato(aux, i));
  }

  /*Añadimos los nuevos estados*/
  /*Estados del primer automata*/
  AFNDInsertaEstado(nuevo, "inicial", INICIAL);
    for(i=0; i< (getNumEstados(p_afnd_origen1)); i++){

      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen1, i));
      strcat(nombre_estado_i, concatena);

      if(AFNDTipoEstadoEn(p_afnd_origen1, i)==FINAL){
      aux_final_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd_origen1, i)==INICIAL){
      aux_inicial_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd_origen1, i));
      }
    }


    /*Estados del segundo automata*/
    i=0;
    for(i=0; i< (getNumEstados(p_afnd_origen2)); i++){
      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen2, i));
      strcat(nombre_estado_i, concatenaa);

      if(AFNDTipoEstadoEn(p_afnd_origen2, i)==INICIAL){
      aux_inicial_dos=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd_origen2, i)==FINAL){
      aux_final_dos=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }
    else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd_origen2, i));
      }
    }
    AFNDInsertaEstado(nuevo, "final", FINAL);


    /*Añadimos las transiciones*/

    /*Primero las del primer automata*/
    for(i=0; i<(getNumEstados(p_afnd_origen1)); i++){
      for(k=0; k<(getNumSimbolos(p_afnd_origen1)); k++){
        for(j=0; j<getNumEstados(p_afnd_origen1); j++){
            if(indiceTransicion(p_afnd_origen1->transicion, i, k, j)==1){
              strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen1, i));
              strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen1, j));
              strcat(nombre_estado_i, concatena);
              strcat(nombre_estado_f, concatena);
              AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd_origen1, k), nombre_estado_f);
            }
          }
      }
  }

  /*Por último las del segundo automata*/
  for(i=0; i<(getNumEstados(p_afnd_origen2)); i++){
    for(k=0; k<(getNumSimbolos(p_afnd_origen2)); k++){
      for(j=0; j<getNumEstados(p_afnd_origen2); j++){
          if(indiceTransicion(p_afnd_origen2->transicion, i, k, j)==1){
            strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen2, i));
            strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen2, j));
            strcat(nombre_estado_i, concatenaa);
            strcat(nombre_estado_f, concatenaa);
            AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd_origen2, k), nombre_estado_f);
          }
        }
    }
}

/*Añadimos ahora las transiciones lambda*/

/*Primero las del primer automata*/
for(i=0; i<(getNumEstados(p_afnd_origen1)); i++){
  for(k=0; k<(getNumEstados(p_afnd_origen1)); k++){
      if(indiceLTransicion(p_afnd_origen1->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen1, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen1, k));
        strcat(nombre_estado_i, concatena);
        strcat(nombre_estado_f, concatena);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
}
/*Y ahora las del segundo automata*/
for(i=0; i<(getNumEstados(p_afnd_origen2)); i++){
  for(k=0; k<(getNumEstados(p_afnd_origen2)); k++){
      if(indiceLTransicion(p_afnd_origen2->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen2, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen2, k));
        strcat(nombre_estado_i, concatenaa);
        strcat(nombre_estado_f, concatenaa);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
}
/*Por último hacemos las transiciones lambda propias de la operacion de concatenacion (1.1),
es decir, los indices que guardamos antes como aux_incial y aux_final*/
strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen2, aux_inicial_dos));
strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen1, aux_final_uno));
strcat(nombre_estado_i, concatenaa);
strcat(nombre_estado_f, concatena);
AFNDInsertaLTransicion(nuevo, nombre_estado_f, nombre_estado_i);


strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen1, aux_inicial_uno));
strcat(nombre_estado_i, concatena);
AFNDInsertaLTransicion(nuevo, "inicial", nombre_estado_i);

strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen2, aux_final_dos));
strcat(nombre_estado_f, concatenaa);
AFNDInsertaLTransicion(nuevo, nombre_estado_f, "final");

libera(aux);
return nuevo;
}





AFND * AFND1OEstrella(AFND * p_afnd_origen){

  int i, j=0, k=0, aux_final_uno=0, aux_inicial_uno=0;
  char nombre_estado_i[100], nombre_estado_f[100];
  char concatena[]="_X";;



  AFND *nuevo=AFNDNuevo("Estrella", getNumEstados(p_afnd_origen)+2, getNumSimbolos(p_afnd_origen));

  /*Añadimos los símbolos*/
  for(i=0; i<getNumSimbolos(p_afnd_origen); i++){
  AFNDInsertaSimbolo(nuevo, getAlfabeto(p_afnd_origen->alfabeto, i));
  }

  /*Añadimos los nuevos estados*/
  /*Estados del primer automata*/
  AFNDInsertaEstado(nuevo, "inicial", INICIAL);
    for(i=0; i< (getNumEstados(p_afnd_origen)); i++){

      strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen, i));
      strcat(nombre_estado_i, concatena);

      if(AFNDTipoEstadoEn(p_afnd_origen, i)==FINAL){
      aux_final_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else if(AFNDTipoEstadoEn(p_afnd_origen, i)==INICIAL){
      aux_inicial_uno=i;
      AFNDInsertaEstado(nuevo, nombre_estado_i, NORMAL);
    }else{
      AFNDInsertaEstado(nuevo, nombre_estado_i, AFNDTipoEstadoEn(p_afnd_origen, i));
      }
    }

    AFNDInsertaEstado(nuevo, "final", FINAL);


    /*Añadimos las transiciones*/

    /*Primero las del primer automata*/
    for(i=0; i<(getNumEstados(p_afnd_origen)); i++){
      for(k=0; k<(getNumSimbolos(p_afnd_origen)); k++){
        for(j=0; j<getNumEstados(p_afnd_origen); j++){
            if(indiceTransicion(p_afnd_origen->transicion, i, k, j)==1){
              strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen, i));
              strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen, j));
              strcat(nombre_estado_i, concatena);
              strcat(nombre_estado_f, concatena);
              AFNDInsertaTransicion(nuevo, nombre_estado_i, AFNDSimboloEn(p_afnd_origen, k), nombre_estado_f);
            }
          }
      }
  }


/*Añadimos ahora las transiciones lambda*/

/*Primero las del primer automata*/
for(i=0; i<(getNumEstados(p_afnd_origen)); i++){
  for(k=0; k<(getNumEstados(p_afnd_origen)); k++){
      if(indiceLTransicion(p_afnd_origen->transicionesL, i, k)==1){
        strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen, i));
        strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen, k));
        strcat(nombre_estado_i, concatena);
        strcat(nombre_estado_f, concatena);
        AFNDInsertaLTransicion(nuevo, nombre_estado_i, nombre_estado_f);
      }
  }
}

/*Por último hacemos las transiciones lambda propias de la operacion de estrella ()*,
es decir, los indices que guardamos antes como aux_incial y aux_final, los usamos para unir
el nuevo estado inicial, con el antiguo, y el nuevo estado final con el antiguo, y para unir
estos dos estados nuevos entre ellos*/


strcpy(nombre_estado_i, AFNDNombreEstadoEn(p_afnd_origen, aux_inicial_uno));
strcat(nombre_estado_i, concatena);
AFNDInsertaLTransicion(nuevo, "inicial",  nombre_estado_i);

strcpy(nombre_estado_f, AFNDNombreEstadoEn(p_afnd_origen, aux_final_uno));
strcat(nombre_estado_f, concatena);
AFNDInsertaLTransicion(nuevo, nombre_estado_f, "final");

AFNDInsertaLTransicion(nuevo, "inicial", "final");
AFNDInsertaLTransicion(nuevo, "final", "inicial");


return nuevo;



}


void AFNDADot(AFND *p_afnd){

  int i=0, j, k;
  char aux[100];
  strcpy(aux, p_afnd->nombres);
  strcat(aux, ".dot");
  FILE *f=fopen(aux, "w");
  fprintf(f, "digraph grafo{rankdir=LR;\n\t_invisible [style=\"invis\"];\n");
  for(i=0; i<getNumEstados(p_afnd); i++){
    if(AFNDTipoEstadoEn(p_afnd, i)==FINAL){
      fprintf(f,"\t%s [label=\"q%d\"][penwidth=\"2\"];\n", AFNDNombreEstadoEn(p_afnd, i), i);
    }
    else if(AFNDTipoEstadoEn(p_afnd, i)==INICIAL){
      fprintf(f,"\t%s [label=\"q%d\"];\n", AFNDNombreEstadoEn(p_afnd, i), i);
      fprintf(f, "\t_invisible -> %s;\n", AFNDNombreEstadoEn(p_afnd, i));
    }
    else{
      fprintf(f,"\t%s [label=\"q%d\"];\n", AFNDNombreEstadoEn(p_afnd, i), i);
    }
  }

  for(i=0; i<(getNumEstados(p_afnd)); i++){
    for(k=0; k<(getNumSimbolos(p_afnd)); k++){
      for(j=0; j<getNumEstados(p_afnd); j++){
          if(indiceTransicion(p_afnd->transicion, i, k, j)==1){
            fprintf(f, "\t%s -> %s [label=\"%s\"];\n", AFNDNombreEstadoEn(p_afnd, i), AFNDNombreEstadoEn(p_afnd, j), AFNDSimboloEn(p_afnd, k));
          }
        }
    }
}
for(i=0; i<(getNumEstados(p_afnd)); i++){
  for(k=0; k<(getNumEstados(p_afnd)); k++){
      if(indiceLTransicion(p_afnd->transicionesL, i, k)==1){
        fprintf(f, "\t%s -> %s [label=\"&lambda;\"];\n", AFNDNombreEstadoEn(p_afnd, i), AFNDNombreEstadoEn(p_afnd, k));
      }
  }
}

fprintf(f, "}");
fclose(f);

}
