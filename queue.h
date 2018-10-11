#ifndef QUEUE_H
#define QUEUE_H
#include "types.h"

#define MAXQUEUE 2000

typedef struct _Queue Queue;
/* Tipos de los punteros a función soportados por la cola */
typedef void (*destroy_elementqueue_function_type)(void*);
typedef void (*(*copy_elementqueue_function_type)(const void*));
typedef int (*print_elementqueue_function_type)(FILE *, const void*);
/**------------------------------------------------------------------
Inicializa la cola: reserva memoria para ella e inicializa todos sus elementos. Es importante que no se reserve
memoria para los elementos de la cola.
------------------------------------------------------------------*/
Queue* queue_ini(destroy_elementqueue_function_type f1, copy_elementqueue_function_type f2,print_elementqueue_function_type f3);
/**------------------------------------------------------------------
Libera la cola y todos sus elementos.
------------------------------------------------------------------*/
void queue_destroy(Queue *q);
/**------------------------------------------------------------------
Comprueba si la cola está vacía.
------------------------------------------------------------------*/
Bool queue_isEmpty(const Queue *q);
/**------------------------------------------------------------------
Comprueba si la cola está llena.
------------------------------------------------------------------*/
Bool queue_isFull(Queue* queue);
/**------------------------------------------------------------------
Inserta un elemento en la cola realizando para ello una copia del mismo, reservando memoria nueva para él.
------------------------------------------------------------------*/
Queue* queue_insert(Queue *q, const void* pElem);
/**------------------------------------------------------------------
Extrae un elemento de la cola. Es importante destacar que la cola deja de apuntar a este elemento por lo que
la gestión de su memoria debe ser coherente: devolver el puntero al elemento o devolver una copia liberando
el elemento en la cola.
------------------------------------------------------------------*/
void * queue_extract(Queue *q);
/**------------------------------------------------------------------
Devuelve el número de elementos de la cola.
------------------------------------------------------------------*/
int queue_size(Queue *q);
/**------------------------------------------------------------------
Imprime toda la cola, devolviendo el número de caracteres escritos.
------------------------------------------------------------------*/
int queue_print(FILE *pf,const Queue *q);

destroy_elementqueue_function_type destroy_element_queue(Queue *q);

#endif /* QUEUE_H */