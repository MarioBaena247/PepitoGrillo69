#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "element.h"
#include "types.h"

/*pila de que contiene elementos*/
struct _Stack {
    int top; 
    Element* item[MAXSTACK];
};

Stack * stack_ini(){
    
    Stack * pStack=NULL;
    int i;
    
    pStack = (Stack *) malloc(sizeof(Stack));
    if(!pStack)
        return NULL;
    
    pStack->top = -1;
    
    for(i=0;i<MAXSTACK;i++)  
        pStack->item[i] = NULL;
        
    return pStack;
}

void stack_destroy(Stack * pStack){
    
    int i;
    
    if(!pStack)
        return;
        
    for(i=0;i<=pStack->top;i++){
        element_destroy(pStack->item[i]);
        pStack->item[i] = NULL;
    }
    
    free (pStack);
    
    return;       
}

Stack * stack_push(Stack * pStack, const Element * pElement){
    
    Element * aux=NULL;
    
    if(!pStack || !pElement || stack_isFull(pStack)==TRUE)
        return NULL;
    
    aux = element_copy(pElement);
    
    if(!aux)
        return NULL;
    
    pStack->top ++;
    
    pStack->item[pStack->top]=aux;
    
    return pStack;
}

Element* stack_pop(Stack *pStack){
    
    Element *aux=NULL;
    
    if(!pStack || stack_isEmpty(pStack)==TRUE)
        return NULL;
    
    aux = pStack->item[pStack->top];
    
    pStack->item[pStack->top] = NULL;
    
    pStack->top --;
    
    return aux;
}

Element* stack_top(const Stack *pStack){ 
    
    Element * aux=NULL;
    
    if(!pStack || stack_isEmpty(pStack)==TRUE)
        return NULL;
    
    aux = element_copy(pStack->item[pStack->top]);
    if(!aux)
        return NULL;
        
    return aux;
}

Bool stack_isEmpty(const Stack *pStack){
    
    if(!pStack)
        return TRUE;
        
    if(pStack->top==-1)
        return TRUE;
        
    return FALSE;
    
}

Bool stack_isFull(const Stack * pStack){
    
    if(!pStack)
        return TRUE;
        
    if(pStack->top==MAXSTACK-1)
        return TRUE;
        
    return FALSE;
}

int stack_print(FILE* pf , const Stack* pStack){
    
    int num=0,i=0;
    
    if(!pStack || !pf)
        return -1;
    
    for(i=pStack->top;i>=0;i--){
        num+=element_print(pf,pStack->item[i]);
    }
    
    return num;
}

