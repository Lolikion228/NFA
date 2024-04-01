//
// Created by lolikion on 01.04.24.
//

#include "Stack.h"
#include "stdlib.h"
#include "nfa.h"
#include "logic.h"


//0 = operator; 1 = automata
Stack *Stack_init(int type){
    Stack *stack = malloc(sizeof(struct Stack));
    stack->data = NULL;
    stack->size = 0;
    stack->type=type;
    return stack;
}


void Stack_free(Stack *stack){
    free(stack->data);
    free(stack);
}


void Stack_push(Stack *stack, void *element){
    int el_sz = stack->type ? sizeof(NFA *) : sizeof(Operator *);
    stack->data = realloc(stack->data,(++stack->size)*el_sz);
    if(stack->type){
        stack->data[stack->size - 1] = (NFA *)element;
    }
    else{
        stack->data[stack->size - 1] = (Operator *)element;
    }
}


void *Stack_pop(Stack *stack){
    if(stack->type){
        NFA *res = stack->data[--stack->size];
        stack->data = realloc(stack->data, (stack->size)*sizeof(NFA *));
        return res;
    }
    else{
        Operator *res = stack->data[--stack->size];
        stack->data = realloc(stack->data, (stack->size)*sizeof(Operator *));
        return res;
    }
}
