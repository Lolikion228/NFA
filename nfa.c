//
// Created by lolikion on 16.02.24.
//
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "other/linked_list.h"


// NFA over \{0,1\}^{dim}
NFA *NFA_init(int dim){
    NFA *a = (NFA *)malloc(sizeof(NFA));
    if(!a){
        printf("memory allocation error in NFA_init_1\n");
        exit(1);
    }
    a->states_cnt=1;
    a->dim = dim;
    a->initial_state=NFA_state_init(a,0);
    a->initial_state->index=0;
    a->states = malloc(sizeof(NFA_state));
    if(!a->states){
        printf("memory allocation error in NFA_init_2\n");
        exit(1);
    }
    a->states[0]=a->initial_state;
    return a;
}


NFA_state *NFA_state_init(NFA *a,int is_final){
    NFA_state *state = (NFA_state *)malloc(sizeof(NFA_state));
    if(!state){
        printf("memory allocation error in state_init1\n");
        exit(1);
    }
    state->index=a->states_cnt;
    state->accept_state=is_final;
    state->transitions=malloc(sizeof(struct list));
    if(!state->transitions){
        printf("memory allocation error in state_init2\n");
        exit(1);
    }
    state->transitions->head = NULL;
    state->transitions_cnt=0;
    return state;
}


void NFA_add_state(NFA *a,int is_final){
    NFA_state *state=NFA_state_init(a,is_final);
    a->states=realloc(a->states,((a->states_cnt)+1)*sizeof(NFA_state));
    if(!a->states){
        printf("memory allocation error in NFA_add_state\n");
        exit(1);
    }
    a->states_cnt++;
    a->states[a->states_cnt-1]=state;
}


void NFA_state_free(NFA_state *s){
    node_free(s->transitions->head);
    free(s->transitions);
    free(s);
}


int is_transitions_equal(NFA_transition *tr1,NFA_transition *tr2){
    if(!tr1 || !tr2){return 0;}
    return ( (tr1->state_to->index==tr2->state_to->index) && (tr1->state_from->index==tr2->state_from->index) && (tr1->transition_trigger==tr2->transition_trigger)  );
}


void NFA_add_transition(NFA *a, int state_from, int state_to, int trigger){

    NFA_transition *transition= (NFA_transition *)malloc(sizeof(NFA_transition));
    if(!transition){
        printf("memory allocation error in NFA_add_transition\n");
        exit(1);
    }
    transition->state_from=a->states[state_from];
    transition->state_to=a->states[state_to];
    transition->transition_trigger= trigger;
    node *curr=(a->states[state_from])->transitions->head;

    if(!curr){
        (a->states[state_from])->transitions->head=node_init(transition);
    }
    else{
        while(curr->next){
            if(is_transitions_equal(transition,curr->val)){
                free(transition);
                return;
            }
            curr=curr->next;
        }
        if(is_transitions_equal(transition,curr->val)){
            free(transition);
            return;
        }
        curr->next = node_init(transition);
    }
    a->states[state_from]->transitions_cnt++;
}


void NFA_remove_transition(NFA *a,int state_from,int state_to,int trigger){
    node *curr=(a->states[state_from])->transitions->head;
    node *tmp=NULL;
    while(curr){

        if(curr->val->state_from->index==state_from
        && curr->val->state_to->index==state_to
        && curr->val->transition_trigger==trigger
        ){
            if(tmp){
                tmp->next=curr->next;
                free(curr->val);
                free(curr);
            }
            else{
                (a->states[state_from])->transitions->head=curr->next;
                free(curr->val);
                free(curr);
            }
            a->states[state_from]->transitions_cnt--;
            return;
        }
        tmp=curr;
        curr=curr->next;
    }
}



int NFA_check(NFA *a,big_int *sentence,int verbose){

    big_int *sent2= big_int_copy(sentence);

    NFA_state *a_current_state=a->initial_state;

    if(verbose==1){
        printf("sentence: ");
        big_int_print(sent2);
    }

    if(0!=  ((sent2->bit_len)%a->dim) ){
        (sent2->bit_len) += a->dim- ((sent2->bit_len)%a->dim) ;
    }

    long max_num_words= (sent2->bit_len)/a->dim;
    long processed_words= 0;

    int curr_word=(sent2->number[0] & ( (1<<a->dim)-1 ) );
    big_int_bin_shft_r2(sent2,a->dim);

    while(processed_words<max_num_words) {
        node *curr_nd=a_current_state->transitions->head;
        if(verbose==1) {
            printf("------------------------\n");
            printf("not processed part of sentence:");
            big_int_print(sent2);
            printf("current word:");
            printf("%d\n",curr_word);
            printf("current state:%d\n",a_current_state->index);
        }
        while (curr_nd != NULL) {
            if (curr_word == curr_nd->val->transition_trigger) {
                if(verbose==1){
                    printf("transition from state_%d to state_%d by trigger ",
                           curr_nd->val->state_from->index,
                           curr_nd->val->state_to->index);
                    printf("%b\n",curr_nd->val->transition_trigger);
                    printf("current state:%d\n",curr_nd->val->state_to->index);

                }
                a_current_state = curr_nd->val->state_to;
                break;
            }
            curr_nd = curr_nd->next;
        }
        curr_word=(sent2->number[0] & ( (1<<a->dim)-1 ) );
        big_int_bin_shft_r2(sent2,a->dim);
        processed_words++;
    }
    big_int_free2(1,&sent2);
    return a_current_state->accept_state;
}


void NFA_print(NFA* a){
    printf("automata_states:\n\n");
    printf("--------------------------------------------\n");
    for(int i=0;i<a->states_cnt;i++){
        printf("state_ix=%d; is_acceptable=%d; transitions=%d\n",
               a->states[i]->index,a->states[i]->accept_state,
               a->states[i]->transitions_cnt
               );
        printf("   ------transitions-----\n");
        node* curr=a->states[i]->transitions->head;
        while(curr){
            printf("to=%d; trigger=",curr->val->state_to->index);
            printf("%b\n",curr->val->transition_trigger);
            curr=curr->next;
        }
        printf("--------------------------------------------\n\n");
    }
}


void NFA_free(NFA *a){
    for(int i=0;i<a->states_cnt;i++){
        NFA_state_free(a->states[i]);
    }
    free(a->states);
    free(a);

}


void NFA_to_dot(NFA *a){
    FILE *f=fopen("../fsm.gv", "w");

    char template[]=" digraph finite_state_machine {\n"
                    "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                   "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\trankdir=LR;\n"
                   "\tnode [shape = doublecircle ];";

    fprintf(f,template);

    for(int i=0;i<a->states_cnt;i++){
        if(a->states[i]->accept_state==1){
             fprintf(f," %d",i);
        }
    }

    fprintf(f,";\n\tnode [shape = circle];\n");


    for(int i=0;i<a->states_cnt;i++){
        node* curr=a->states[i]->transitions->head;

        while(curr){
            fprintf(f,"\t%d -> %d [label =\" ",
                    curr->val->state_from->index,
                    curr->val->state_to->index);

            fprintf(f,"%b" ,curr->val->transition_trigger);
            fprintf(f," \"]; \n");
            curr=curr->next;
        }

    }
    fprintf(f,"}   ");
    fclose(f);
    system("dot -Tpng ../fsm.gv -o ../automata1.png");
}


int * NFA_check_many(NFA *a, big_int **sentences, int len, int verbose){
    int *res=(int *)calloc( len,sizeof(int));
    for(int i=0;i<len;i++){
        if(verbose){
            printf("Sentence_%d\n",i);
            printf("////////////////////////////////////////////////\n");
        }
        res[i]= NFA_check(a,sentences[i],verbose);
        if(verbose){
            printf("\n\n");
        }
    }

    return res;
}

/*
dim
final_states_cnt Final states ix-s separated with spaces
transitions_cnt
trigger_value state_from_ix state_to_ix
 */
void NFA_to_file(NFA *a){
    FILE *f= fopen("../automata.txt","w");
    fprintf(f,"%d\n",a->dim);

    fprintf(f,"%d ",a->states_cnt);

    for(int j=0; j<a->states_cnt; j++){
        fprintf(f,"%d ",a->states[j]->accept_state);
    }
    fprintf(f,"\n");

    int cnt_tr=0;
    for(int j=0; j<a->states_cnt; j++){
        cnt_tr+=a->states[j]->transitions_cnt;
    }
    fprintf(f,"%d\n",cnt_tr);
    for(int j=0; j<a->states_cnt; j++){
        node *curr_nd=a->states[j]->transitions->head;
        while(curr_nd){
            fprintf(f,"%d %d %d\n",curr_nd->val->transition_trigger, curr_nd->val->state_from->index,curr_nd->val->state_to->index);
            curr_nd=curr_nd->next;
        }
    }


    fclose(f);
}


NFA *NFA_from_file(char* file_pth){
    FILE *f= fopen(file_pth,"r");

    int dim;
    fscanf(f,"%d\n",&dim);

    NFA *a= NFA_init(dim);

    int cnt_st;
    fscanf(f,"%d",&cnt_st);


    int final[cnt_st];

    for(int i=0;i<cnt_st;i++){
        fscanf(f,"%d",&final[i]);
    }

    for(int i=1;i<cnt_st;i++){
        NFA_add_state(a,final[i]);
    }

    int cnt_tr=0;
    fscanf(f,"%d\n",&cnt_tr);

    for(int i=0; i<cnt_tr; i++){
        int trigger,from,to;
        fscanf(f,"%d",&trigger);
        fscanf(f,"%d",&from);
        fscanf(f,"%d\n",&to);
        NFA_add_transition(a,from,to,trigger);
    }

    fclose(f);
    return a;
}










