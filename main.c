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

    char dim= 4;

    
    int tr1= (int)strtol("1000",NULL,2) ;
    int tr2= (int)strtol("1010",NULL,2);
    int tr3= (int)strtol("0101",NULL,2);
    int tr4= (int)strtol("1111",NULL,2);

    big_int *sent= big_int_get("10001010");

    NFA *nfa=NFA_init(dim);

    NFA_add_state(nfa, 0);
    NFA_add_state(nfa, 1);
    NFA_add_state(nfa, 0);
    NFA_add_state(nfa, 1);

    NFA_add_transition(nfa,0,1, tr2);
    NFA_add_transition(nfa,0,1, tr4);
    NFA_add_transition(nfa,2,3, tr3);
    NFA_add_transition(nfa,1,2, tr3);
    NFA_add_transition(nfa,1,1, tr1);
    NFA_add_transition(nfa,2,4, tr1);


    NFA_to_dot(nfa);

    printf("ACCEPTED=%d",NFA_check(nfa,sent,1));

    NFA_free(nfa);

    big_int_free2(1,&sent);

    return 0;
}