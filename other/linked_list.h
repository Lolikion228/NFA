//
// Created by lolikion on 16.02.24.
//
//tst228
#ifndef DFA_LINKED_LIST_H
#define DFA_LINKED_LIST_H
#include "../nfa.h"


typedef struct node {
    struct NFA_transition *val;
    struct node *next;
} node;

typedef struct list{
    node* head;
} list;

node *node_init(struct NFA_transition *tr);
void node_free(node *nd);
#endif //DFA_LINKED_LIST_H
