//
// Created by lolikion on 29.03.24.
//
#include "logic.h"

#ifndef NFA_STACK_H
#define NFA_STACK_H

typedef struct op_stack op_stack;

typedef struct op_stack{
    int size;
    operation *data;
} op_stack;

op_stack *op_stack_init();

void op_stack_free(op_stack *stack);

operation op_stack_pop(op_stack *stack);

void op_stack_push(op_stack *stack, operation element);

int op_stack_is_empty(op_stack *stack);
#endif //NFA_STACK_H
