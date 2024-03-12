//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "other/big_int.h"


//       done
//add multidim; remove curr_state
//read write to file
//cleaning
//add bigger dim
//remove cmake_build_debug and .idea from git_remote
//multiple states
//fix a->dim/9
//epsilon transition
//NFA union/intersection
//remove fsm.gv
//add epsilon_tr for intersection
//nfa_id_dfa



//          todo
//add README
//add examples FROM FILES WITH NFA_TO_FILE and normal verbose to check
//rework epsilon tr????
//tests???
//NFA_Copy

int main(){



//ex1
//    int dim = 10;
//    int tr5= (int)strtol("1000000011",NULL,2);//dim=10
//    int tr6= (int)strtol("1100000011",NULL,2);
//    int tr7= (int)strtol("1110000011",NULL,2);
//    int tr8= (int)strtol("1111000011",NULL,2);
//    big_int *sent1= big_int_get("11100000111100000011110000001110000000111000000011");
//    big_int *sent2= big_int_get("11110000111000000011100000001111000000111000000011");
//    int sent_cnt=2;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//
//    NFA *nfa=NFA_init(dim);
//    NFA_add_state(nfa,1);
//    NFA_add_state(nfa,0);
//    NFA_add_state(nfa,1);
//    NFA_add_transition(nfa,0,1,tr5);
//    NFA_add_transition(nfa,1,1,tr5);
//    NFA_add_transition(nfa,1,0,tr8);
//    NFA_add_transition(nfa,1,2,tr6);
//    NFA_add_transition(nfa,2,2,tr6);
//    NFA_add_transition(nfa,2,3,tr7);
//    NFA_add_transition(nfa,2,1,tr5);
//    NFA_to_file(nfa);
//    NFA* a2=NFA_from_file("../automata.txt");
//
//    int*res=NFA_check_many(nfa,sents,sent_cnt,0);
//    int*res2=NFA_check_many(a2,sents,sent_cnt,0);
//    for(int i=0;i<sent_cnt;i++){
//        printf("by_[orig,copy]_sent_%d=[%d,%d]\n",i,res[i],res2[i]);
//    }
//
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent1));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent2));
//
//
////    NFA_print(nfa);
////    NFA_to_dot(nfa);
//
//    NFA_free(nfa);
//    NFA_free(a2);
//    free(sents);
//    free(res);
//    free(res2);
//    big_int_free2(2,&sent1,&sent2);

//ex2
//    int dim = 2;
//    int tr0= (int)strtol("00",NULL,2) ;
//    int tr1= (int)strtol("11",NULL,2);
//    big_int *sent1= big_int_get("11001100101000");
//    big_int *sent2= big_int_get("1110101011110011001011");
//    big_int *sent3= big_int_get("00110010101011");
//    big_int *sent4= big_int_get("11010101010100");
//    int sent_cnt=4;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;
//    sents[3]=sent4;
//
//    NFA *nfa=NFA_init(dim);
//    NFA_add_state(nfa, 0);
//    NFA_add_state(nfa, 0);
//    NFA_add_state(nfa, 0);
//    NFA_add_state(nfa, 0);
//    NFA_add_state(nfa, 1);
//
//    NFA_add_transition(nfa,0,0, tr0);
//    NFA_add_transition(nfa,0,0, tr1);
//    NFA_add_transition(nfa,0,1, tr1);
//    NFA_add_transition(nfa,1,2, tr0);
//    NFA_add_transition(nfa,2,5, tr1);
//    NFA_add_transition(nfa,0,3, tr0);
//    NFA_add_transition(nfa,3,4, tr1);
//    NFA_add_transition(nfa,4,5, tr0);
//
//    NFA_to_file(nfa);
//    NFA* a2=NFA_from_file("../automata.txt");
//
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent1));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent2));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent3));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent4));
//
//    int*res=NFA_check_many(nfa,sents,sent_cnt);
//    int*res2=NFA_check_many(a2,sents,sent_cnt);
////
//    for(int i=0;i<sent_cnt;i++){
//        printf("by_[orig,copy]_sent_%d=[%d,%d]\n",i,res[i],res2[i]);
//    }
//
////    NFA_print(nfa);
//    NFA_to_pic(nfa);
//    NFA_free(nfa);
//    NFA_free(a2);
//    free(sents);
//    free(res);
//    free(res2);
//    big_int_free2(4,&sent1,&sent2,&sent3,&sent4);


//ex3
//    int dim = 2;
//    int tr0= (int)strtol("00",NULL,2) ;
//    int tr1= (int)strtol("11",NULL,2);
//    big_int *sent1= big_int_get("01111010");
//    big_int *sent2= big_int_get("11110001");
//    big_int *sent3= big_int_get("00110000");
//    big_int *sent4= big_int_get("101010101100");
//    int sent_cnt=4;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;
//    sents[3]=sent4;
//
//    NFA *nfa=NFA_init(dim);
//    NFA_add_state(nfa, 1);
//    NFA_add_state(nfa, 0);
//    NFA_add_state(nfa, 1);
//    NFA_add_state(nfa, 0);
//
//
//    NFA_add_transition(nfa,0,1, -1);
//    NFA_add_transition(nfa,0,3, -1);
//
//    NFA_add_transition(nfa,1,2, tr0);
//    NFA_add_transition(nfa,2,1, tr0);
//    NFA_add_transition(nfa,3,4, tr1);
//    NFA_add_transition(nfa,4,3, tr1);
//
//    NFA_add_transition(nfa,1,1, tr1);
//    NFA_add_transition(nfa,2,2, tr1);
//    NFA_add_transition(nfa,3,3, tr0);
//    NFA_add_transition(nfa,4,4, tr0);
//
//
//    NFA_to_file(nfa);
//    NFA* a2=NFA_from_file("../automata.txt");
//
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent1));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent2));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent3));
//    printf("ACCEPTED=%d\n",NFA_check(nfa,sent4));
//
////    int*res=NFA_check_many(nfa,sents,sent_cnt);
////    int*res2=NFA_check_many(a2,sents,sent_cnt);
//////
////    for(int i=0;i<sent_cnt;i++){
////        printf("by_[orig,copy]_sent_%d=[%d,%d]\n",i,res[i],res2[i]);
////    }
//
////    NFA_print(nfa);
//    NFA_to_pic(nfa);
//    NFA_free(nfa);
//    NFA_free(a2);
//    free(sents);
////    free(res);
////    free(res2);
//    big_int_free2(4,&sent1,&sent2,&sent3,&sent4);


//ex4
//    big_int *sent1= big_int_get("001101");
//    big_int *sent2= big_int_get("010010");
//    big_int *sent3= big_int_get("00110000");
//    big_int *sent4= big_int_get("00001111");
//    int sent_cnt=4;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;
//    sents[3]=sent4;
//
//    NFA* a1=NFA_from_file("../automatons/cnt0_is_even.txt");
//    NFA* a2=NFA_from_file("../automatons/cnt1_is_even.txt");
////    NFA_print(a1);
////    NFA_print(a2);
//    NFA *union_= NFA_union(a1,a2);
//    NFA *intersection= NFA_intersection(a1,a2);
//    NFA *intersection_c= NFA_complement(intersection);
//
//
//
////    NFA_to_pic(union_);
//    NFA_to_pic(intersection_c);
//
//    for(int i=0;i<sent_cnt;i++){
//        printf("by_[cnt0,cnt1,union,intersection,mintersection_c]_sent_%d=[%d,%d,%d,%d,%d]\n",i,
//               NFA_check(a1,sents[i]),
//               NFA_check(a2,sents[i]),
//               NFA_check(union_,sents[i]),
//               NFA_check(intersection,sents[i]),
//               NFA_check(intersection_c,sents[i]));
//    }
//
//    NFA_free(a1);
//    NFA_free(a2);
//    NFA_free(union_);
//    NFA_free(intersection);
//    NFA_free(intersection_c);
//    free(sents);
//    big_int_free2(4,&sent1,&sent2,&sent3,&sent4);


//ex 5
//    NFA *a= NFA_init(8);
//    NFA_add_state(a,0);
//    NFA_add_state(a,1);
//    NFA_add_transition(a,0,0,(int) strtol("10111001",NULL,2));
//    NFA_add_transition(a,0,1,(int) strtol("10011101",NULL,2));
//    NFA_add_transition(a,1,1,(int) strtol("10111001",NULL,2));
//    NFA *pr_a= NFA_project(a,5);
//    NFA *pr_a2= NFA_project(pr_a,0);
//    NFA_to_pic(pr_a2);
//    NFA_free(a);
//    NFA_free(pr_a);
    return 0;
}