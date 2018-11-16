#include "TADcfo.h"
#include "alfabeto.h"
#include "estados.h"
#include "palabra.h"

int main (int argc, char** argv){

    TADcfo * nuevo=NULL;
    Alfabeto *alf=NULL;
    Estados *estado=NULL;

    nuevo= crearTADcfo(10);
    insertarTADcfo(nuevo, "hola");
    insertarTADcfo(nuevo, "adios");
    insertarTADcfo(nuevo, "zzzz");
    imprimirTAD(stdout,nuevo);
    ordenaTADcfo(nuevo);
    imprimirTAD(stdout,nuevo);
    insertarTADcfo(nuevo, "aaaaaaaaaaaaaa");
    ordenaTADcfo(nuevo);
    imprimirTAD(stdout,nuevo);
    printf("Posicion de hola-> %d\n",buscarTADcfo(nuevo, "hola"));
    alf=nuevoAlfabeto(4);
    addsimbolo(alf, "1");
    addsimbolo(alf, "4");
    addsimbolo(alf, "2");
    addsimbolo(alf, "3");
    imprimeAlfabeto(stdout, alf);
    estado=nuevoEstados(4);
    addEstado(estado, "q0", 0);
    addEstado(estado, "q3", 2);
    addEstado(estado, "q2", 1);
    addEstado(estado, "q1", 3);
    imprimeEstados(stdout, estado);
    Palabra *palabra=nuevaPalabra(alf);
    addLetra(palabra, "0");
    addLetra(palabra, "5");
    addLetra(palabra, "1");
    addLetra(palabra, "2");
    imprimePalabra(stdout, palabra);
    char *a=extraePalabra(palabra);
    printf("Letra extraida: %s", a);
    liberaAlfabeto(alf);
    liberaEstado(estado);
    liberaPalabra(palabra);
    libera(nuevo);
    return 0;
}
