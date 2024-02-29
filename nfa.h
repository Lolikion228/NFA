//
// Created by lolikion on 16.02.24.
//

//#include "prikol.h"


#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "big_int/big_int.h"
#include "other/linked_list.h"

typedef struct NFA_state NFA_state;
typedef struct NFA_transition NFA_transition;
typedef struct NFA NFA;


typedef struct NFA_state{
    int index;
    int accept_state;
    struct list *transitions;
    int transitions_cnt;
} NFA_state;


typedef struct NFA_transition {
    NFA_state *state_from;
    NFA_state *state_to;
    big_int *transition_trigger;
} NFA_transition;


typedef struct NFA{
    NFA_state **states;
    int states_cnt;
    big_int *dim; //<=255!!!!
    NFA_state *initial_state;
    NFA_state *current_state;
} NFA;

int is_transitions_equal(NFA_transition *tr1,NFA_transition *tr2);

NFA_state *NFA_state_init(NFA *a,int accept_state);

int NFA_check(NFA *a,big_int *sentence,int verbose);

void NFA_remove_transition(NFA *a,int state_from,int state_to, big_int *trigger);

NFA *NFA_init(big_int *dim);

void NFA_print(NFA* a);

void NFA_add_state(NFA *a,NFA_state *state);

void NFA_free(NFA *a);

void NFA_state_free(NFA_state *s);

void NFA_transition_free(NFA_transition *tr);

void NFA_add_transition(NFA *a,int state_from,int state_to, big_int *trigger);

void NFA_to_dot(NFA *a);
#endif //NFA_NFA_H
