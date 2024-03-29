//
// Created by lolikion on 29.03.24.
//

#include "op_stack.h"
#include "stdlib.h"


op_stack *op_stack_init(){
    op_stack *res = malloc(sizeof(struct op_stack));
    res->data = NULL;
    res->size = 0;
    return res;
}

int op_stack_is_empty(op_stack *stack){
    return stack->size == 0;
}

operation op_stack_pop(op_stack *stack){
    operation res = stack->data[--stack->size];
    stack->data = (operation *)realloc(stack->data, stack->size);
    return res;
}

void op_stack_push(op_stack *stack, operation element){
    stack->data = (operation *)realloc(stack->data, ++stack->size);
    stack->data[stack->size-1] = element;
}


void op_stack_free(op_stack *stack){
    free(stack->data);
    free(stack);
}