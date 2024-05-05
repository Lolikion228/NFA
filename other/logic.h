//
// Created by lolikion on 01.04.24.
//


#ifndef NFA_LOGIC_H
#define NFA_LOGIC_H
#include "nfa.h"
#include "a_dict.h"
#include "Stack2.h"

typedef struct Operator Operator;

typedef struct Operator{
    int id;
    int priority;
    int pr_x;
    int pr_y;
    int pr_z;
}Operator;

Operator op_init(int id);

NFA *read_cool_lin(char *str);

void parser_helper(Operator op, struct Stack2 *a_stack);

NFA *Parser(char *formula, a_dict *dict);

void RPN_print(char *formula);

#endif //NFA_LOGIC_H
