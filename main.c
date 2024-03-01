//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "big_int/big_int.h"
//big_int to int
//multiple states


int main(){

    char dim= 2;


    int tr1= 2 ; //10
    int tr2= 2; //01
    int tr3= 3; //01
    int tr4= 3;//11

    big_int *sent= big_int_get("11110010");

    NFA *nfa=NFA_init(dim);

    NFA_add_state(nfa, NFA_state_init(nfa,1));
    NFA_add_state(nfa, NFA_state_init(nfa,1));
    NFA_add_state(nfa, NFA_state_init(nfa,0));
    NFA_add_state(nfa, NFA_state_init(nfa,1));

    NFA_add_transition(nfa,0,1, tr2);
    NFA_add_transition(nfa,0,1, tr4);
    NFA_add_transition(nfa,2,3, tr3);
    NFA_add_transition(nfa,1,2, tr3);
    NFA_add_transition(nfa,2,4, tr1);


    NFA_to_dot(nfa);

    printf("ACCEPTED=%d",NFA_check(nfa,sent,1));

    NFA_free(nfa);

    big_int_free2(1,&sent);

    return 0;
}