//
// Created by lolikion on 01.04.24.
//

#ifndef NFA_STACK_H
#define NFA_STACK_H
#include "logic.h"
typedef Operator el_type;

typedef struct Stack Stack;

typedef struct Stack{
    el_type *data;
    int size;
}Stack;


Stack *Stack_init();

void Stack_push(Stack *stack, el_type element);

el_type Stack_pop(Stack *stack);

void Stack_free(Stack *stack);


#endif //NFA_STACK_H
