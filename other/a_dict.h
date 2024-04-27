//
// Created by lolikion on 27.04.24.
//

#ifndef NFA_A_DICT_H
#define NFA_A_DICT_H
#include "nfa.h"
typedef struct a_dict a_dict;

typedef struct a_dict{
    NFA **automata;
    char **keys;
    int len;
}a_dict;

a_dict *dict_init();

NFA  *dict_get_a(a_dict *dict, char *key);

void dict_free(a_dict *dict);

#endif //NFA_A_DICT_H
