//
// Created by lolikion on 16.02.24.
//
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int NFA_transitions_cnt(NFA_state *state){
    NFA_transition *curr = state->transitions;
    int cnt = 0;
    while(curr){
        cnt++;
        curr = curr->next;
    }
    return cnt;
}


NFA *NFA_init(int dim, int order){
    NFA *a = (NFA *)malloc(sizeof(NFA));
    if(!a){
        printf("memory allocation error in NFA_init_1\n");
        exit(1);
    }
    a->order      = order;
    a->states_cnt =     0;
    a->dim        =   dim;
    a->states     =  NULL;
    return a;
}


void NFA_add_state(NFA *a, int is_final){

    NFA_state *state = (NFA_state *)malloc(sizeof(NFA_state));
    if(!state){
        printf("memory allocation error in state_init1\n");
        exit(1);
    }
    state->index       = a->states_cnt;
    state->is_final    =      is_final;
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


void print_array(int a[], int len){
    printf("[");
    for(int i=0; i<len; i++){
        printf(" %d", a[i]);
    }
    printf(" ]\n");
}


int NFA_check(const NFA *a, big_int **sentences){
    big_int **sents2 = (big_int **)malloc(a->dim * sizeof(big_int*));
    int max_len = 0, is_transition = 0, processed_words = 0, curr_wrd = 0, bit_cnt = 0;

    for(int i=0; i<a->dim; i++){
        if(sentences[i]->bit_len >= max_len){
            max_len = sentences[i]->bit_len;
        }
        sents2[i] = big_int_copy(sentences[i]);
    }
    int bit_ix = max_len - 1;

    int curr_states[a->states_cnt];
    for(int i=0; i<a->states_cnt; i++){
        curr_states[i] = 0;
    }
    curr_states[0] = 1;

    while(1) {
        curr_wrd = 0;
        bit_cnt = 0;
        if(a->order == 0) {
            for (int j = 0; j < a->dim; j++) {
                curr_wrd += (sents2[j]->number[0] & 1) << bit_cnt;
                bit_cnt++;
                big_int_bin_shft_r(sents2[j]);
            }
        }
        else{
            for (int j = 0; j < a->dim; j++) {
                curr_wrd += ( ( sents2[j]->number[bit_ix/8] & ( 1 << (bit_ix & 7) ) ) != 0) << bit_cnt;
                bit_cnt++;
            }
        }
        bit_ix--;
//        printf("curr_wrd=%b\n",curr_wrd);

        is_transition = 0;
        int curr_states2[a->states_cnt];
        for(int i=0; i<a->states_cnt; i++){
            curr_states2[i] = 0;
        }

        for(int i=0; i<a->states_cnt; i++) {
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
                    if (  curr_wrd == curr_tr->transition_trigger  ) {
                        curr_states2[curr_tr->state_to_ix] = 1;
                        is_transition = 1;
                    }
                    curr_tr = curr_tr->next;
                }

            }
        }

        if(is_transition) {
            for (int i=0; i<a->states_cnt; i++) {
                curr_states[i] = curr_states2[i];
            }
        }

        processed_words++;
        if(processed_words == max_len){break;}
    }

    for(int i=0; i<a->dim; i++){
        big_int_free( &sents2[i]);
    }
    free(sents2);

    for(int i=0; i<a->states_cnt; i++){
        if(curr_states[i] & a->states[i]->is_final){return 1;}
    }
    return 0;
}

void NFA_print(const NFA* a){
    printf("automata_states:\n\n");
    printf("--------------------------------------------\n");
    for(int i=0; i<a->states_cnt; i++){
        printf("state_ix=%d; is_acceptable=%d; transitions=%d\n",
                    a->states[i]->index,a->states[i]->is_final,
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
    for(int i=0; i<a->states_cnt; i++){
        NFA_state_free(a->states[i]);
    }
    free(a->states);
    free(a);
}


void NFA_to_pic(const NFA *a){
    FILE *f = fopen("../fsm.gv", "w");

    char template[] = " digraph finite_state_machine {\n"
                      "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                      "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                      "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                      "\trankdir=LR;\n"
                      "\tnode [shape = doublecircle ];";

    fprintf(f,template);

    for(int i=0; i<a->states_cnt; i++){
        if(a->states[i]->is_final == 1){
             fprintf(f," %d", i);
        }
    }
    fprintf(f,";\n\tnode [shape = circle];\n");

    for(int i=0; i<a->states_cnt; i++){
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
    system("dot -Tpng ../fsm.gv -o ../temp/automata.png");
    system("rm ../fsm.gv");
}


void NFA_to_file(const NFA *a){
    FILE *f = fopen("../temp/automata.txt","w");
    fprintf(f,"%d\n", a->order);
    fprintf(f,"%d\n", a->dim);
    fprintf(f,"%d ", a->states_cnt);

    for(int j=0; j<a->states_cnt; j++){
        fprintf(f,"%d ", a->states[j]->is_final);
    }
    fprintf(f,"\n");

    int cnt_tr = 0;
    for(int j=0; j<a->states_cnt; j++){
        cnt_tr += NFA_transitions_cnt(a->states[j]);
    }
    fprintf(f,"%d\n", cnt_tr);

    for(int j=0; j<a->states_cnt; j++){
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
    int dim, cnt_st, cnt_tr, order;

    fscanf(f,"%d\n", &order);
    fscanf(f,"%d\n", &dim);
    fscanf(f,"%d", &cnt_st);

    int final[cnt_st];
    for(int i=0; i<cnt_st; i++){
        fscanf(f,"%d", &final[i]);
    }

    fscanf(f,"%d\n", &cnt_tr);

    NFA *a = NFA_init(dim,order);

    for(int i=0; i<cnt_st; i++){
        NFA_add_state(a,final[i]);
    }
    for(int i=0; i<cnt_tr; i++){
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
    if(a1->order != a2->order){ printf("a1 order != a2 order");exit(1); }
    NFA *res = NFA_init(a1->dim,a1->order);
    NFA_add_state(res,0);

    for(int i=0; i<a1->states_cnt; i++){
        NFA_add_state(res, a1->states[i]->is_final);
    }
    for(int i=0; i<a1->states_cnt; i++){
        NFA_transition* curr_tr = a1->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               1 + a1->states[i]->index,
                               1 + curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }

    for(int i=0; i<a2->states_cnt; i++){
        NFA_add_state(res, a2->states[i]->is_final);
    }
    for(int i=0; i<a2->states_cnt; i++){
        NFA_transition* curr_tr = a2->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               1 + a1->states_cnt + a2->states[i]->index,
                               1 + a1->states_cnt + curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }

    NFA_add_transition(res,0,1,-1);
    NFA_add_transition(res,0,a1->states_cnt+1,-1);
    return res;
}


NFA *NFA_intersection(const NFA *a1, const NFA *a2) {
//  ix_uni(ix_a1,ix_a2) = a2->states_cnt * ix_a1 + ix_a2
    if(a1->order != a2->order){printf("a1 order != a2 order");exit(1);}
    NFA *res = NFA_init(a1->dim,a1->order);

    for(int i=0; i<a1->states_cnt; i++){
        for(int j=0; j<a2->states_cnt; j++){
            NFA_add_state(res,a1->states[i]->is_final & a2->states[j]->is_final);
        }
    }

    for(int i=0; i<a1->states_cnt; i++){
        for(int j=0; j<a2->states_cnt; j++) {
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
    if(!NFA_is_dfa(a)){printf("complement only for DFA for now.\n");exit(1);}
    NFA *res = NFA_copy(a);
    for(int i=0; i<res->states_cnt; i++){
        res->states[i]->is_final = !res->states[i]->is_final;
    }
    return res;
}


int project_int(int n, int num_cord){
    int n1 = n & ( (1 << num_cord) - 1 );
    int n2 = n >> (num_cord + 1);
    return n1 + (n2 << num_cord);
}


int extend_int(int n, int num_cord, int bit){
    int n1 = n & ( (1 << num_cord) - 1 );
    int n2 = n >> (num_cord);
    return n1 + ( n2 << (num_cord + 1) ) + (bit << num_cord);
}


NFA *NFA_project(const NFA *a, int num_cord){
    if(num_cord >= a->dim){printf("num_cord >= dim.\n");exit(1);}
    NFA *res = NFA_copy(a);
    res->dim--;
    for(int i=0; i<res->states_cnt; i++){
        NFA_transition *curr_tr = res->states[i]->transitions;
        while(curr_tr){
            curr_tr->transition_trigger = project_int(curr_tr->transition_trigger, num_cord);
            curr_tr = curr_tr->next;
        }
    }
    return res;
}


NFA *NFA_extend(const NFA *a, int num_cord){
//    if(num_cord >= a->dim){printf("num_cord >= dim.\n"); exit(1);}
    NFA *res = NFA_init(a->dim + 1, a->order);
    for(int i=0; i<a->states_cnt; i++){
        NFA_add_state(res, a->states[i]->is_final);
    }

    for(int i=0; i<a->states_cnt; i++){
        int tr_cnt = NFA_transitions_cnt(a->states[i]);
        NFA_transition *curr_tr = a->states[i]->transitions;
        for(int j=0; j<tr_cnt; j++){
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


NFA *NFA_copy(const NFA *a){
    NFA *res = NFA_init(a->dim,a->order);
    for(int i=0; i<a->states_cnt; i++){
        NFA_add_state(res, a->states[i]->is_final);
    }
    for(int i=0; i<a->states_cnt; i++){
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                               a->states[i]->index,
                               curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }
    return res;
}


int NFA_is_dfa(const NFA *a){
    for(int i=0; i<a->states_cnt; i++){
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


NFA *NFA_is_mult_of_pow2(int pow){
    NFA *a = NFA_init(1,0);
    for(int i=0; i<=pow; i++){
        NFA_add_state(a,0);
    }
    a->states[pow]->is_final = 1;
    NFA_add_state(a,0);
    NFA_add_state(a,1);

    for(int i=0; i<pow; i++){
        NFA_add_transition(a,i,i+1,0);
        NFA_add_transition(a,i,pow+1,1);
    }

    NFA_add_transition(a,pow+1,pow+1,0);
    NFA_add_transition(a,pow+1,pow+1,1);
    NFA_add_transition(a,pow,pow,0);
    NFA_add_transition(a,pow,pow,1);

    NFA_add_transition(a,0,pow+2,0);
    NFA_add_transition(a,pow+2,pow+2,0);
    NFA_add_transition(a,pow+2,pow+1,1);

    return a;
}


NFA *NFA_const(int n){
    NFA *a = NFA_init(1,0);

    big_int *num = big_int_from_int(n);
    int bit_cnt=0, n2=n;
    while (n2){n2 >>= 1; bit_cnt++;}
    num->bit_len = bit_cnt;

    for(int i=0; i <= bit_cnt+1; i++){
        NFA_add_state(a,0);
    }

    for(int i=0; i <= bit_cnt-1; i++){
        NFA_add_transition(a, i, i+1, n&1    );
        NFA_add_transition(a, i, bit_cnt+1, !(n&1) );
        n>>=1;
    }

    NFA_add_transition(a, bit_cnt, bit_cnt, 0);
    NFA_add_transition(a, bit_cnt, bit_cnt+1, 1);
    NFA_add_transition(a, bit_cnt+1, bit_cnt+1, 0);
    NFA_add_transition(a, bit_cnt+1, bit_cnt+1, 1);
    a->states[bit_cnt]->is_final=1;

    big_int_free(&num);
    return a;
}

