//
// Created by lolikion on 16.02.24.
//


#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "big_int/big_int.h"
#include "other/linked_list.h"

typedef struct NFA_state NFA_state;
typedef struct NFA_transition NFA_transition;
typedef struct NFA NFA;

//typedef char dim_type;


typedef struct NFA_state{
    int index;
    int accept_state;
    struct list *transitions;
    int transitions_cnt;
} NFA_state;


typedef struct NFA_transition {
    NFA_state *state_from;
    NFA_state *state_to;
    int transition_trigger;
} NFA_transition;


typedef struct NFA{
    NFA_state **states;
    int states_cnt;
    int dim; //<=255!!!!
    NFA_state *initial_state;
} NFA;

NFA *NFA_init(int dim);

NFA_state *NFA_state_init(NFA *a,int is_final);

void NFA_add_state(NFA *a,int is_final);

void NFA_state_free(NFA_state *s);

void NFA_add_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_remove_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_transition_free(NFA_transition *tr);

int NFA_check(NFA *a,big_int *sentence,int verbose);

int * NFA_check_many(NFA *a, big_int **sentences, int len,int verbose);

void NFA_print(NFA* a);

void NFA_free(NFA *a);

void NFA_to_dot(NFA *a);

void NFA_to_file(NFA *a);

NFA *NFA_from_file(char* file_pth);
#endif //NFA_NFA_H
