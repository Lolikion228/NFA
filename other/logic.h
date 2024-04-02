//
// Created by lolikion on 01.04.24.
//


#ifndef NFA_LOGIC_H
#define NFA_LOGIC_H
#include "nfa.h"
typedef struct Operator Operator;

typedef struct Operator{
    int id;
    int priority;
}Operator;

Operator op_init(int id);

//NFA *parser(char *formula);

void RPN_print(char *formula);

#endif //NFA_LOGIC_H
