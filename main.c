//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "big_int/big_int.h"


int main(){

    big_int *dim= big_int_get("10");

    big_int *tr1= big_int_get("10");
    big_int *tr2= big_int_get("01");
    big_int *tr3= big_int_get("11");
    big_int *tr4= big_int_get("00");

    big_int *sent= big_int_get("001111111101");

    NFA *nfa=NFA_init(dim);




    NFA_add_state(nfa, NFA_state_init(nfa,1));
    NFA_add_state(nfa, NFA_state_init(nfa,1));
    NFA_add_state(nfa, NFA_state_init(nfa,0));
    NFA_add_state(nfa, NFA_state_init(nfa,1));
    NFA_add_state(nfa, NFA_state_init(nfa,1));

   //comment 1
    NFA_add_transition(nfa,0,1, tr2);
    NFA_add_transition(nfa,0,0, tr1);
    NFA_add_transition(nfa,0,2, tr1);
    NFA_add_transition(nfa,1,1, tr3);
    NFA_add_transition(nfa,1,5, tr4);

    NFA_add_transition(nfa,2,4, tr1);

    //new comment in br2
    NFA_to_dot(nfa);

    printf("ACCEPTED=%d",NFA_check(nfa,sent,0));

    NFA_free(nfa);

    big_int_free2(6,&tr1,&tr2,&tr3,&tr4,&dim,&sent);

    return 0;
}