//
// Created by lolikion on 16.02.24.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"
#include "other/big_int.h"
#include "string.h"

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
//bug with bit_ix
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
//    NFA* a= NFA_from_file("../automatons/x_eq_y.txt");
//    big_int *sent1= big_int_get(    "0000010111");
//    big_int *sent2= big_int_get(  "0010111");
//
//    int sent_cnt=2;
//    big_int**sents=(big_int **)calloc(sent_cnt,sizeof(big_int*));
//    sents[0]=sent1;
//    sents[1]=sent2;
//
//    printf("res= %d\n",NFA_check(a,sents));
//    NFA_to_pic(a);
//    NFA_free(a);
//    free(sents);
//    big_int_free2(2,&sent1,&sent2);




//ex7
    NFA* a2 = NFA_from_file("../automatons/lsd/2|x.txt");

    NFA* a3 = NFA_from_file("../automatons/lsd/3|x.txt");
    NFA* a5 = NFA_from_file("../automatons/msd/5|x.txt");
    NFA *a512 = NFA_is_mult_of_pow2(9);
     NFA *a_in = NFA_intersection(a512,a3);
    NFA *a_un = NFA_union(a2,a3);

//    big_int *sent1= big_int_get(    "1111111111111111111111111111111111111111100");
    big_int**sents=(big_int **)calloc(1,sizeof(big_int*));
//    sents[0]=sent1;
//    printf("%d\n",NFA_check(a5,sents));
    NFA_to_pic(a_in);
    for(int i=0; i<20000; i++){

        big_int * num= big_int_from_int(i);
        sents[0]=num;
//        int res2 = NFA_check(a2, sents);
//        int res3 = NFA_check(a3, sents);
//        int res5 = NFA_check(a5, sents);
//        int res1024 = NFA_check(a1024, sents);
        int res_in = NFA_check(a_in, sents);
//        int res_un = NFA_check(a_un, sents);

        if((i%1536==0) != res_in){
            printf("err\n");
            exit(1);
        }


        big_int_free(&num);

    }



    NFA_free(a_in);
    NFA_free(a_un);
    NFA_free(a2);
    NFA_free(a5);
    NFA_free(a512);
    NFA_free(a3);
    free(sents);
//    big_int_free2(1,&sent1);

//ex8
//    NFA *a = NFA_init(4,1);
//    big_int **sents= malloc(4*sizeof(big_int *));
//    big_int *word1 = big_int_get("111001");
//    big_int *word2 = big_int_get("001111");
//    big_int *word3 = big_int_get("000001");
//    big_int *word4 = big_int_get("100000");
//    sents[0]=word1;
//    sents[1]=word2;
//    sents[2]=word3;
//    sents[3]=word4;
//    NFA_check(a,sents);
//    NFA_free(a);
    return 0;

}

/*
 * by_[cnt0,cnt1,union,intersection,intersection_c]_sent_0=[0,1,1,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_1=[1,0,1,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_2=[0,0,0,0,1]
by_[cnt0,cnt1,union,intersection,intersection_c]_sent_3=[1,1,1,1,0]
 */