//
// Created by lolikion on 01.04.24.
//

#ifndef NFA_STACK2_H
#define NFA_STACK2_H
#include "logic.h"


typedef struct Stack2 Stack2;

typedef struct Stack2{
    NFA **data;
    int size;
}Stack2;


Stack2 *Stack2_init();

void Stack2_push(Stack2 *stack, NFA* element);

NFA* Stack2_pop(Stack2 *stack);

void Stack2_free(Stack2 *stack);


#endif //NFA_STACK2_H
