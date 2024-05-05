//
// Created by lolikion on 16.02.24.
//
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "other/good_stuff.h"
#include "other/Stack2.h"


int NFA_transitions_cnt(NFA_state *state){
    NFA_transition *curr = state->transitions;
    int cnt = 0;
    while(curr){
        cnt++;
        curr = curr->next;
    }
    return cnt;
}


NFA *NFA_init(int dim){
    NFA *a = (NFA *)malloc(sizeof(NFA));
    if(!a){
        printf("memory allocation error in NFA_init_1\n");
        exit(1);
    }

    a->states_cnt =     0;
    a->dim        =   dim;
    a->states     =  NULL;

    return a;
}


void NFA_add_state(NFA *a, int is_final, int is_starting){

    NFA_state *state = (NFA_state *)malloc(sizeof(NFA_state));
    if(!state){
        printf("memory allocation error in state_init1\n");
        exit(1);
    }
    state->index       = a->states_cnt;
    state->is_final    =      is_final;
    state->is_starting =   is_starting;
    state->transitions =          NULL;


    a->states = realloc(a->states,((a->states_cnt)+1) * sizeof(NFA_state) );

    if(!a->states){
        printf("memory allocation error in NFA_add_state\n");
        exit(1);
    }
    a->states_cnt++;
    a->states[a->states_cnt - 1] = state;

}


void NFA_state_free(NFA_state *s){
    NFA_transition *tr = s->transitions;
    if (tr != NULL) {
        NFA_transition *tmp = tr;
        while (tr->next != NULL) {
            tmp = tr;
            tr = tr->next;
            free(tmp);
        }
        free(tr);
    }
    free(s);
}


int NFA_is_transitions_equal(NFA_transition *tr1, NFA_transition *tr2){
    if(!tr1 || !tr2){return 0;}
    return (tr1->state_to_ix == tr2->state_to_ix) && (tr1->transition_trigger == tr2->transition_trigger);
}


void NFA_add_transition(NFA *a, int state_from, int state_to, int trigger){
    if(trigger > (1 << a->dim) ){printf("too big num for trigger %d %d\n",a->dim,trigger);exit(1);}
    NFA_transition *transition = (NFA_transition *)malloc(sizeof(NFA_transition));
    if(!transition){
        printf("memory allocation error in NFA_add_transition\n");
        exit(1);
    }
    transition->state_to_ix        = state_to;
    transition->transition_trigger =  trigger;
    transition->next               =     NULL;

    NFA_transition *curr = a->states[state_from]->transitions;

    if(!curr){
        a->states[state_from]->transitions = transition;
    }
    else{
        while(curr->next){
            if(NFA_is_transitions_equal(transition, curr)){
                free(transition);
                return;
            }
            curr = curr->next;
        }
        if(NFA_is_transitions_equal(transition, curr)){
            free(transition);
            return;
        }
        curr->next = transition;
    }
}


void NFA_remove_transition(NFA *a, int state_from, int state_to, int trigger){
    NFA_transition *curr = a->states[state_from]->transitions;
    NFA_transition *tmp = NULL;

    while(curr){
        if(curr->state_to_ix == state_to && curr->transition_trigger == trigger){
            if(tmp){
                tmp->next = curr->next;
                free(curr);
            }
            else{
                a->states[state_from]->transitions = curr->next;
                free(curr);
            }
            return;
        }
        tmp = curr;
        curr = curr->next;
    }
}


int NFA_check(const NFA *a, const int *sentences){
    if(a->dim==0){printf("check got 0-dim\n");exit(1);}
    int *sents2 = malloc(a->dim * sizeof(int));
    int max_len = -1, is_transition = 0, processed_words = 0, curr_wrd = 0, bit_cnt = 0;
    for(int i=0; i<a->dim; ++i){
        sents2[i] = sentences[i];
        if(bit_len(sentences[i])>max_len){
            max_len=bit_len(sentences[i]);
        }
    }
    max_len=31;
    int *curr_states = calloc(a->states_cnt,sizeof(int));
    for(int i=0; i<a->states_cnt; ++i){
        curr_states[i] = a->states[i]->is_starting;
    }
    while(1) {
        curr_wrd = 0;
        bit_cnt = 0;
        for (int j = 0; j < a->dim; ++j) {
            curr_wrd += (sents2[j] & 1) << bit_cnt;
            bit_cnt++;
            sents2[j] = sents2[j]>>1;
        }

        is_transition = 0;
        int curr_states2[a->states_cnt];
        for(int i=0; i<a->states_cnt; ++i){
            curr_states2[i] = 0;
        }

        for(int i=0; i<a->states_cnt; ++i) {
            if(curr_states[i] == 1) {

                NFA_transition *curr_tr = a->states[i]->transitions;

                while (curr_tr != NULL) {
                    if (  curr_tr->transition_trigger == -1 ) {
                        curr_states[curr_tr->state_to_ix] = 1;
                    }
                    curr_tr = curr_tr->next;
                }

                curr_tr = a->states[i]->transitions;
                while (curr_tr != NULL) {
                    if (  curr_wrd == curr_tr->transition_trigger ) {
                        curr_states2[curr_tr->state_to_ix] = 1;
                        is_transition = 1;
                    }
                    curr_tr = curr_tr->next;
                }

            }
        }

        if(is_transition) {
            for (int i=0; i<a->states_cnt; ++i) {
                curr_states[i] = curr_states2[i];
            }
        }
        processed_words++;
        if(processed_words >= max_len){break;}
    }

    free(sents2);

    for(int i=0; i<a->states_cnt; ++i){
        if(curr_states[i] & a->states[i]->is_final){ free(curr_states); return 1;}
    }
    free(curr_states);
    return 0;
}

void NFA_print(const NFA* a){
    printf("automata_states:\n\n");
    printf("--------------------------------------------\n");
    for(int i=0; i<a->states_cnt; ++i){
        printf("state_ix=%d; is_acceptable=%d; is_starting=%d; transitions=%d\n",
                    a->states[i]->index,a->states[i]->is_final, a->states[i]->is_starting,
                    NFA_transitions_cnt(a->states[i]) );
        printf("   ------transitions-----\n");
        NFA_transition *curr = a->states[i]->transitions;
        while(curr){
            printf("to=%d; trigger=",curr->state_to_ix);
            printf("%b\n",curr->transition_trigger);
            curr = curr->next;
        }
        printf("--------------------------------------------\n\n");
    }
}


void NFA_free(NFA *a){
    for(int i=0; i<a->states_cnt; ++i){
        NFA_state_free(a->states[i]);
    }
    free(a->states);
    free(a);
}


void NFA_to_pic(const NFA *a, int id){
    FILE *f = fopen("../fsm.gv", "w");

    char template[] = " digraph finite_state_machine {\n"
                      "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                      "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                      "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                      "\trankdir=LR;\n";

    fprintf(f,template);

    int first=1;
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_final == 1 && a->states[i]->is_starting == 0){
            if(first){fprintf(f,"\tnode [shape = doublecircle ];");first=0;}
             fprintf(f," %d", i);
        }
    }
    if(!first){fprintf(f,";\n");}

    first = 1;
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_final == 0 && a->states[i]->is_starting == 1){
            if(first){fprintf(f,"\tnode [shape=circle, color=blue ];");first=0;}
            fprintf(f," %d", i);
        }
    }
    if(!first){fprintf(f,";\n");}

    first = 1;
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_final == 1 && a->states[i]->is_starting == 1){
            if(first){fprintf(f,"\tnode [shape=doublecircle, color=blue ];");first=0;}
            fprintf(f," %d", i);
        }
    }
    if(!first){fprintf(f,";\n");}


    fprintf(f,"\tnode [shape = circle,color =black];\n");

    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *curr = a->states[i]->transitions;
        while(curr){
            fprintf(f,"\t%d -> %d [label =\" ", a->states[i]->index, curr->state_to_ix);
            if(curr->transition_trigger!=-1){
                fprintf(f,"%b" , curr->transition_trigger);
            }
            else{
                fprintf(f,"Eps");
            }
            fprintf(f," \"]; \n");
            curr = curr->next;
        }

    }

    fprintf(f,"}   ");
    fclose(f);
    switch(id){
        case 1:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton1.png");
            break;
        case 2:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton2.png");
            break;
        case 3:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton3.png");
            break;
        case 4:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton4.png");
            break;
        case 5:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton5.png");
            break;
        default:
            system("dot -Tpng ../fsm.gv -o ../temp/automaton0.png");
            break;
    }

    system("rm ../fsm.gv");
}


void NFA_to_file(const NFA *a){
    FILE *f = fopen("../temp/automata.txt","w");

    fprintf(f,"%d\n", a->dim);
    fprintf(f,"%d ", a->states_cnt);

    for(int j=0; j<a->states_cnt; ++j){
        fprintf(f,"%d ", a->states[j]->is_final);
    }
    fprintf(f,"\n");

    for(int j=0; j<a->states_cnt; ++j){
        fprintf(f,"%d ", a->states[j]->is_starting);
    }
    fprintf(f,"\n");

    int cnt_tr = 0;
    for(int j=0; j<a->states_cnt; ++j){
        cnt_tr += NFA_transitions_cnt(a->states[j]);
    }
    fprintf(f,"%d\n", cnt_tr);

    for(int j=0; j<a->states_cnt; ++j){
        NFA_transition *curr_tr = a->states[j]->transitions;
        while(curr_tr){
            fprintf(f, "%d %d %d\n", curr_tr->transition_trigger, a->states[j]->index, curr_tr->state_to_ix);
            curr_tr = curr_tr->next;
        }
    }
    fclose(f);
}


NFA *NFA_from_file(char* file_pth){
    FILE *f = fopen(file_pth,"r");
    int dim, cnt_st, cnt_tr;


    fscanf(f,"%d\n", &dim);
    fscanf(f,"%d", &cnt_st);

    int final[cnt_st];
    int starting[cnt_st];
    for(int i=0; i<cnt_st; ++i){
        fscanf(f,"%d", &final[i]);
    }

    for(int i=0; i<cnt_st; ++i){
        fscanf(f,"%d", &starting[i]);
    }

    fscanf(f,"%d\n", &cnt_tr);

    NFA *a = NFA_init(dim);

    for(int i=0; i<cnt_st; ++i){
        NFA_add_state(a,final[i],starting[i]);
    }
    for(int i=0; i<cnt_tr; ++i){
        int trigger, from, to;
        fscanf(f,"%d", &trigger);
        fscanf(f,"%d", &from);
        fscanf(f,"%d\n", &to);
        NFA_add_transition(a,from,to, trigger);
    }
    fclose(f);
    return a;
}


NFA *NFA_union(const NFA *a1, const NFA *a2){
    if(a1->dim != a2->dim){
        printf("a1 dim != a2 dim\n");
        NFA_to_pic(a2,2);
        printf("got %d %d\n",a1->dim,a2->dim);
        exit(1);
    }
    if(a1->dim==0){printf("union got 0-dim\n");exit(1);}
    NFA *res = NFA_init(a1->dim);
    NFA_add_state(res,0,1);

    if(a1->dim==0){
        printf("zero dim in union\n");
        return res;
    }

    for(int i=0; i<a1->states_cnt; ++i){
        NFA_add_state(res, a1->states[i]->is_final,0);
    }
    for(int i=0; i<a1->states_cnt; ++i){
        NFA_transition* curr_tr = a1->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               1 + a1->states[i]->index,
                               1 + curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }

    for(int i=0; i<a2->states_cnt; ++i){
        NFA_add_state(res, a2->states[i]->is_final,0);
    }
    for(int i=0; i<a2->states_cnt; ++i){
        NFA_transition* curr_tr = a2->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               1 + a1->states_cnt + a2->states[i]->index,
                               1 + a1->states_cnt + curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }

    //add transitions to all starting states

    for(int i=0; i<a1->states_cnt; ++i){
        if(a1->states[i]->is_starting){
            NFA_add_transition(res,0,1+i,-1);
        }
    }
    for(int i=0; i<a2->states_cnt; ++i){
        if(a2->states[i]->is_starting){
            NFA_add_transition(res,0,1+i+a1->states_cnt,-1);
        }
    }




    return res;
}


NFA *NFA_intersection(const NFA *a1, const NFA *a2) {

//  ix_uni(ix_a1,ix_a2) = a2->states_cnt * ix_a1 + ix_a2
    if(a1->dim != a2->dim){printf("a1 dim != a2 dim\n");printf("got %d and %d\n",a1->dim,a2->dim);exit(1);}
    if(a1->dim==0){printf("int got 0-dim\n");exit(1);}
    NFA *res = NFA_init(a1->dim);

    if(a1->dim==0){
        printf("zero dim in intersection\n");
        return res;
    }

    for(int i=0; i<a1->states_cnt; ++i){
        for(int j=0; j<a2->states_cnt; ++j){
            NFA_add_state(res,
                          a1->states[i]->is_final && a2->states[j]->is_final,
                          a1->states[i]->is_starting && a2->states[j]->is_starting);
        }
    }

    for(int i=0; i<a1->states_cnt; ++i){
        for(int j=0; j<a2->states_cnt; ++j) {
            NFA_transition *curr_tr1 = a1->states[i]->transitions;
            NFA_transition *curr_tr2 = a2->states[j]->transitions;
            while(curr_tr1){
                while(curr_tr2){
                    if(curr_tr1->transition_trigger == curr_tr2->transition_trigger)
                    {
                        NFA_add_transition(res,
                                           a2->states_cnt * a1->states[i]->index + a2->states[j]->index,
                                           a2->states_cnt * curr_tr1->state_to_ix + curr_tr2->state_to_ix,
                                           curr_tr1->transition_trigger);
                    }
                    if(curr_tr1->transition_trigger == -1){
                        NFA_add_transition(res,
                                           a2->states_cnt * a1->states[i]->index + a2->states[j]->index,
                                           a2->states_cnt * curr_tr1->state_to_ix + a2->states[j]->index,
                                           -1);
                    }
                    if(curr_tr2->transition_trigger == -1){
                        NFA_add_transition(res,
                                           a2->states_cnt * a1->states[i]->index + a2->states[j]->index,
                                           a2->states_cnt * a1->states[i]->index + curr_tr2->state_to_ix,
                                           -1);
                    }
                    curr_tr2 = curr_tr2->next;
                }
                curr_tr2 = a2->states[j]->transitions;
                curr_tr1 = curr_tr1->next;
            }
        }
    }



    return res;
}


NFA* NFA_complement(const NFA *a){
    NFA *res;

    if(a->dim==0){
        res = NFA_copy(a);
        res->straight = !res->straight;
        return res;
    }

    if(!NFA_is_dfa(a)){
         res = NFA_to_DFA((NFA *)a);
    }
    else{
        res = NFA_copy(a);
    }

    for(int i=0; i<res->states_cnt; ++i){
        res->states[i]->is_final = !res->states[i]->is_final;
    }
//    res->straight = !res->straight;
    return res;
}


NFA *NFA_swap_digits(const NFA *a, int n1, int n2){
    if(n1 >= a->dim || n2 >= a->dim) {printf("out of dim exception.\n");exit(1);}
    NFA *res = NFA_copy(a);
    for(int i=0; i<res->states_cnt; ++i){
        NFA_transition *curr_tr = res->states[i]->transitions;
        while(curr_tr){
            curr_tr->transition_trigger = swap_bits(curr_tr->transition_trigger, n1, n2);
            curr_tr = curr_tr->next;
        }
    }
    return res;
}


NFA *NFA_extend(const NFA *a, int num_cord){
    if(num_cord > a->dim){printf("num_cord > dim.\n"); exit(1);}
    NFA *res = NFA_init(a->dim + 1);
    for(int i=0; i<a->states_cnt; ++i){
        NFA_add_state(res, a->states[i]->is_final,a->states[i]->is_starting);
    }

    for(int i=0; i<a->states_cnt; ++i){
        int tr_cnt = NFA_transitions_cnt(a->states[i]);
        NFA_transition *curr_tr = a->states[i]->transitions;
        for(int j=0; j<tr_cnt; ++j){
            int old_tr = curr_tr->transition_trigger;
            NFA_add_transition(res,
                               a->states[i]->index,
                               curr_tr->state_to_ix,
                               extend_int(old_tr,num_cord,1));
            NFA_add_transition(res,
                               a->states[i]->index,
                               curr_tr->state_to_ix,
                               extend_int(old_tr,num_cord,0));
            curr_tr = curr_tr->next;
        }
    }
    return res;
}


NFA *NFA_project(const NFA *a, int num_cord){
    if(a->dim==0){printf("project got 0-dim\n");exit(1);}
//    printf("?????????????????projection func\n");
    if(num_cord >= a->dim){printf("out of dim exception\n"); exit(1);}
    NFA *res = NFA_copy(a);
    res->dim = res->dim -1;
    for(int i=0; i<res->states_cnt; ++i){
        NFA_transition *curr_tr=res->states[i]->transitions;
        while(curr_tr){
            curr_tr->transition_trigger = project_int(curr_tr->transition_trigger, num_cord);
            curr_tr=curr_tr->next;
        }
    }
//    NFA_to_pic(res,1);

    NFA *tmp = kill_zeroes(res,a);
//    NFA *tmp1 = kill_zeroes2(res);

    NFA_free(res);
//    NFA_free(tmp);

    if(a->dim==1){
        tmp->straight=1;
    }

    return tmp;
}


NFA *NFA_copy(const NFA *a){
    NFA *res = NFA_init(a->dim);
    for(int i=0; i<a->states_cnt; ++i){
        NFA_add_state(res, a->states[i]->is_final,a->states[i]->is_starting);
    }
    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               a->states[i]->index,
                               curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }
    res->straight = a->straight;
    return res;
}


int NFA_is_dfa(const NFA *a){
    if(a->dim==0){printf("NFA_IS_DFA got 0-dim\n");return 0;}
    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *curr_tr1 = a->states[i]->transitions;
        while(curr_tr1){
            NFA_transition *curr_tr2 = curr_tr1->next;
            while(curr_tr2){
                if( curr_tr1->transition_trigger == curr_tr2->transition_trigger ||
                    curr_tr1->transition_trigger == -1 ||
                    curr_tr2->transition_trigger == -1 ) {return 0;}
                curr_tr2 = curr_tr2->next;
            }
            curr_tr1 = curr_tr1->next;
        }
    }
    return 1;
}


///
/// \param coeff a coefficient for a variable y
/// \return an automaton for (coeff * x = y)
NFA *NFA_mult_scalar(int coeff){

    if(coeff==0){
        NFA *res = NFA_from_file("../automata/lsd/zeros.txt");
        NFA *res2 = NFA_extend(res,0);
        free(res);
        return res2;
    }

    NFA *res0 = NFA_from_file("../automata/lsd/zeros.txt");
    NFA *res = NFA_extend(res0,0);
    NFA_free(res0);
    NFA *sum =  NFA_from_file("../automata/lsd/sum.txt");

    NFA *tmp0, *tmp1,*tmp2,*tmp3,*tmp4;
    NFA *curr1,*curr2,*curr3;
    NFA *res1, *res2;

    for(int cnt = 0; coeff; coeff >>= 1){
        if(coeff & 1){
            curr1 = NFA_xy_pow2(cnt); // 2^cnt * x = y
            curr2 = NFA_extend(curr1,2);
            curr3 = NFA_extend(curr2,3);

            tmp0 = NFA_extend(res,1);
            tmp1 = NFA_extend(tmp0,3);
            tmp2= NFA_extend(sum,0);
            tmp3 = NFA_intersection(curr3,tmp1);

            NFA_free(res);
            res = NFA_intersection(tmp3,tmp2);
            res1 = NFA_project(res,1);
            res2 = NFA_project(res1,1);
            NFA_free(res);
            tmp4 = NFA_to_DFA(res2);
            res = DFA_minimization(tmp4);

            NFA_free(res1);
            NFA_free(res2);
            NFA_free(curr1);
            NFA_free(curr2);
            NFA_free(curr3);
            NFA_free(tmp0);
            NFA_free(tmp1);
            NFA_free(tmp2);
            NFA_free(tmp3);
            NFA_free(tmp4);
        }
        cnt++;
    }

    NFA_free(sum);


//    2^cnt * y = u, (v), (z)
//    k     * y = (u), v,  (z)
//           (y),  u + v = z


    // x + y = z -> (2^cnt * y + k*y = z) <-> Eu,v (2^cnt * y = u /\ k*y = v /\ u + v = z)
    return res;
}

/*
 * y = 139 * x -> 128x = 64x+64x; 64x = 32x+32x; ... 2x = x+ x. I.e., we need O(log(139)) new automata.
 *
*/


///
/// \param pow
/// \return an automaton for ( 2^pow * x = y)
NFA *NFA_xy_pow2(int pow){
    NFA *sum = NFA_from_file("../automata/lsd/sum.txt");
    NFA *eq = NFA_from_file("../automata/lsd/x_eq_y.txt");

    NFA **pow2_NFAs = (NFA**)malloc((1+pow)*sizeof (NFA*)); // each nfa for  2^i * x = y
    pow2_NFAs[0] = NFA_from_file("../automata/lsd/x_eq_y.txt");

    if(pow>0) {
        NFA *eq_ex = NFA_extend(eq, 2);// x=y,z
        NFA *res = NFA_intersection(sum, eq_ex); // (x=y,z) /\ (x+y=z) == ()
        pow2_NFAs[1] = NFA_project(res, 0); //2*x=y
        NFA_free(eq_ex);
        NFA_free(res);
    }
    NFA *tmp1=NULL,*tmp2=NULL,*tmp3=NULL;

    for(int i = 2; i <= pow; ++i) {
        tmp1 = NFA_extend(pow2_NFAs[i-1],0);
        tmp2 = NFA_extend(pow2_NFAs[1],2);
        tmp3 = NFA_intersection(tmp1,tmp2);
        NFA *tmp4 = NFA_to_DFA(tmp3);
        NFA *tmp5 = DFA_minimization(tmp4);
        pow2_NFAs[i] = NFA_project(tmp5,1);
        NFA_free(tmp1);
        NFA_free(tmp2);
        NFA_free(tmp3);
        NFA_free(tmp4);
        NFA_free(tmp5);
    }

    NFA_free(sum);
    NFA_free(eq);

    for(int i=0; i<pow; ++i){
        NFA_free(pow2_NFAs[i]);
    }
    NFA *res_ = pow2_NFAs[pow];

//    NFA *res2 = NFA_to_DFA(res_);
//    NFA *res3 = DFA_minimization(res2);
//    NFA_free(res_);
//    NFA_free(res2);
    free(pow2_NFAs);
    return res_;
}

NFA *kill_zeroes(NFA *a, const NFA *orig) {
    NFA *res = NFA_copy(a);


    int *new_final = zeros(res->states_cnt);
    int any_final_on_zeros_at_all = 1;


    while (any_final_on_zeros_at_all) {
        any_final_on_zeros_at_all = 0;

        for (int i = 0; i < res->states_cnt; ++i) {

            int any_final_on_zeros = 0;
            int *reachable = zeros(res->states_cnt);
            NFA_transition *curr_tr = res->states[i]->transitions;
            while (curr_tr) {
                if (curr_tr->transition_trigger == 0) {
                    if (res->states[curr_tr->state_to_ix]->is_final) {
                        any_final_on_zeros = 1;
                    }
                    reachable[curr_tr->state_to_ix] = 1;
                }
                curr_tr = curr_tr->next;
            }

            int changes = 1;
            while (changes) {
                changes = 0;
                for (int j = 0; j < res->states_cnt; ++j) {
                    if (reachable[j]) {
                        curr_tr = res->states[j]->transitions;
                        while (curr_tr) {
                            if (curr_tr->transition_trigger == 0) {
                                if (res->states[curr_tr->state_to_ix]->is_final) {
                                    any_final_on_zeros = 1;
                                }
                                if (reachable[curr_tr->state_to_ix] == 0) { changes = 1; }
                                reachable[curr_tr->state_to_ix] = 1;
                            }
                            curr_tr = curr_tr->next;
                        }
                    }
                }
            }

            if (any_final_on_zeros) {
                any_final_on_zeros_at_all = 1;
                res->states[i]->is_final = 1;
                new_final[i]=1;
                for (int j = 0; j < res->states_cnt; ++j) {
                    if (reachable[j] && res->states[j]->is_final) {
//                            printf("%d is gone1\n", j);
                        res->states[j]->is_final = 0;
                    }
                }
            }
            free(reachable);
        }


    }


    for (int i = 0; i < a->states_cnt; ++i) {
        if (a->states[i]->is_final) {
            NFA_transition *curr = a->states[i]->transitions;
            while (curr) {
                if (curr->transition_trigger == 0) {
                    if (curr->state_to_ix == i) {
                        res->states[i]->is_final = 1;
                    }
                }
                curr = curr->next;
            }
        }
    }


    for (int i = 0; i < res->states_cnt; ++i) {
        if (res->states[i]->is_final) {

            int exists = 0;
            NFA_transition *curr1 = res->states[i]->transitions;

            while (curr1) {
                if (curr1->state_to_ix == i && curr1->transition_trigger == 0) {
                    exists = 1;
                    break;
                }
                curr1 = curr1->next;
            }

            if (!exists) { continue; }

            int tr_to_self_by_zero_in_a = 0;
            NFA_transition *curr = orig->states[i]->transitions;

            while (curr) {
                if (curr->state_to_ix == i && curr->transition_trigger == 0) {
                    tr_to_self_by_zero_in_a = 1;
                }
                curr = curr->next;
            }
            if (!tr_to_self_by_zero_in_a) {
//                    printf("%d is gone2\n", i);
                res->states[i]->is_final = 0;
            }

        }
    }



    return res;
}

NFA *kill_zeroes2(NFA *a) {
    NFA *res = NFA_copy(a);
    int changes = 1;

    while(changes){
        changes = 0;

        for(int i=0; i<res->states_cnt; ++i){
            NFA_transition *curr_tr = res->states[i]->transitions;
            while(curr_tr){
                if( (curr_tr->transition_trigger == 0) && (res->states[curr_tr->state_to_ix]->is_final) ){
                    if(!res->states[i]->is_final){changes = 1;}
                    res->states[i]->is_final=1;
                }
                curr_tr = curr_tr->next;
            }
        }

    }

    return res;
}

NFA *NFA_const(int n){
    NFA *a = NFA_init(1);

    int bit_cnt = 0, n2 = n;
    while (n2) {n2 >>= 1; bit_cnt++;}

    for(int i=0; i <= bit_cnt+1; ++i){
        NFA_add_state(a,0,0);
    }

    for(int i=0; i <= bit_cnt-1; ++i){
        NFA_add_transition(a, i, i+1, n&1    );
        NFA_add_transition(a, i, bit_cnt+1, !(n&1) );
        n >>= 1;
    }

    NFA_add_transition(a, bit_cnt, bit_cnt, 0);
    NFA_add_transition(a, bit_cnt, bit_cnt+1, 1);
    NFA_add_transition(a, bit_cnt+1, bit_cnt+1, 0);
    NFA_add_transition(a, bit_cnt+1, bit_cnt+1, 1);
    a->states[bit_cnt]->is_final = 1;
    a->states[0]->is_starting = 1;
    return a;
}

//fix
NFA *NFA_leftquo(const NFA *a1,const NFA *a2){
    if(a1->dim != a2->dim){printf("a1 dim != a2 dim");exit(1);}
    NFA *res = NFA_copy(a1);

    for(int i=0; i<res->states_cnt; ++i){
        res->states[i]->is_starting=0;
    }

    int *new_final = calloc(res->states_cnt, sizeof(int));

    for(int i=0; i<res->states_cnt; ++i){
        res->states[i]->is_starting=1;
        NFA *tmp = NFA_intersection(res,a2);
        res->states[i]->is_starting=0;
        // Reachability Problems 2024
        int *starting = calloc(tmp->states_cnt, sizeof(int));
        //reachable != starting make in oher way
        for(int j=0; j<tmp->states_cnt; ++j){ starting[j] = tmp->states[j]->is_starting; }

        int *reachable = calloc(tmp->states_cnt, sizeof(int));

        for(int j=0; j<tmp->states_cnt; ++j){
            if(starting[j]){
                NFA_transition *curr_tr = tmp->states[j]->transitions;
                while(curr_tr){
                    reachable[curr_tr->state_to_ix] = 1;
                    curr_tr = curr_tr->next;
                }
            }
        }
        free(starting);
        while(1){
            int new = 0;
            for(int j=0; j<tmp->states_cnt; ++j){
                if(reachable[j]){
                    NFA_transition *curr_tr = tmp->states[j]->transitions;
                    while(curr_tr){
                        if(reachable[curr_tr->state_to_ix] == 0){new = 1;}
                        reachable[curr_tr->state_to_ix] = 1;
                        curr_tr = curr_tr->next;
                    }
                }
            }
            if(!new){break;}
        }

        for(int j=0; j<tmp->states_cnt; ++j){
            if(reachable[j] && tmp->states[j]->is_final){
                new_final[i] = 1;
                break;
            }
        }
        NFA_free(tmp);
        free(reachable);
    }

    for(int i=0; i<res->states_cnt; ++i){
        res->states[i]->is_final = new_final[i];
        res->states[i]->is_starting = a1->states[i]->is_starting;
    }
    free(new_final);
    return res;
}


int *NFA_reachable_by(const NFA *a, const int *states_set, int trigger){
    int *res = calloc(a->states_cnt,sizeof(int));
    for(int i=0; i<a->states_cnt; ++i){
        if(states_set[i]){
            NFA_transition *curr_tr = a->states[i]->transitions;
            while(curr_tr){
                if(curr_tr->transition_trigger == trigger){ res[curr_tr->state_to_ix] = 1; }
                curr_tr = curr_tr->next;
            }
        }
    }
    return res;
}


int *NFA_eps_cl(const NFA *a, const int *states_set){
    int *res = calloc(a->states_cnt,sizeof(int));

    for(int i=0; i<a->states_cnt; ++i){
        if(states_set[i]){
            res[i]=1;
            NFA_transition *curr_tr = a->states[i]->transitions;
            while(curr_tr){
                if(curr_tr->transition_trigger == -1){ res[curr_tr->state_to_ix] = 1; }
                curr_tr = curr_tr->next;
            }
        }
    }

    int changes = 1;
    while(changes){
        changes = 0;
        for(int j=0; j<a->states_cnt; ++j){
            if(res[j]){
                NFA_transition *curr_tr = a->states[j]->transitions;
                while(curr_tr){
                    if(curr_tr->transition_trigger==-1){
                        if(res[curr_tr->state_to_ix]==0){changes=1;}
                        res[curr_tr->state_to_ix]=1;
                    }
                    curr_tr=curr_tr->next;
                }
            }
        }
    }


    return res;
}


NFA *DFA_minimization(const NFA *a){
    if(a->dim==0){printf("minimization got 0-dim\n");exit(1);}
    if(!NFA_is_dfa(a)){printf("expected DFA, got NFA\n"); exit(1);}

    // tr[ix_from][trigger] -> ix_to
    int transitions[a->states_cnt][1 << a->dim];

    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            transitions[i][curr_tr->transition_trigger] = curr_tr->state_to_ix;
            curr_tr = curr_tr->next;
        }
    }

    int partition[a->states_cnt];
    int any_final=0;
    int any_not_final=0;
    for (int i = 0; i < a->states_cnt; ++i) {
        partition[i] = a->states[i]->is_final ? 1 : 0;
        if(a->states[i]->is_final){any_final=1;}
        else{any_not_final=1;}
    }

    if( !any_not_final || !any_final){
        return NFA_copy(a);
    }
//    printf("%d\n",components_cnt);
    //HERE

    int components_cnt = 2, components_cnt_new, changed;

    do {
        changed = 0;
        components_cnt_new = components_cnt;
        for(int component_ix = 0; component_ix < components_cnt; ++component_ix) {

            //finding first state in current component
            int ix = -1;
            for (int i = 0; i < a->states_cnt; ++i) {
                if(partition[i] == component_ix){
                    ix = i;
                    break;
                }
            }

            //creating a new component for states which are not equivalent to the first state
            int any_not_eq = 0;
            for(int i = ix + 1; i<a->states_cnt; ++i){
                if(partition[i] == component_ix) {
                    for (int k = 0; k < (1 << a->dim); ++k) {
                        int state1 = transitions[ix][k];
                        int state2 = transitions[i][k];
                        if (partition[state1] != partition[state2]) {
                            any_not_eq = 1;
                            partition[i] = components_cnt_new;
                        }
                    }
                }
            }
            if(any_not_eq){
                ++components_cnt_new;
                changed = 1;
            }
        }
        components_cnt = components_cnt_new;
    } while (changed);

    //choosing one state for each set in partition
    int representatives[components_cnt];
    for(int i=0; i<a->states_cnt; ++i){
        representatives[partition[i]] = i;
    }

    int new_starting_state_ix;
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_starting){ new_starting_state_ix = partition[i]; break;}
    }
    //print_array(representatives,components_cnt);
    //add states to a new automaton
    NFA *res = NFA_init(a->dim);
    for(int i=0; i < components_cnt; ++i){
        NFA_add_state(res,a->states[representatives[i]]->is_final, i == new_starting_state_ix);
    }

    //add transitions to the new automaton
    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *tr = a->states[i]->transitions;
        while(tr){
            NFA_add_transition(res,partition[i],partition[tr->state_to_ix],tr->transition_trigger);
            tr = tr->next;
        }
    }

    NFA *res2 = NFA_remove_dead_states(res);
    NFA_free(res);

    return res2;
}


NFA *NFA_remove_dead_states(const NFA *a){

    int *reachable = calloc(a->states_cnt,sizeof(int));
    NFA *res = NFA_copy(a);
    for(int i=0; i<a->states_cnt; ++i){
        reachable[i] = a->states[i]->is_starting;
    }

    //marking all reachable states
    while(1){
        int new = 0;
        for(int i=0; i<a->states_cnt; ++i){
            if(reachable[i]){
                NFA_transition *curr_tr = a->states[i]->transitions;
                while(curr_tr){
                    if(reachable[curr_tr->state_to_ix] == 0){new = 1;}
                    reachable[curr_tr->state_to_ix] = 1;
                    curr_tr = curr_tr->next;
                }
            }
        }
        if(!new){break;}
    }

    int old_new_states_map[res->states_cnt];
    NFA *res2 = NFA_init(res->dim);

    for(int i=0; i<a->states_cnt; ++i){

        //remove transitions TO unreachable states
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            if(!reachable[curr_tr->state_to_ix]){
                NFA_remove_transition(res,i,curr_tr->state_to_ix,curr_tr->transition_trigger);
            }
            curr_tr = curr_tr->next;
        }

        //remove transitions FROM unreachable states
        if(!reachable[i]){
            NFA_transition *tr = res->states[i]->transitions;
            if (tr != NULL) {
                NFA_transition *tmp = tr;
                while (tr->next != NULL) {
                    tmp = tr;
                    tr = tr->next;
                    free(tmp);
                }
                free(tr);
            }
            res->states[i]->transitions=NULL;
        }

        //add reachable states to new automata
        if(reachable[i]){
            old_new_states_map[i] = res2->states_cnt;
            NFA_add_state(res2, res->states[i]->is_final, res->states[i]->is_starting);
        }

    }

    //copy transitions from to new automata
    for(int i=0; i<res->states_cnt; ++i){
        NFA_transition *tr = res->states[i]->transitions;
        while(tr){
            NFA_add_transition(res2, old_new_states_map[i], old_new_states_map[tr->state_to_ix], tr->transition_trigger);
            tr = tr->next;
        }
    }

    free(reachable);
    NFA_free(res);

    return res2;
}


void NFA_complete(NFA *a){
    for(int i=0; i<a->states_cnt; ++i){
        int *exists = zeros(1 << a->dim);
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            if(curr_tr->transition_trigger!=-1){
                exists[curr_tr->transition_trigger] = 1;
            }
            curr_tr = curr_tr->next;
        }
        for(int j=0; j<(1<<a->dim); ++j){
            if(!exists[j]){
                NFA_add_transition(a,i,i,j);
            }
        }
        free(exists);
    }
}


//returns 1 if equal, 0 othewise
int arrays_are_equal(int *a1, int *a2, int len){
    for(int i=0; i<len; ++i){
        if(a1[i] != a2[i]){return 0;}
    }
    return 1;
}


//returns 1 if this subset already exists, 0 otherwise
int add_subset(int *subset, int ***subsets, int *subsets_cnt, int subset_len) {
    for (int i = 0; i < *subsets_cnt; ++i) {
        if (arrays_are_equal(subset, (*subsets)[i], subset_len)) {
            return i;
        }
    }

    int **tmp = (int **)realloc(*subsets, (*subsets_cnt + 1) * sizeof(int *));
    if (!tmp) {
        printf("Memory allocation error in add_subset\n");
        exit(1);
    }

    (*subsets) = tmp;
    (*subsets)[*subsets_cnt] = subset;
    (*subsets_cnt)++;

    return 0;
}


void print_subsets(int subsets_cnt, int subset_len, int **subsets){
    for(int i=0; i<subsets_cnt; ++i){
        print_array(subsets[i],subset_len);
    }
}


NFA *NFA_to_DFA(NFA *a){
    if(a->dim==0){return 0;}
    if(NFA_is_dfa(a)){return NFA_copy(a);}
    int **subsets;
    int **transitions = NULL;
    int subsets_cnt = 0, subsets_cnt2 = 0, changed = 1,transitions_cnt = 0;

    int *starting_states = zeros(a->states_cnt);
    for(int i=0; i<a->states_cnt; ++i){
        starting_states[i] = a->states[i]->is_starting;
    }
    subsets = realloc(subsets,(subsets_cnt+1)*sizeof(int*));
    subsets[0] = starting_states;
    ++subsets_cnt;
    ++subsets_cnt2;
    while(changed){

        changed = 0;

        for(int i=0; i<subsets_cnt; ++i){
            int *eps_cl = NFA_eps_cl(a,subsets[i]);
            free(subsets[i]);
            subsets[i] = eps_cl;
        }

        for(int i=0; i<subsets_cnt; ++i){
            for(int j=0; j<(1<<a->dim); ++j){
                int *reachable = NFA_reachable_by(a,subsets[i],j);
                int already_exists = add_subset(reachable,&subsets,&subsets_cnt2,a->states_cnt);
                int *tr = malloc(3*sizeof(int));
                tr[0]=i;
                tr[2]=j;
                transitions = (int**)realloc(transitions,(++transitions_cnt)*sizeof(int*));
                transitions[transitions_cnt-1] = tr;
                if(!already_exists){
                    tr[1]=subsets_cnt2-1;
                    changed = 1;
                }
                else{
                    tr[1]=already_exists;
                    free(reachable);
                }
                transitions[transitions_cnt-1] = tr;
            }
        }

        subsets_cnt = subsets_cnt2;

    }

    int *new_final = calloc(subsets_cnt,sizeof(int));
    for(int i=0; i<subsets_cnt; ++i){
        for(int j=0; j<a->states_cnt; ++j){
            if(subsets[i][j] && a->states[j]->is_final){
                new_final[i]=1;
                break;
            }
        }
    }


    NFA *res = NFA_init(a->dim);
    for(int i=0; i<subsets_cnt; ++i){
        NFA_add_state(res,new_final[i],0);//wtf
    }
    res->states[0]->is_starting = 1;


    for(int i=0; i<transitions_cnt; ++i){
        NFA_add_transition(res,transitions[i][0],transitions[i][1],transitions[i][2]);
    }

    for(int i=0; i<transitions_cnt; ++i){
        free(transitions[i]);
    }
    free(transitions);

    for(int i=0; i<subsets_cnt; ++i){
        free(subsets[i]);
    }

    free(subsets);
    free(new_final);

    return res;
}


NFA *NFA_div_n(int n){
    NFA *a1 = NFA_mult_scalar(n); // a*x=y
    NFA *a2 = NFA_project(a1,0);
    NFA *a3 = NFA_to_DFA(a2);
    NFA *a4 = DFA_minimization(a3);
    NFA_free(a1);
    NFA_free(a2);
    NFA_free(a3);
    return a4;
}


// linear terms (at least over x,y,z), i.e. a*x+b*y+c*z+d // a*x
// NFA_lin_term(int *a, int n) // an array of size n for the linear term a[0]+a[1]*x[1]+...+a[n-1]*x[n-1]
// returns an NFA of dim n that recognize ( a[0]+a[1]*x[1]+...+a[n-1]*x[n-1] = y)
// Next, we define substitution of a linear term (NFA *lin for (a*x+b*y+c*z+d = t)) into an automaton NFA *a
// function: NFA_subs(NFA *a, NFA *lin) -> exists t (a(t) /\  a[0]+a[1]*x[1]+...+a[n-1]*x[n-1] = t)
// div3(x+2) -> x s.t. x = 1 (mod 3)
// Ex($div2(2x+1)) -> False


//returns an NFA for  factor * x + bias  = y
NFA *NFA_lin_term(int factor, int bias){

    //f(x,y): a*x + bias = y <-> Eu,v [ (a * x = u) /\ (v = bias) /\ ( u + v = y)  ]

    NFA *mult = NFA_mult_scalar(factor);
    NFA *bias_a = NFA_const(bias);
    NFA *sum =  NFA_from_file("../automata/lsd/sum.txt");

    NFA* sum1 = NFA_extend(sum,0);
    NFA_free(sum);
    NFA *mult1 = NFA_extend(mult,2);
    NFA_free(mult);
    mult = NFA_extend(mult1,3);
    NFA_free(mult1);


    NFA *bias_a1 = NFA_extend(bias_a,1);
    NFA_free(bias_a);
    bias_a = NFA_extend(bias_a1,0);
    NFA_free(bias_a1);
    bias_a1 = NFA_extend(bias_a,0);
    NFA_free(bias_a);


    NFA *res = NFA_intersection(mult,bias_a1);
    NFA_free(mult);
    NFA_free(bias_a1);
    NFA *res1 = NFA_intersection(res,sum1);
    NFA_free(sum1);
    NFA_free(res);

    res = NFA_project(res1,1);
    NFA_free(res1);
    res1 = NFA_project(res,1);
    NFA_free(res);


    NFA *tmp1 = NFA_to_DFA(res1);
    NFA_free(res1);
    NFA *tmp2 = DFA_minimization(tmp1);
    NFA_free(tmp1);

    return tmp2;
}

// input: (a, lin), where a->dim == 1.
// returns: an automaton of dim (lin->dim - 1) for a(lin).
// Et (a(t) /\ lin(x,t)), where lin(x,t) <-> a*x+b = t
NFA *NFA_subs(NFA *a, NFA *lin){
    NFA* tmp1 = NFA_copy(a);
    for (int i = 0; i < lin->dim - 1; ++i)
    {
        NFA *tmp2 = NFA_extend(tmp1,0);
        NFA_free(tmp1);
        tmp1 = tmp2;
    }
    // (a(x,t))
    NFA *tmp2 = NFA_intersection(tmp1,lin); // a( [x], t )  /\  coef*x = t
    NFA *tmp3 = NFA_project(tmp2,lin->dim - 1);
    NFA_free(tmp1);
    NFA_free(tmp2);

    // Questionable ...
    tmp1 = NFA_to_DFA(tmp3);
    NFA_free(tmp3);
    tmp2 = DFA_minimization(tmp1);
    NFA_free(tmp1);
    // Consider commenting out

    return tmp2;
}

int NFA_th_check(NFA *a){

    int max_len=31,processed_words=0;
    int *curr_states = calloc(a->states_cnt,sizeof(int));
    for(int i=0; i<a->states_cnt; ++i){
        curr_states[i] = a->states[i]->is_starting;
    }

    while(1) {
        int is_transition = 0;
        int curr_states2[a->states_cnt];
        for(int i=0; i<a->states_cnt; ++i){
            curr_states2[i] = 0;
        }

        for(int i=0; i<a->states_cnt; ++i) {
            if(curr_states[i] == 1) {

                NFA_transition *curr_tr = a->states[i]->transitions;

                while (curr_tr != NULL) {
                    if (  curr_tr->transition_trigger == -1 ) {
                        curr_states[curr_tr->state_to_ix] = 1;
                    }
                    curr_tr = curr_tr->next;
                }

                curr_tr = a->states[i]->transitions;
                while (curr_tr != NULL) {
                    curr_states2[curr_tr->state_to_ix] = 1;
                    is_transition = 1;
                    curr_tr = curr_tr->next;
                }

            }
        }

        if(is_transition) {
            for (int i=0; i<a->states_cnt; ++i) {
                curr_states[i] = curr_states2[i];
            }
        }
        processed_words++;
        if(processed_words >= max_len){break;}
    }


    for(int i=0; i<a->states_cnt; ++i){
        if(curr_states[i] & a->states[i]->is_final){ free(curr_states); return  a->straight ? 1 : 0;}
    }
    free(curr_states);

    return a->straight ? 0 : 1;
}
