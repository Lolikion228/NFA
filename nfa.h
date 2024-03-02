//
// Created by lolikion on 16.02.24.
//

//tst228
#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "big_int/big_int.h"
#include "other/linked_list.h"

typedef struct NFA_state NFA_state;
typedef struct NFA_transition NFA_transition;
typedef struct NFA NFA;


typedef struct NFA_state{
    int index;
    int is_final;
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
    int dim; // <= 8!!!!
    NFA_state *initial_state;
} NFA;


NFA *NFA_init(int dim);

void NFA_add_state(NFA *a,int is_final);

void NFA_state_free(NFA_state *s);

void NFA_add_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_remove_transition(NFA *a,int state_from,int state_to, int trigger);

int NFA_check(NFA *a,big_int *sentence,int verbose);

int *NFA_check_many(NFA *a, big_int **sentences, int len,int verbose);

void NFA_print(NFA* a);

void NFA_free(NFA *a);

void NFA_to_dot(NFA *a);

/*
dim
states_cnt [state[i] is_final]
transitions_cnt
trigger_value state_from_ix state_to_ix
 */
void NFA_to_file(NFA *a);

NFA *NFA_from_file(char* file_pth);

#endif //NFA_NFA_H
