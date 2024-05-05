//
// Created by lolikion on 05.05.24.
//

#include "reg.h"
#include "nfa.h"
#include "logic.h"
#include "Stack.h"
#include "Stack2.h"
#include "string.h"
#include "stdlib.h"



// | * .
NFA *RegEx_to_NFA(char *formula){

    Stack *op_stack = Stack_init();
    Stack2 *a_stack = Stack2_init();
    Operator op, curr;

    for(char *i = formula; *i != '\0'; ++i){

        int op_id = -1;
        switch(*i){

            case '(':
                Stack_push(op_stack,op_init(0));
                break;

            case ')':
                curr = Stack_pop(op_stack);
                while( curr.id != 0 ){
                    parser_helper(curr,a_stack);
                    curr = Stack_pop(op_stack);
                }
                break;

            case '|':
                op_id = 2;
                break;

            case '0':
            case '1':
                int num = (int)strtol(i,NULL,2);
//                printf("")
                Stack2_push(a_stack, NFA_const(num));
                while( (*i=='0' || *i=='1') && *i!='\0' )
                    ++i;
                --i;
                break;

            case '.':
                op_id=12;
                break;

            case '*':
                op_id=11;
                break;

            default:
                break;
        }

        if(op_id != -1){
            op = op_init(op_id);
            while( (op_stack->size != 0) && ( Stack_top(op_stack).priority > op.priority ) ){
                curr = Stack_pop(op_stack);
                parser_helper(curr,a_stack);
            }
            Stack_push(op_stack,op);
        }

    }

    while(op_stack->size != 0){
        curr = Stack_pop(op_stack);
        parser_helper(curr,a_stack);
    }
    NFA *res=NULL;
    if(a_stack->size!=0){
        res = Stack2_pop(a_stack);
    }
    Stack_free(op_stack);
    Stack2_free(a_stack);

    return res;
}


NFA* NFA_CL_star(NFA *a){
    //create new starting and final and eps from him to old starting and eps tr to him from final
    NFA *res = NFA_copy(a);
    NFA_add_state(res,1,1);
    NFA_add_state(res,1,0);
    NFA_add_transition(res,a->states_cnt+1,a->states_cnt+1,0);
    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_starting){
            NFA_add_transition(res,a->states_cnt,i,-1);
            res->states[i]->is_starting=0;
        }
    }

    for(int i=0; i<a->states_cnt; ++i){
        if(a->states[i]->is_final){
            NFA_remove_transition(res,i,i,0);
            NFA_add_transition(res,i,a->states_cnt+1,-1);
            NFA_add_transition(res,i,a->states_cnt,-1);
        }
    }


    return res;
}

NFA *NFA_concat(NFA *a1, NFA *a2){
    //connect every final state of a1 to all a2 starting states with eps tr
    NFA *res = NFA_init(a1->dim);

    for(int i=0; i<a1->states_cnt; ++i){
        NFA_add_state(res, 0,a1->states[i]->is_starting);
    }
    for(int i=0; i<a1->states_cnt; ++i){
        NFA_transition* curr_tr = a1->states[i]->transitions;
        while(curr_tr){
            NFA_add_transition(res,
                                a1->states[i]->index,
                                curr_tr->state_to_ix,
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
                                a1->states_cnt + a2->states[i]->index,
                                a1->states_cnt + curr_tr->state_to_ix,
                               curr_tr->transition_trigger);
            curr_tr = curr_tr->next;
        }
    }

    for(int i=0; i<a1->states_cnt; ++i){
        if(a1->states[i]->is_final){
            NFA_remove_transition(res,i,i,0);
            NFA_remove_transition(res,i,i,1);
            for(int j=0; j<a2->states_cnt; ++j){
                if(a2->states[j]->is_starting){
                    NFA_add_transition(res,
                                       a1->states[i]->index,
                                       a1->states_cnt + j,
                                       -1);
                }
            }
        }
    }



    return res;
}