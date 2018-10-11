#ifndef GRAFO_H
#define GRAFO_H

#define OK 1
#define ERROR 0

#include <stdio.h>
#include <stdlib.h>
#include <generacion.h>

typedef struct NodoGrafo{
    
    char *id;
    void *datos;
    NodoGrafo **padres;
    NodoGrafo **hijos;
    
}NodoGrafo;

typedef struct GrafoBidireccional{
    
    NodoGrafo *raiz;
    
}GrafoBidireccional;

GrafoBidireccional* crearGrafoBidireccional(NodoGrafo *raiz);
NodoGrafo* crearNodoGrafo(char *id, void *datos);
int insertarNodoGrafoBidireccional(GrafoBidireccional *grafo, NodoGrafo *insertar);
NodoGrafo* buscarNodoGrafoBidireccionalProfundidad(GrafoBidireccional *grafo, NodoGrafo *buscar);
NodoGrafo* buscarNodoGrafoBidireccionalAnchura(GrafoBidireccional *grafo, NodoGrafo *buscar);

#endif /*TREE_H*/