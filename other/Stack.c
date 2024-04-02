//
// Created by lolikion on 01.04.24.
//

#include "Stack.h"
#include "stdlib.h"
#include "logic.h"


Stack *Stack_init(){
    Stack *stack = malloc(sizeof(struct Stack));
    stack->data = NULL;
    stack->size = 0;
    return stack;
}


void Stack_free(Stack *stack){
    free(stack->data);
    free(stack);
}


void Stack_push(Stack *stack, Operator element){
    stack->data = realloc(stack->data,(++stack->size)*(sizeof(Operator)));
    stack->data[stack->size - 1] = element;

}


Operator Stack_pop(Stack *stack){
    Operator res = stack->data[--stack->size];
    stack->data = realloc(stack->data, (stack->size)*sizeof(Operator));
    return res;

}
