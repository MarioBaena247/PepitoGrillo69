

#include "TADcfo.h"

int main (int argc, char** argv){

    TADcfo * nuevo=NULL;
    nuevo= crearTADcfo(10);
    insertarTADcfo(nuevo, "hola");
    insertarTADcfo(nuevo, "adios");
    insertarTADcfo(nuevo, "zzzz");
    imprimirTAD(nuevo);
    ordenaTADcfo(nuevo);
    imprimirTAD(nuevo);
    insertarTADcfo(nuevo, "aaaaaaaaaaaaaa");
    ordenaTADcfo(nuevo);
    imprimirTAD(nuevo);
    printf("\nPosicion de hola-> %d",buscarTADcfo(nuevo, "hola"));
    libera(nuevo);




}
