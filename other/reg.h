//
// Created by lolikion on 05.05.24.
//

#ifndef NFA_REG_H
#define NFA_REG_H
#include "nfa.h"

NFA *RegEx_to_NFA(char *formula);

NFA *NFA_CL_star(NFA *a);

NFA *NFA_concat(NFA *a1, NFA *a2);


#endif //NFA_REG_H
