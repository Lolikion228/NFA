//
// Created by lolikion on 16.02.24.
//
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "other/linked_list.h"

NFA *NFA_init(big_int *dim){
    NFA *a = (NFA *)malloc(sizeof(NFA));
    if(!a){
        printf("memory allocation error in NFA_init_1\n");
        exit(1);
    }
    a->states_cnt=1;
    a->dim= big_int_copy(dim);
    a->initial_state=NFA_state_init(a,0);
    a->initial_state->index=0;
    a->current_state=a->initial_state;
    a->states = malloc(sizeof(NFA_state));
    if(!a->states){
        printf("memory allocation error in NFA_init_2\n");
        exit(1);
    }
    a->states[0]=a->initial_state;
    return a;
}


void NFA_add_transition(NFA *a, int state_from, int state_to, big_int *trigger){

    NFA_transition *transition= (NFA_transition *)malloc(sizeof(NFA_transition));
    if(!transition){
        printf("memory allocation error in NFA_add_transition\n");
        exit(1);
    }
    transition->state_from=a->states[state_from];
    transition->state_to=a->states[state_to];
    transition->transition_trigger= big_int_copy(trigger);
    node *curr=(a->states[state_from])->transitions->head;

    if(!curr){
        (a->states[state_from])->transitions->head=node_init(transition);
    }
    else{
        while(curr->next){
            if(is_transitions_equal(transition,curr->val)){
                NFA_transition_free(transition);
                return;
            }
            curr=curr->next;
        }
        if(is_transitions_equal(transition,curr->val)){
            NFA_transition_free(transition);
            return;
        }
        curr->next = node_init(transition);
    }
    a->states[state_from]->transitions_cnt++;
}


void NFA_remove_transition(NFA *a,int state_from,int state_to,big_int* trigger){
    node *curr=(a->states[state_from])->transitions->head;
    node *tmp=NULL;
    while(curr){

        if(curr->val->state_from->index==state_from
        && curr->val->state_to->index==state_to
        && big_int_equal(curr->val->transition_trigger,trigger)
        ){
            if(tmp){
                tmp->next=curr->next;
                NFA_transition_free(curr->val);
                free(curr);
            }
            else{
                (a->states[state_from])->transitions->head=curr->next;
                NFA_transition_free(curr->val);
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
    int accepted=0;
    big_int *sent2= big_int_copy(sentence);

    if(verbose==1){
        printf("sentence: ");
        big_int_print(sent2);
    }

    long max_num_words= (sent2->bit_len)/a->dim->number[0];
    long processed_words= 0;

    big_int *curr_word= big_int_get("0");
    big_int *two=big_int_get("10");
    big_int *dvdr= big_int_lr3(two,a->dim);

    big_int_div2_for_pow(sent2,dvdr,curr_word);
    big_int_bin_shft_r2(sent2,a->dim->number[0]);


    while(processed_words<max_num_words) {
        node *curr_nd=a->current_state->transitions->head;
        if(verbose==1) {
            printf("------------------------\n");
            printf("not processed part of sentence:");
            big_int_print(sent2);
            printf("current word:");
            big_int_print(curr_word);
            printf("current state:%d\n",a->current_state->index);
        }
        while (curr_nd != NULL) {
            if (big_int_equal(curr_word, curr_nd->val->transition_trigger)) {
                if(verbose==1){
                    printf("transition from state_%d to state_%d by trigger ",
                           curr_nd->val->state_from->index,
                           curr_nd->val->state_to->index);
                    big_int_print(curr_nd->val->transition_trigger);
                    printf("current state:%d\n",curr_nd->val->state_to->index);
                }
                a->current_state = curr_nd->val->state_to;
                break;
            }
            curr_nd = curr_nd->next;
        }

        big_int_div2_for_pow(sent2,dvdr,curr_word);
        big_int_bin_shft_r2(sent2,a->dim->number[0]);
        processed_words++;


    }

    big_int_free2(4,&sent2,&curr_word,&two,&dvdr);

    accepted=a->current_state->accept_state;
    a->current_state=a->initial_state;
    return accepted;
}




NFA_state *NFA_state_init(NFA *a,int accept_state){
    NFA_state *state = (NFA_state *)malloc(sizeof(NFA_state));
    if(!state){
        printf("memory allocation error in state_init1\n");
        exit(1);
    }
    state->index=a->states_cnt;
    state->accept_state=accept_state;
    state->transitions=malloc(sizeof(struct list));
    if(!state->transitions){
        printf("memory allocation error in state_init2\n");
        exit(1);
    }
    state->transitions->head = NULL;
    state->transitions_cnt=0;
    return state;
}


int is_transitions_equal(NFA_transition *tr1,NFA_transition *tr2){
    if(!tr1 || !tr2){return 0;}
    return ( (tr1->state_to->index==tr2->state_to->index) && (tr1->state_from->index==tr2->state_from->index) && (big_int_equal(tr1->transition_trigger,tr2->transition_trigger))  );
}


void NFA_add_state(NFA *a,NFA_state *state){
    a->states=realloc(a->states,((a->states_cnt)+1)*sizeof(NFA_state));
    if(!a->states){
        printf("memory allocation error in NFA_add_state\n");
        exit(1);
    }
    a->states_cnt++;
    a->states[a->states_cnt-1]=state;
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
            big_int_print(curr->val->transition_trigger);
            curr=curr->next;
        }
        printf("--------------------------------------------\n\n");
    }
}


///maybe set to NULL?
void NFA_free(NFA *a){
    for(int i=0;i<a->states_cnt;i++){
        NFA_state_free(a->states[i]);
    }
    free(a->states);
    big_int_free(&a->dim);
    free(a);

}


///maybe set to NULL?
void NFA_state_free(NFA_state *s){
    node_free(s->transitions->head);
    free(s->transitions);
    free(s);
}

void NFA_transition_free(NFA_transition *tr){
    big_int_free(&tr->transition_trigger);
    free(tr);
}




void NFA_to_dot(NFA *a){
    FILE *f=fopen("../fsm.gv", "w");

    char template[]=" digraph finite_state_machine {\n"
                    "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                   "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\trankdir=LR;\n"
                   "\tnode [shape = doublecircle];";




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
            big_int_fprint(curr->val->transition_trigger,f);
            fprintf(f," \"]; \n");
            curr=curr->next;
        }

    }
    fprintf(f,"}   ");
    fclose(f);
    system("dot -Tpng ../fsm.gv -o ../automata1.png");
}























