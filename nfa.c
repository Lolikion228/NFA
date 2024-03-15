//
// Created by lolikion on 16.02.24.
//
#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "other/linked_list.h"

NFA_state *NFA_state_init(NFA *a,int is_final);

// NFA over \{0,1\}^{dim}
NFA *NFA_init(int dim){
    NFA *a = (NFA *)malloc(sizeof(NFA));
    if(!a){
        printf("memory allocation error in NFA_init_1\n");
        exit(1);
    }
    a->states_cnt=0;
    a->dim = dim;
    a->initial_state=NULL;
    a->states = NULL;
    return a;
}


NFA_state *NFA_state_init(NFA *a,int is_final){
    NFA_state *state = (NFA_state *)malloc(sizeof(NFA_state));
    if(!state){
        printf("memory allocation error in state_init1\n");
        exit(1);
    }
    state->index=a->states_cnt;
    state->is_final=is_final;
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
    if(a->states_cnt==1){
        a->initial_state=a->states[0];
    }
}


void NFA_state_free(NFA_state *s){
    node_free(s->transitions->head);
    free(s->transitions);
    free(s);
}


int NFA_is_transitions_equal(NFA_transition *tr1, NFA_transition *tr2){
    if(!tr1 || !tr2){return 0;}
    return (
            (tr1->state_to->index==tr2->state_to->index) &&
            (tr1->state_from->index==tr2->state_from->index) &&
            (tr1->transition_trigger==tr2->transition_trigger)
            );
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
            if(NFA_is_transitions_equal(transition, curr->val)){
                free(transition);
                return;
            }
            curr=curr->next;
        }
        if(NFA_is_transitions_equal(transition, curr->val)){
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

void print_array(int a[],int len){
    printf("[");
    for(int i=0;i<len;i++){
        printf(" %d",a[i]);
    }
    printf(" ]\n");
}

int NFA_check(const NFA *a,big_int *sentence){

    big_int *sent2= big_int_copy(sentence);
    int is_transition=0;

    int curr_states[a->states_cnt];
    for(int i=0;i<a->states_cnt;i++){
        curr_states[i]=0;
    }
    curr_states[0]=1;


    if( ((sent2->bit_len)%a->dim) ){
        big_int_print(sentence);
        printf("bit_len is not multiple of a->dim\n");
        exit(1);
    }

    long max_num_words= (sent2->bit_len)/a->dim;
    long processed_words= 0;

    int curr_wrd=0;
    for(int i=0; i <= (a->dim >> 3)  - ( (a->dim & 7) == 0 ); i++ ){
        curr_wrd+= (   (sent2->number[i] << 8*i) & ( ((1<<a->dim)-1) ) ) ;
    }

    big_int_bin_shft_r2(sent2,a->dim);

    while(1) {
        is_transition=0;
        int curr_states2[a->states_cnt];
        for(int i=0;i<a->states_cnt;i++){
            curr_states2[i]=0;
        }
        for(int i=0;i<a->states_cnt;i++) {
            if(curr_states[i] == 1) {
                node *curr_tr = a->states[i]->transitions->head;
                while (curr_tr != NULL) {
                    if (  curr_tr->val->transition_trigger == -1 ) {
                        curr_states[curr_tr->val->state_to->index]=1;
                    }
                    curr_tr = curr_tr->next;
                }
                curr_tr = a->states[i]->transitions->head;
                while (curr_tr != NULL) {
                    if (  curr_wrd == curr_tr->val->transition_trigger  ) {
                        curr_states2[curr_tr->val->state_to->index]=1;
                        is_transition=1;
                    }
                    curr_tr = curr_tr->next;
                }
            }
        }

        if(is_transition) {
            for (int i = 0; i < a->states_cnt; i++) {
                curr_states[i] = curr_states2[i];
            }
        }

        processed_words++;
        curr_wrd=0;
        if(processed_words==max_num_words){break;}
        for(int i=0; i <= (a->dim >> 3)  - ( (a->dim & 7) == 0 ); i++ ){
            curr_wrd+= (  ( ((1<<a->dim)-1) ) & (sent2->number[i]<< 8*i)   );
        }
        big_int_bin_shft_r2(sent2,a->dim);
    }

    big_int_free2(1,&sent2);
    for(int i=0;i<a->states_cnt;i++){
        if(curr_states[i] & a->states[i]->is_final){return 1;}
    }
    return 0;
}


void NFA_print(const NFA* a){
    printf("automata_states:\n\n");
    printf("--------------------------------------------\n");
    for(int i=0;i<a->states_cnt;i++){
        printf("state_ix=%d; is_acceptable=%d; transitions=%d\n",
               a->states[i]->index,a->states[i]->is_final,
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


void NFA_to_pic(const NFA *a){
    FILE *f=fopen("../fsm.gv", "w");

    char template[]=" digraph finite_state_machine {\n"
                    "\tfontname=\"Helvetica,Arial,sans-serif\"\n"
                   "\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                   "\trankdir=LR;\n"
                   "\tnode [shape = doublecircle ];";

    fprintf(f,template);

    for(int i=0;i<a->states_cnt;i++){
        if(a->states[i]->is_final == 1){
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
            if(curr->val->transition_trigger!=-1){
                fprintf(f,"%b" ,curr->val->transition_trigger);
            }
            else{
                fprintf(f,"Eps");
            }
            fprintf(f," \"]; \n");
            curr=curr->next;
        }

    }

    fprintf(f,"}   ");
    fclose(f);
    system("dot -Tpng ../fsm.gv -o ../temp/automata.png");
    system("rm ../fsm.gv");
}


int * NFA_check_many(const NFA *a, big_int **sentences, int len){
    int *res=(int *)calloc( len,sizeof(int));
    for(int i=0;i<len;i++){
        res[i]= NFA_check(a,sentences[i]);
    }
    return res;
}


void NFA_to_file(const NFA *a){
    FILE *f= fopen("../temp/automata.txt","w");
    fprintf(f,"%d\n",a->dim);
    fprintf(f,"%d ",a->states_cnt);

    for(int j=0; j<a->states_cnt; j++){
        fprintf(f,"%d ",a->states[j]->is_final);
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
    int dim,cnt_st,cnt_tr;

    fscanf(f,"%d\n",&dim);
    fscanf(f,"%d",&cnt_st);

    int final[cnt_st];
    for(int i=0;i<cnt_st;i++){

        fscanf(f,"%d",&final[i]);
    }

    fscanf(f,"%d\n",&cnt_tr);

    NFA *a= NFA_init(dim);

    for(int i=0;i<cnt_st;i++){
        NFA_add_state(a,final[i]);
    }
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


NFA *NFA_union(const NFA *a1,const NFA *a2){

    NFA *res= NFA_init(a1->dim);
    NFA_add_state(res,0);

    for(int i=0; i<a1->states_cnt; i++){
        NFA_add_state(res,a1->states[i]->is_final);
    }
    for(int i=0; i<a1->states_cnt; i++){
        node* curr_tr=a1->states[i]->transitions->head;
        while(curr_tr){
            NFA_add_transition(res,
                               curr_tr->val->state_from->index+1,
                               curr_tr->val->state_to->index+1,
                               curr_tr->val->transition_trigger);
            curr_tr=curr_tr->next;
        }
    }

    for(int i=0; i<a2->states_cnt; i++){
        NFA_add_state(res,a2->states[i]->is_final);
    }
    for(int i=0; i<a2->states_cnt; i++){
        node* curr_tr=a2->states[i]->transitions->head;
        while(curr_tr){
            NFA_add_transition(res,
                               curr_tr->val->state_from->index+a1->states_cnt+1,
                               curr_tr->val->state_to->index+a1->states_cnt+1,
                               curr_tr->val->transition_trigger);
            curr_tr=curr_tr->next;
        }
    }

    NFA_add_transition(res,0,1,-1);
    NFA_add_transition(res,0,a1->states_cnt+1,-1);
    return res;
}



NFA *NFA_intersection(const NFA *a1,const NFA *a2) {

    NFA *res = NFA_init(a1->dim);

    for(int i=0;i<a1->states_cnt;i++){
        for(int j=0;j<a2->states_cnt;j++){
            NFA_add_state(res,a1->states[i]->is_final & a2->states[j]->is_final);
        }
    }

    res->initial_state=res->states[0];
    for(int i=0;i<a1->states_cnt;i++){
        for(int j=0;j<a2->states_cnt;j++) {
            node *curr_tr1=a1->states[i]->transitions->head;
            node *curr_tr2=a2->states[j]->transitions->head;
            while(curr_tr1){
                while(curr_tr2){
                    if(curr_tr1->val->transition_trigger==curr_tr2->val->transition_trigger)
                    {
                        NFA_add_transition(res,
                                           a2->states_cnt*curr_tr1->val->state_from->index \
                                           + curr_tr2->val->state_from->index,
                                           a2->states_cnt*curr_tr1->val->state_to->index \
                                           + curr_tr2->val->state_to->index, \
                                           curr_tr1->val->transition_trigger);
                    }
                    if(curr_tr1->val->transition_trigger==-1){
                        NFA_add_transition(res,
                                           a2->states_cnt*curr_tr1->val->state_from->index \
                                           + curr_tr2->val->state_from->index,
                                           a2->states_cnt*curr_tr1->val->state_to->index \
                                           + curr_tr2->val->state_from->index, \
                                           -1);
                    }
                    if(curr_tr2->val->transition_trigger==-1){
                        NFA_add_transition(res,
                                           a2->states_cnt*curr_tr1->val->state_from->index \
                                           + curr_tr2->val->state_from->index,
                                           a2->states_cnt*curr_tr1->val->state_from->index \
                                           + curr_tr2->val->state_to->index, \
                                           -1);
                    }
                    curr_tr2=curr_tr2->next;
                }
                curr_tr2=a2->states[j]->transitions->head;
                curr_tr1=curr_tr1->next;
            }
        }
    }
    return res;
}


NFA* NFA_complement(const NFA *a){
    if(!NFA_is_dfa(a)){printf("complement only for DFA for now.\n");exit(1);}
    NFA *res = NFA_copy(a);
    for(int i=0;i<res->states_cnt;i++){
        res->states[i]->is_final=!res->states[i]->is_final;
    }
    return res;
}


int project_int(int n,int num_cord){
    int n1=n&((1<<num_cord)-1);
    int n2=n>>(num_cord+1);
    return n1+(n2<<num_cord);
}

int extend_int(int n,int num_cord,int bit){
    int n1=n&((1<<num_cord)-1);
    int n2=n>>(num_cord);
    return n1+(n2<<(num_cord+1))+(bit<<num_cord);
}


NFA *NFA_project(const NFA *a, int num_cord){
    if(num_cord >= a->dim){printf("num_cord >= dim.\n");exit(1);}
    NFA *res= NFA_copy(a);
    res->dim--;
    for(int i=0;i<res->states_cnt;i++){
        node *curr_tr=res->states[i]->transitions->head;
        while(curr_tr){
            curr_tr->val->transition_trigger= project_int(curr_tr->val->transition_trigger,num_cord);
            curr_tr=curr_tr->next;
        }
    }
    return res;
}


NFA *NFA_extend(const NFA *a, int num_cord){
    if(num_cord >= a->dim){printf("num_cord >= dim.\n");exit(1);}
    NFA *res= NFA_init(a->dim +1);
    for(int i=0;i<a->states_cnt;i++){
        NFA_add_state(res,a->states[i]->is_final);
    }

    for(int i=0;i<a->states_cnt;i++){
        node *curr_tr=a->states[i]->transitions->head;
        int cnt1=a->states[i]->transitions_cnt;
        for(int j=0;j<cnt1;j++){
            int old_tr=curr_tr->val->transition_trigger;
            NFA_add_transition(res,curr_tr->val->state_from->index,
                               curr_tr->val->state_to->index,
                               extend_int(old_tr,num_cord,1));
            NFA_add_transition(res,curr_tr->val->state_from->index,
                               curr_tr->val->state_to->index,
                               extend_int(old_tr,num_cord,0));
            curr_tr=curr_tr->next;
        }
    }

    return res;
}


NFA *NFA_copy(const NFA *a){
    NFA *res= NFA_init(a->dim);
    for(int i=0;i<a->states_cnt;i++){
        NFA_add_state(res,a->states[i]->is_final);
    }
    for(int i=0;i<a->states_cnt;i++){
        node *curr_tr=a->states[i]->transitions->head;
        while(curr_tr){
            NFA_add_transition(res,curr_tr->val->state_from->index,
                               curr_tr->val->state_to->index,
                               curr_tr->val->transition_trigger);
            curr_tr=curr_tr->next;
        }
    }
    return res;
}


int NFA_is_dfa(const NFA *a){
    for(int i=0;i<a->states_cnt;i++){
        node *curr_tr1=a->states[i]->transitions->head;
        while(curr_tr1){
            node *curr_tr2=curr_tr1->next;
            while(curr_tr2){
                if(curr_tr1->val->transition_trigger==curr_tr2->val->transition_trigger ||
                curr_tr1->val->transition_trigger==-1 || curr_tr2->val->transition_trigger==-1 ){
                    return 0;
                }
                curr_tr2=curr_tr2->next;
            }
            curr_tr1=curr_tr1->next;
        }
    }
    return 1;
}


NFA *NFA_zero_star_closure(NFA *a){
    NFA *res= NFA_copy(a);

//    ??????
//    for(int i=0; i<res->states_cnt; i++){
//        if(res->states[i]->is_final){
//            NFA_add_transition(res,res->states[i]->index,res->states[i]->index,0);
//        }
//    }

    return res;
}

