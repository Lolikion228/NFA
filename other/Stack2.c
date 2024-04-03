//
// Created by lolikion on 01.04.24.
//

#include "Stack2.h"
#include "stdlib.h"
#include "logic.h"


Stack2 *Stack2_init(){
    Stack2 *stack = malloc(sizeof(struct Stack2));
    stack->data = NULL;
    stack->size = 0;
    return stack;
}


void Stack2_free(Stack2 *stack){
    free(stack->data);
    free(stack);
}


void Stack2_push(Stack2 *stack, NFA* element){
    stack->data = realloc(stack->data,(++stack->size)*(sizeof(NFA*)));
    stack->data[stack->size - 1] = element;

}

NFA *Stack2_top(Stack2 *stack){
    return stack->data[stack->size-1];
}

NFA* Stack2_pop(Stack2 *stack){
    NFA* res = stack->data[--stack->size];
    stack->data = realloc(stack->data, (stack->size)*sizeof(NFA*));
    return res;

}
