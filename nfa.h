//
// Created by lolikion on 16.02.24.
//


#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "other/big_int.h"
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
    int dim;
    NFA_state *initial_state;
} NFA;

NFA *NFA_init(int dim);

void NFA_add_state(NFA *a,int is_final);

void NFA_state_free(NFA_state *s);

void NFA_add_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_remove_transition(NFA *a,int state_from,int state_to, int trigger);

int NFA_check(const NFA *a,big_int *sentence);

int *NFA_check_many(const NFA *a, big_int **sentences, int len);

void NFA_print(const NFA* a);

void NFA_free(NFA *a);

void NFA_to_pic(const NFA *a);


/* dim
 * states_cnt [state[i] is_final]
 * transitions_cnt
 * trigger_value state_from_ix state_to_ix
 */
void NFA_to_file(const NFA *a);

NFA *NFA_from_file(char* file_pth);

NFA *NFA_intersection(const NFA *a1,const NFA *a2);

NFA *NFA_union(const NFA *a1,const NFA *a2);

NFA* NFA_complement(const NFA *a);

int NFA_is_dfa(const NFA *a);

NFA *NFA_copy(const NFA *a);

NFA *NFA_project(const NFA *a, int num_cord);

NFA *NFA_extend(const NFA *a, int num_cord);

NFA *NFA_zero_star_closure(NFA *a);
#endif //NFA_NFA_H
