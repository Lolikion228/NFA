//
// Created by lolikion on 01.04.24.
//

#ifndef NFA_STACK_H
#define NFA_STACK_H
#include "logic.h"

typedef struct Stack Stack;

typedef struct Stack{
    Operator *data;
    int size;
}Stack;


Stack *Stack_init();

void Stack_push(Stack *stack, Operator element);

Operator Stack_pop(Stack *stack);

void Stack_free(Stack *stack);


#endif //NFA_STACK_H
