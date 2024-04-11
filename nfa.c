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


//eps tr to final states???
int NFA_check(const NFA *a, const int *sentences){
    int *sents2 = malloc(a->dim * sizeof(int));
    int max_len = -1, is_transition = 0, processed_words = 0, curr_wrd = 0, bit_cnt = 0;

    for(int i=0; i<a->dim; i++){
        sents2[i] = sentences[i];
        if(bit_len(sentences[i])>max_len){
            max_len=bit_len(sentences[i]);
        }
    }

    int *curr_states = calloc(a->states_cnt,sizeof(int));
    for(int i=0; i<a->states_cnt; i++){
        curr_states[i] = a->states[i]->is_starting;
    }
    while(1) {
        curr_wrd = 0;
        bit_cnt = 0;
        for (int j = 0; j < a->dim; j++) {
            curr_wrd += (sents2[j] & 1) << bit_cnt;
            bit_cnt++;
            sents2[j] = sents2[j]>>1;
        }


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
                    if (  curr_wrd == curr_tr->transition_trigger ) {
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
        if(processed_words >= max_len){break;}
    }

    free(sents2);

    for(int i=0; i<a->states_cnt; i++){
        if(curr_states[i] & a->states[i]->is_final){ free(curr_states); return 1;}
    }
    free(curr_states);
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
                      "\trankdir=LR;\n";

    fprintf(f,template);
    int first=1;
    for(int i=0; i<a->states_cnt; i++){

        if(a->states[i]->is_final == 1){
            if(first){fprintf(f,"\tnode [shape = doublecircle ];");first=0;}
             fprintf(f," %d", i);
        }
    }
    if(!first){fprintf(f,";\n");}
    fprintf(f,"\tnode [shape = circle];\n");

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

    fprintf(f,"%d\n", a->dim);
    fprintf(f,"%d ", a->states_cnt);

    for(int j=0; j<a->states_cnt; j++){
        fprintf(f,"%d ", a->states[j]->is_final);
    }
    fprintf(f,"\n");

    for(int j=0; j<a->states_cnt; j++){
        fprintf(f,"%d ", a->states[j]->is_starting);
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
    int dim, cnt_st, cnt_tr;


    fscanf(f,"%d\n", &dim);
    fscanf(f,"%d", &cnt_st);

    int final[cnt_st];
    int starting[cnt_st];
    for(int i=0; i<cnt_st; i++){
        fscanf(f,"%d", &final[i]);
    }

    for(int i=0; i<cnt_st; i++){
        fscanf(f,"%d", &starting[i]);
    }

    fscanf(f,"%d\n", &cnt_tr);

    NFA *a = NFA_init(dim);

    for(int i=0; i<cnt_st; i++){
        NFA_add_state(a,final[i],starting[i]);
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
    if(a1->dim != a2->dim){printf("a1 dim != a2 dim");exit(1);}

    NFA *res = NFA_init(a1->dim);
    NFA_add_state(res,0,1);

    for(int i=0; i<a1->states_cnt; i++){
        NFA_add_state(res, a1->states[i]->is_final,0);
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
        NFA_add_state(res, a2->states[i]->is_final,0);
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
    if(a1->dim != a2->dim){printf("a1 dim != a2 dim");exit(1);}
    NFA *res = NFA_init(a1->dim);

    for(int i=0; i<a1->states_cnt; i++){
        for(int j=0; j<a2->states_cnt; j++){
            NFA_add_state(res,
                          a1->states[i]->is_final && a2->states[j]->is_final,
                          a1->states[i]->is_starting && a2->states[j]->is_starting);
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


NFA *NFA_swap_digits(const NFA *a, int n1, int n2){
    if(n1 >= a->dim || n2 >= a->dim) {printf("out of dim exception.\n");exit(1);}
    NFA *res = NFA_copy(a);
    for(int i=0; i<res->states_cnt; i++){
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
    for(int i=0; i<a->states_cnt; i++){
        NFA_add_state(res, a->states[i]->is_final,a->states[i]->is_starting);
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



NFA *NFA_project(const NFA *a, int num_cord){
    if(num_cord >= a->dim){printf("out of dim exception\n"); exit(1);}
    NFA *res = NFA_copy(a);
    res->dim = res->dim -1;
    for(int i=0; i<res->states_cnt; i++){
        NFA_transition *curr_tr=res->states[i]->transitions;
        while(curr_tr){
            curr_tr->transition_trigger = project_int(curr_tr->transition_trigger, num_cord);
            curr_tr=curr_tr->next;
        }
    }
    return res;
}


NFA *NFA_copy(const NFA *a){
    NFA *res = NFA_init(a->dim);
    for(int i=0; i<a->states_cnt; i++){
        NFA_add_state(res, a->states[i]->is_final,a->states[i]->is_starting);
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


//?
///
/// \param coeff a coefficient for a variable y
/// \return an automaton for (x = coeff * y)
NFA *NFA_mult_scalar(int coeff){
    NFA *res =  NFA_from_file("../automatons/lsd/zeros.txt");//NFA_xy_pow2(0);
    NFA *zero =  NFA_from_file("../automatons/lsd/zeros.txt");//NFA_xy_pow2(0);
    res->dim=2;
    NFA *tmp1,*tmp2, *tmp3;
    NFA *sum =  NFA_from_file("../automatons/lsd/sum.txt");//NFA_xy_pow2(0);

    for(int cnt = 0; coeff; coeff >>= 1){
        if(coeff & 1){
            printf("%d\n",cnt);
            //x,y,z
            NFA *curr = NFA_xy_pow2(cnt); // 2^i * x = y
            tmp1 = NFA_project(curr,0);
            tmp1 = NFA_extend(tmp1,1);
            tmp1 = NFA_extend(tmp1,2);
            tmp2 = NFA_project(res,0);
            tmp2 = NFA_extend(tmp2,1);
            tmp2 = NFA_extend(tmp2,0);
            printf("%d %d\n",tmp1->dim,tmp2->dim);
            tmp3 = NFA_intersection(tmp1,tmp2);
            res  = NFA_intersection(tmp3,sum);
        }
        cnt++;
    }
    // x + y = z -> (2^i * y + k*y = z) <-> Eu,v (2^i * y = u /\ k*y = v /\ u + v = z)
    return res;
}

/*
 * y = 139 * x -> 128x = 64x+64x; 64x = 32x+32x; ... 2x = x+ x. I.e., we need O(log(139)) new automata.
 *
*/

//?
///
/// \param pow
/// \return an automaton for ( 2^pow * x = y)
NFA *NFA_xy_pow2(int pow){
    NFA *sum = NFA_from_file("../automatons/lsd/sum.txt");
    NFA *eq = NFA_from_file("../automatons/lsd/x_eq_y.txt");


    NFA **pow2_NFAs = (NFA**)malloc((1+pow)*sizeof (NFA*)); // each nfa for  2^i * x = y
    pow2_NFAs[0] = NFA_from_file("../automatons/lsd/x_eq_y.txt");

    NFA *eq_ex = NFA_extend(eq, 2);
    NFA *res = NFA_intersection(sum, eq_ex);
    pow2_NFAs[1] = NFA_project(res, 0); //2*x=y

    NFA*tmp1,*tmp2,*tmp3;

    for(int i = 2; i <= pow; ++i) {
        tmp1 = NFA_extend(pow2_NFAs[i-1],0);
        tmp2 = NFA_extend(pow2_NFAs[1],2);
        tmp3 = NFA_intersection(tmp1,tmp2);
        pow2_NFAs[i] = NFA_project(tmp3,1);
    }

    return pow2_NFAs[pow];
}


//?
///
/// \param pow
/// \return an automaton for (2^pow | x)
NFA *NFA_mult_of_pow2(int pow){
    NFA *a = NFA_init(1);
    for(int i=0; i<=pow; i++){
        NFA_add_state(a,0,0);
    }
    a->states[pow]->is_final = 1;
    NFA_add_state(a,0,0);
    NFA_add_state(a,1,0);

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

    a->states[0]->is_starting = 1;
    return a;
}



NFA *NFA_const(int n){
    NFA *a = NFA_init(1);

    int bit_cnt = 0, n2 = n;
    while (n2) {n2 >>= 1; bit_cnt++;}

    for(int i=0; i <= bit_cnt+1; i++){
        NFA_add_state(a,0,0);
    }

    for(int i=0; i <= bit_cnt-1; i++){
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
    for(int i=0; i<res->states_cnt; i++){
        res->states[i]->is_starting = 0;
    }

    int *new_final= zeros(res->states_cnt);

    for(int i=0; i<res->states_cnt; i++){
        res->states[i]->is_starting = 1;
        NFA *tmp = NFA_intersection(res,a2);
        res->states[i]->is_starting = 0;

        int *curr_states = zeros(tmp->states_cnt);
        for(int j=0; j<tmp->states_cnt; j++){ curr_states[j] = tmp->states[j]->is_starting;}

        while(1){

            int *curr_states_new  = zeros(tmp->states_cnt);

            for(int j=0; j<tmp->states_cnt; j++){
                if(curr_states[j]){
                    NFA_transition *curr_tr = tmp->states[j]->transitions;
                    while(curr_tr){
                        curr_states_new[curr_tr->state_to_ix] = 1;
                        curr_tr = curr_tr->next;
                    }
                }
            }


            int all_zeros = 1, exists_path = 0;
            for(int j=0; j<tmp->states_cnt; j++){
                if( (curr_states_new[j]) && (!curr_states[j]) ){ all_zeros = 0; }
                if( (curr_states_new[j]) && (tmp->states[j]->is_final)){ exists_path = 1; new_final[i] = 1; }
            }

            if(all_zeros || exists_path){ free(curr_states_new);break; }

            for(int m=0; m<tmp->states_cnt; m++){
                curr_states[m] = curr_states_new[m];
            }

            free(curr_states_new);
        }
        free(curr_states);
        NFA_free(tmp);
    }

    for(int i=0; i<a1->states_cnt; i++){
        res->states[i]->is_starting = a1->states[i]->is_starting;
        res->states[i]->is_final = new_final[i];
    }

    free(new_final);

    return res;
}


//fix
NFA *NFA_rightquo(const NFA *a1,const NFA *a2){
    if(a1->dim != a2->dim){printf("a1 dim != a2 dim");exit(1);}
    NFA *res = NFA_copy(a1);
    for(int i=0; i<res->states_cnt; i++){
        res->states[i]->is_final = 0;
    }

    int *new_starting= zeros(res->states_cnt);

    for(int i=0; i<res->states_cnt; i++){
        res->states[i]->is_final = 1;
        NFA *tmp = NFA_intersection(res,a2);
        res->states[i]->is_final = 0;

        int *curr_states = zeros(tmp->states_cnt);
        for(int j=0; j<tmp->states_cnt; j++){ curr_states[j] = tmp->states[j]->is_starting;}

        while(1){
            int *curr_states_new = zeros(tmp->states_cnt);

            for(int j=0; j<tmp->states_cnt; j++){
                if(curr_states[j]){
                    NFA_transition *curr_tr = tmp->states[j]->transitions;
                    while(curr_tr){
                        curr_states_new[curr_tr->state_to_ix]=1;
                        curr_tr=curr_tr->next;
                    }
                }
            }

            int all_zeros = 1, exists_path = 0;
            for(int j=0; j<tmp->states_cnt; j++){
                if( (curr_states_new[j]) && (!curr_states[j]) ){ all_zeros = 0; }
                if( ( curr_states_new[j] || curr_states[j] ) && (tmp->states[j]->is_final)){ exists_path = 1; new_starting[i]=1; }
            }

            if(all_zeros || exists_path){ free(curr_states_new);break; }

            for(int m=0; m<tmp->states_cnt; m++){
                curr_states[m]=curr_states_new[m];
            }

            free(curr_states_new);
        }
        free(curr_states);
        NFA_free(tmp);
    }

    for(int i=0; i<a1->states_cnt; i++){
        res->states[i]->is_final = a1->states[i]->is_final;
        res->states[i]->is_starting = new_starting[i];
    }

    free(new_starting);

    return res;
}


//?
NFA *NFA_n_eq(int n){
    if(n<2){printf("nfa_n_eq");exit(1);}

    NFA *eq = NFA_from_file("../automatons/lsd/x_eq_y.txt");
    NFA *eq2 = NFA_from_file("../automatons/lsd/x_eq_y.txt");
    NFA *tmp;
    for(int i=0; i<n-2; ++i ){
        tmp = eq;
        eq = NFA_extend(tmp,0);
        NFA_free(tmp);

        tmp = eq2;
        eq2 = NFA_extend(tmp,2+i);
        NFA_free(tmp);

        tmp = eq;
        eq = NFA_intersection(eq,eq2);
        NFA_free(tmp);
    }
    NFA_free(eq2);

    return eq;
}


//?
NFA *NFA_n_sum(int n){
    if(n<2){printf("nfa_n_eq");exit(1);}

    NFA *sum2 = NFA_from_file("../automatons/lsd/sum.txt");
    NFA *zer =NFA_from_file("../automatons/lsd/zeros.txt");
    NFA *sum_sw = NFA_from_file("../automatons/lsd/sum.txt");


    sum2 = NFA_extend(sum2,1);

    for(int num=0; num<n-2; ++num){
        sum_sw = NFA_swap_digits(sum_sw,0,2+num);
        sum_sw = NFA_extend(sum_sw,3+num);
        sum_sw = NFA_extend(sum_sw,3+num+1);
        sum2 = NFA_extend(sum2,1);
        sum_sw = NFA_intersection(sum_sw,sum2);
        sum_sw = NFA_project(sum_sw,0);
    }

    NFA_free(sum2);
    NFA_free(zer);
    return sum_sw;

}



int *NFA_eps_closure(const NFA *a, int *states_set){
    int *res = calloc(a->states_cnt,sizeof(int));
    for(int i=0; i<a->states_cnt; ++i){
        if(states_set[i]){
            res[i] = 1;
            NFA_transition *curr_tr = a->states[i]->transitions;
            while(curr_tr){
                if(curr_tr->transition_trigger == -1){ res[curr_tr->state_to_ix] = 1; }
                curr_tr = curr_tr->next;
            }
        }
    }
    return res;
}



NFA *DFA_minimization(NFA *a){
    if(!NFA_is_dfa(a)){printf("expected DFA, got NFA\n"); exit(1);}

    int transitions[a->states_cnt][1 << a->dim];

    for(int i=0; i<a->states_cnt; i++){
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            transitions[i][curr_tr->transition_trigger] = curr_tr->state_to_ix;
            curr_tr=curr_tr->next;
        }
    }



    int partition[a->states_cnt];
    for (int i = 0; i < a->states_cnt; ++i) {
        partition[i] = a->states[i]->is_final ? 1 : 0;
    }
    int comp_cnt=2;
    int changed;
//    printf("starting\n");
//    print_array(partition,a->states_cnt);
    do {
        changed = 0;
        int comp_cnt2 = comp_cnt;
        for(int comp_num=0; comp_num<comp_cnt; ++comp_num) {
//            printf("**************  comp_num = %d  **************\n",comp_num);
            int ix=-1;
            for (int i = 0; i < a->states_cnt; ++i) {
                if(partition[i] == comp_num){
                    ix = i;
                    break;
                }
            }

            int any_not_eq=0;
            for(int i=ix+1; i<a->states_cnt; ++i){
                if(partition[i]==comp_num) {
                    for (int k = 0; k < (1 << a->dim); ++k) {
                        int state1 = transitions[ix][k];
                        int state2 = transitions[i][k];
                        if (partition[state1] != partition[state2]) {
//                            printf("%d not eq %d because:\n",ix,i);
//                            printf("    from %d by %d to %d which in %d\n",ix,k,state1,partition[state1]);
//                            printf("    from %d by %d to %d which in %d\n",i,k,state2,partition[state2]);
                            any_not_eq = 1;
                            partition[i] = comp_cnt2;
                        }
                    }
                }
            }
            if(any_not_eq){++comp_cnt2;changed=1;}
//            print_array(partition,a->states_cnt);
        }
        comp_cnt=comp_cnt2;

    } while (changed);
    printf("****** final version *******\n");
    print_array(partition,a->states_cnt);

    int representatives[comp_cnt];

    for(int i=0; i<a->states_cnt; ++i){
        representatives[partition[i]]=i;
    }

    printf("****** representatives *******\n");
    print_array(representatives,comp_cnt);//[new_set_ix][old_state_ix]

    int max=0;
    for(int i=0; i<a->states_cnt; ++i){
        if(partition[i]>max){
            max=partition[i];
        }
    }

    int starting;//new_starting_state
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_starting){starting=partition[i];break;}
    }

    NFA *res = NFA_init(a->dim);
    for(int i=0; i<=max; ++i){
        NFA_add_state(res,a->states[representatives[i]]->is_final,i==starting);
        if(a->states[representatives[i]]->is_final){
            printf("final = %d\n",i);
        }
        if(i==starting){
            printf("starting = %d\n",i);
        }
    }

    //add transitions


    //remove dead_states



    return res;
}





//NFA *NFA_div_n(int n) {
//    NFA *a = NFA_mult_scalar(n);
//    NFA *b = NFA_project(a,1);
//    NFA *zero = NFA_from_file("../automatons/lsd/zeros.txt");
//    NFA *result = NFA_leftquo(b, zero);
//    NFA_free(a);
//    NFA_free(b);
//    NFA_free(zero);
//    return result;
//}

//NFA *NFA_div_n(int n){
//    int n1=n;
//    if(n == 0){printf("0|x???");exit(1);}
//    Stack2 *stack = Stack2_init();
//    int num = 0, cnt = 0;
//
//    while(n){
//        if( n & 1 ){
//            cnt++;
//            Stack2_push(stack, NFA_is_mult_of_pow2(num));
//        }
//        n >>= 1;
//        num++;
//    }
//
//    if(cnt == 1){
//        NFA *res = Stack2_pop(stack);
//        Stack2_free(stack);
//        return res;
//    }
//    else{
//        NFA *eq = NFA_n_eq(n1);
//        eq = NFA_extend(eq,n1);
//
//        NFA *sm = NFA_n_sum(n1);
//
//        Stack2_free(stack);
//        return NFA_intersection(sm,eq);
//    }
//
//}





NFA *NFA_remove_dead_states(NFA *a){
    int *reachable = calloc(a->states_cnt,sizeof(int));
    NFA *res = NFA_copy(a);
    for(int i=0; i<a->states_cnt; ++i){
        reachable[i] = a->states[i]->is_starting;
    }

    while(1){
        int new=0;
        for(int i=0; i<a->states_cnt; ++i){
            if(reachable[i]){
                NFA_transition *curr_tr = a->states[i]->transitions;
                while(curr_tr){
                    if(reachable[curr_tr->state_to_ix]==0){new=1;}
                    reachable[curr_tr->state_to_ix] = 1;
                    curr_tr = curr_tr->next;
                }
            }
        }
        if(!new){break;}
    }

    for(int i=0; i<a->states_cnt; ++i){
        NFA_transition *curr_tr = a->states[i]->transitions;
        while(curr_tr){
            if(!reachable[curr_tr->state_to_ix]){
                NFA_remove_transition(res,i,curr_tr->state_to_ix,curr_tr->transition_trigger);
            }
            curr_tr = curr_tr->next;
        }
    }

    for(int i=0; i<a->states_cnt; ++i){
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
    }


    int sum_reachable=0;
    for(int i=0; i<res->states_cnt; ++i){
        if(reachable[i]){sum_reachable++;}
    }

    int old_new_states[res->states_cnt];
    int new_old_states[sum_reachable];


    NFA *res2 = NFA_init(res->dim);
    for(int i=0; i<res->states_cnt; ++i){
        if(reachable[i]){
            old_new_states[i] = res2->states_cnt;
            new_old_states[res2->states_cnt] = i;
            NFA_add_state(res2,res->states[i]->is_final,res->states[i]->is_starting);
        }
    }

    for(int i=0; i<res->states_cnt; ++i){
        NFA_transition *tr = res->states[i]->transitions;
        while(tr){
            NFA_add_transition(res2,old_new_states[i],old_new_states[tr->state_to_ix],tr->transition_trigger);
            tr=tr->next;
        }
    }

    free(reachable);
    NFA_free(res);
    return res2;

}




