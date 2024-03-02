//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "big_int/big_int.h"


//add multidim; remove curr_state


//read write to file walnut

//multiple states
//create strict goals
//clean code


int main(){

    char dim = 3;

    
    int tr1= (int)strtol("000",NULL,2) ;
    int tr2= (int)strtol("011",NULL,2);
    int tr3= (int)strtol("100",NULL,2);
    int tr4= (int)strtol("111",NULL,2);

    big_int *sent1= big_int_get("111000");//1
    big_int *sent2= big_int_get("011011");//0
    big_int *sent3= big_int_get("100000");//0
    big_int**sents=(big_int **)calloc(1,sizeof(big_int*));
    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;

    NFA *nfa=NFA_init(dim);

    NFA_add_state(nfa, 0);
    NFA_add_state(nfa, 1);
    NFA_add_state(nfa, 1);

    NFA_add_transition(nfa,0,1, tr1);
    NFA_add_transition(nfa,0,1, tr2);
    NFA_add_transition(nfa,1,3, tr3);
    NFA_add_transition(nfa,1,2, tr4);
    NFA_add_transition(nfa,2,1, tr4);

    NFA_to_dot(nfa);

//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent1,0));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent2,0));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent3,0));

    int*res=NFA_check_many(nfa,sents,1,0);

//    for(int i=0;i<1;i++){
//        printf("sent_%d=%d\n",i,res[i]);
//    }

    NFA_to_file(nfa);
    NFA* a2=NFA_from_file("../automata.txt");
    NFA_print(a2);
    NFA_free(nfa);

    free(sents);
    free(res);
    big_int_free2(3,&sent1,&sent2,&sent3);

    return 0;
}