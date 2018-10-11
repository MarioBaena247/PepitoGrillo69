#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "types.h"
#include "function.h"

struct _Queue{
    
    void** head; /*es el front*/
    void** end;  /*es el rear*/
    void* item[MAXQUEUE];
    destroy_elementqueue_function_type destroy_element_function;
    copy_elementqueue_function_type copy_element_function;
    print_elementqueue_function_type print_element_function;
};

Queue* queue_ini(destroy_elementqueue_function_type f1, copy_elementqueue_function_type f2, print_elementqueue_function_type f3)
{
    Queue *q=NULL;
    int i;
    
    q=(Queue*)malloc(sizeof(Queue));
    
    if(!q)
        return NULL;
        
    for(i=0;i<MAXQUEUE;i++)
        q->item[i]=NULL;
        
    q->head = &(q->item[0]);
        
    q->end = &(q->item[0]);
    
    q->destroy_element_function=f1;
    
    q->copy_element_function=f2;
        
    q->print_element_function=f3;
    
    return q;
    
}

void queue_destroy(Queue *q){
    if(q!=NULL){
        while(q->head != q->end){
            q->destroy_element_function(*(q->head));
            if(q->head != &(q->item[MAXQUEUE-1])){
                q->head = q->head+1;
            }
            else{
                q->head = &(q->item[0]);
            }
        }
    }
    free(q);
    return;
}

Bool queue_isEmpty(const Queue *q)
{
    if(!q)
        return TRUE;
        
    if(q->end == q->head){
        return TRUE;
    }
    
    return FALSE;
    
}

Bool queue_isFull(Queue* queue)
{
    void **aux=NULL;
    
    if(!queue)
        return FALSE;
        
    if(queue->end == &(queue->item[MAXQUEUE-1]))
        aux=&(queue->item[0]);
    
    else
        aux=queue->end + 1;
        
    if(aux==queue->head)
        return TRUE;
    
    return FALSE;
    
}

Queue* queue_insert(Queue *q, const void* pElem){
    
    void * aux = NULL;
    
    if(!q || !pElem || queue_isFull(q)==TRUE){
        return NULL;
    }
    
    aux = q->copy_element_function(pElem);
    if(!aux)
        return NULL;
        
    *(q->end) = aux;
    
    if(q->end == &(q->item[MAXQUEUE-1])){
        q->end = &(q->item[0]);
    }
    
    else{
        q->end++;
    }
    
    return q;
    
}

void * queue_extract(Queue *q){
    
    void *aux=NULL;
    
    if(!q)
        return NULL;
        
    aux=*(q->head);
    
    if(q->head==&(q->item[MAXQUEUE-1]))
        q->head=&(q->item[0]);
        
    else
        q->head++;
        
    return aux;
    
}

int queue_size(Queue *q){

    void **aux = NULL;
    int i;
    
    if (!q) 
        return -1;
        
    aux=q->head;
    
    for(i=0;aux!=q->end;i++)
    {
        
        if(aux==&(q->item[MAXQUEUE-1]))
            aux=q->item;
        else
            aux++;
    }
    
    return i;
    
}

int queue_print(FILE *pf,const Queue *q){
    
    int num=0;
    void **aux=NULL;
    
    if(!q || !pf)
        return -1;
        
    aux=q->head;
    
    while(aux != q->end){
        
        num+=q->print_element_function(pf,*aux);
        
        aux++;
        
        fprintf(pf,"\n");
       
    }
    
    return num;
}

destroy_elementqueue_function_type destroy_element_queue(Queue *q){
    if(!q)
        return NULL;
    
    return q->destroy_element_function;
}
