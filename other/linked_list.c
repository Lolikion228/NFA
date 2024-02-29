//
// Created by lolikion on 16.02.24.
//
#include "linked_list.h"
#include <stdlib.h>
#include "../nfa.h"

node *node_init(NFA_transition *tr) {
    node *nd = (node *)malloc(sizeof(node));//not sizeof(node*)
    nd->val = tr;
    nd->next = NULL;
    return nd;
}

void node_free(node *nd) {
    if (nd != NULL) {
        node *tmp = nd;
        while (nd->next != NULL) {
            tmp = nd;
            nd = nd->next;
            NFA_transition_free(tmp->val);
            free(tmp);
        }
        NFA_transition_free(nd->val);
        free(nd);
    }
}