#include <stdio.h>
#include <stdlib.h>
#include <generacion.h>
#include <grafo.h>

GrafoBidireccional* crearGrafoBidireccional(NodoGrafo *raiz){
    
    if(!raiz){
        
        return NULL;
    }
    
    GrafoBidireccional *grafo=NULL;
    
    grafo=(GrafoBidireccional*)malloc(sizeof(GrafoBidireccional));
    
    if(!grafo){
        
        return NULL;
    }
    
    grafo->raiz=raiz;
    
    return grafo;
    
}

NodoGrafo* crearNodoGrafo(char *id, void *datos){
    
    if(!id || !datos){
        
        return NULL;
    }
    
    NodoGrafo *nodo=NULL;
    
    nodo=(NodoGrafo*)malloc(sizeof(NodoGrafo));
    
    if(!nodo){
        
        return NULL;
    
    }
    
    nodo->id=id;
    
    nodo->datos=datos;
    
    nodo->padres=NULL;
    
    nodo->hijos=NULL;
    
    return nodo;
    
}

int insertarNodoGrafoBidireccional(GrafoBidireccional *grafo, NodoGrafo *insertar, char **id_padres, char **id_hijos){
    
    if(!grafo || !insertar || !id_padres || !id_hijos){
        
        return ERROR;
    }
    
    if(!grafo->raiz){
        
        grafo->raiz=insertar;
        
        return OK;
    }
    
    else{
        
        
    }
    
}

NodoGrafo* buscarNodoGrafoBidireccionalProfundidad(GrafoBidireccional *grafo, char * id){
    
}

NodoGrafo* buscarNodoGrafoBidireccionalAnchura(GrafoBidireccional *grafo, char * id){
    
}
