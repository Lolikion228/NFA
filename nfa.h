//
// Created by lolikion on 16.02.24.
//

#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "other/big_int.h"


typedef struct NFA_state NFA_state;
typedef struct NFA_transition NFA_transition;
typedef struct NFA NFA;


typedef struct NFA_state{
    int index;
    int is_final;
    int is_starting;
    NFA_transition *transitions;
} NFA_state;


typedef struct NFA_transition {
    int state_to_ix;
    int transition_trigger;
    NFA_transition *next;
} NFA_transition;


typedef struct NFA{
    int straight;
    int truth;
    NFA_state **states;
    int states_cnt;
    int dim;
} NFA;



NFA *NFA_init(int dim);

void NFA_add_state(NFA *a,int is_final, int is_starting);

void NFA_add_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_remove_transition(NFA *a,int state_from,int state_to, int trigger);

void NFA_print(const NFA* a);

void NFA_free(NFA *a);

void NFA_to_pic(const NFA *a, int id);

int NFA_check(const NFA *a, const int *sentences);

/*
 * dim
 * states_cnt [state[i] is_final]
 * [state[i] is_starting]
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

NFA *NFA_xy_pow2(int pow);

NFA *NFA_const(int n);

NFA *NFA_swap_digits(const NFA *a, int n1, int n2);

NFA *NFA_leftquo(const NFA *a1,const NFA *a2);

NFA *NFA_mult_scalar(int coeff);

int *NFA_reachable_by(const NFA *a, const int *states_set, int trigger);

NFA *DFA_minimization(const NFA *a);

NFA *NFA_remove_dead_states(const NFA *a);

NFA *NFA_to_DFA(NFA *a);

void NFA_complete(NFA *a);

NFA *kill_zeroes(NFA *a, const NFA *orig);

NFA *NFA_div_n(int n);

NFA *NFA_lin_term(int factor, int bias);

NFA *NFA_subs(NFA *a, NFA *lin);

int NFA_th_check(NFA *a);

NFA *NFA_subs2(NFA *a, NFA *lin1, NFA *lin2);
#endif //NFA_NFA_H
