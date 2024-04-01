//
// Created by lolikion on 01.04.24.
//

#ifndef NFA_STACK_H
#define NFA_STACK_H


typedef struct Stack Stack;

typedef struct Stack{
    void **data;
    int type;
    int size;
}Stack;


Stack *Stack_init(int type);

void Stack_push(Stack *stack, void *element);

void *Stack_pop(Stack *stack);

void Stack_free(Stack *stack);


#endif //NFA_STACK_H
