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
//rework NFA_check(check_many???) flatten->columns (dim=n -> input={big_int}**n)
//add README
//add examples FROM FILES WITH NFA_TO_FILE and normal verbose to check
//add 3|x 2|x x+y=z x=y automata
//tests???
//AST(abstract syntax tree) [L,R,print], RPN,Shunting yard algorithm
//add int *initial states????
//for intersection (L1/\L2)  L1 -> 0*L1  L2 -> 0*L2  0*closed

//z(op1)x(op2)y
/*
 * z:101001
 * x:111100
 * y:001110
 * */


/*
 * typedef struct NFA_state{
    int index;
    int is_final;
    NFA_transition *transitions;
} NFA_state;

typedef struct NFA_transition {
    NFA_state *state_to;
    int transition_trigger;
    NFA_transition *next;
} NFA_transition;

typedef struct NFA{
    NFA_state *states;
    int states_cnt;
    int dim;
    NFA_state *initial_state;
} NFA;*/

int main(){


//ex4
//    big_int *sent1= big_int_get("0");
//    big_int *sent2= big_int_get("001");
//    big_int *sent3= big_int_get("1110001100");
//    big_int *sent4= big_int_get("0011");
//    int sent_cnt=4;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;
//    sents[3]=sent4;
//
//    NFA* a1=NFA_from_file("../automatons/cnt0_is_even.txt");
//    NFA* a2=NFA_from_file("../automatons/cnt1_is_even.txt");
//    NFA *union_= NFA_union(a1,a2);
//    NFA *intersection= NFA_intersection(a1,a2);
//    NFA *intersection_c= NFA_complement(intersection);
//
//
//    NFA_to_pic(a2);
//    NFA_to_pic(union_);
//    NFA_to_pic(intersection_c);
////    printf("res=%d\n", NFA_check(a2,&sents[0]));
//    for(int i=0;i<sent_cnt;i++){
//        printf("by_[cnt0,cnt1,union,intersection,intersection_c]_sent_%d=[%d,%d,%d,%d,%d]\n",i,
//               NFA_check(a1,&sents[i]),
//               NFA_check(a2,&sents[i]),
//               NFA_check(union_,&sents[i]),
//               NFA_check(intersection,&sents[i]),
//               NFA_check(intersection_c,&sents[i]));
//    }
//
//    NFA_free(a1);
//    NFA_free(a2);
////    NFA_free(union_);
////    NFA_free(intersection);
////    NFA_free(intersection_c);
//    free(sents);
//    big_int_free2(4,&sent1,&sent2,&sent3,&sent4);


//ex 5
//    NFA *a= NFA_init(8);
//    NFA_add_state(a,0);
//    NFA_add_state(a,1);
//    NFA_add_transition(a,0,0,(int) strtol("10111001",NULL,2));
//    NFA_add_transition(a,0,1,(int) strtol("10011101",NULL,2));
//    NFA_add_transition(a,1,1,(int) strtol("10111001",NULL,2));
//
//    NFA *pr_a= NFA_project(a,5);
//    NFA *ex_a= NFA_extend(a,7);
////    NFA_to_pic(pr_a);
//    NFA_free(a);
//    NFA_free(pr_a);
//    NFA_free(ex_a);

//ex6
//    NFA *a= NFA_init(4);
//    big_int *sent1= big_int_get(       "001");
//    big_int *sent2= big_int_get(    "110111");
//    big_int *sent3= big_int_get( "100101011");
//    big_int *sent4= big_int_get( "111111111");
//    int sent_cnt=4;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//    sents[2]=sent3;
//    sents[3]=sent4;
//    NFA_check(a,sents);
//    NFA_free(a);
//    free(sents);
//    big_int_free2(4,&sent1,&sent2,&sent3,&sent4);


//ex7
    NFA* a= NFA_from_file("../automatons/x_eq_y.txt");
    big_int *sent1= big_int_get(    "0111");
    big_int *sent2= big_int_get(  "00101");

    int sent_cnt=2;
    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
    sents[0]=sent1;
    sents[1]=sent2;

    printf("res= %d\n",NFA_check(a,sents));
//    NFA_print(a);
    NFA_to_pic(a);
    NFA_free(a);
    free(sents);
    big_int_free2(2,&sent1,&sent2);

    return 0;
}

/*
 * by_[cnt0,cnt1,union,intersection,intersection_c]_sent_0=[0,1,1,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_1=[1,0,1,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_2=[0,0,0,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_3=[1,1,1,1,0]
 */