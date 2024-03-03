//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "other/big_int.h"



//add multidim; remove curr_state
//read write to file
//cleaning
//add bigger dim


//todo goals issues in git
//multiple states
//clean code and git
//remove cmake_build_debug and .idea from git_remote


int main(){

    int dim = 3;

    int tr1= (int)strtol("000",NULL,2) ;
    int tr2= (int)strtol("011",NULL,2);
    int tr3= (int)strtol("100",NULL,2);
    int tr4= (int)strtol("111",NULL,2);

    int tr5= (int)strtol("1000000011",NULL,2);
    int tr6= (int)strtol("1100000011",NULL,2);

    big_int *sent1= big_int_get("001000");//1
    big_int *sent2= big_int_get("111011");//0
    big_int *sent3= big_int_get("100000");//0
//    big_int *sent4= big_int_get("1100000011110000001110000000111000000011");//0
    int sent_cnt=3;
    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
    sents[0]=sent1;
    sents[1]=sent2;
    sents[2]=sent3;

    NFA *nfa=NFA_init(dim);

//    NFA_add_state(nfa,1);
//    NFA_add_state(nfa,0);
//    NFA_add_transition(nfa,0,1,tr5);
//    NFA_add_transition(nfa,1,1,tr5);
//    NFA_add_transition(nfa,1,2,tr6);
//    NFA_add_transition(nfa,2,2,tr6);


    NFA_add_state(nfa, 0);
    NFA_add_state(nfa, 1);
    NFA_add_state(nfa, 1);
    NFA_add_transition(nfa,0,1, tr1);
    NFA_add_transition(nfa,0,1, tr2);
    NFA_add_transition(nfa,1,3, tr3);
    NFA_add_transition(nfa,1,2, tr4);
    NFA_add_transition(nfa,2,1, tr4);


    NFA_to_file(nfa);
    NFA* a2=NFA_from_file("../automata.txt");
    printf("ACCEPTED=%d\n",NFA_check(nfa,sent1,0));
    printf("ACCEPTED=%d\n",NFA_check(nfa,sent2,0));
    printf("ACCEPTED=%d\n",NFA_check(nfa,sent3,0));

    int*res=NFA_check_many(nfa,sents,sent_cnt,0);

    for(int i=0;i<sent_cnt;i++){
        printf("sent_%d=%d\n",i,res[i]);
    }

    int*res2=NFA_check_many(a2,sents,sent_cnt,0);

    for(int i=0;i<sent_cnt;i++){
        printf("sent_%d=%d\n",i,res2[i]);
    }


//    NFA_print(a2);
    NFA_to_dot(nfa);
    NFA_free(nfa);
    NFA_free(a2);

    free(sents);
    free(res);
    free(res2);
    big_int_free2(3,&sent1,&sent2,&sent3);

    return 0;
}