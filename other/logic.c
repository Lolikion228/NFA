//
// Created by lolikion on 01.04.24.
//


#include "logic.h"
#include "stdlib.h"
#include "stdio.h"
#include "nfa.h"
#include "other/Stack.h"
#include "other/Stack2.h"
#include "string.h"


/*
 * id:  0  1  2  3  4   5     6     7
 * op:  (  )  |  &  !   div2  div3  is_zero
 * */

Operator op_init(int id){
    Operator res;
    switch (id) {
        case 0:
            res.id = 0;
            res.priority = -1;
            break;
        case 1:
            res.id = 1;
            res.priority = -1;
            break;
        case 2:
            res.id = 2;
            res.priority = 2;
            break;
        case 3:
            res.id = 3;
            res.priority = 3;
            break;
        case 4:
            res.id = 4;
            res.priority = 4;
            break;
        case 5:
            res.id = 5;
            res.priority = -1;
            break;
        case 6:
            res.id = 6;
            res.priority = -1;
            break;
        case 7:
            res.id = 7;
            res.priority = -1;
            break;
        default:
            printf("wrong op_id\n");
            exit(1);
            break;
    }
    return res;
}


void op_print(int id){
    switch (id) {
        case 0:
            printf("( ");
            break;
        case 1:
            printf(") ");
            break;
        case 2:
            printf("| ");
            break;
        case 3:
            printf("& ");
            break;
        case 4:
            printf("! ");
            break;
        case 5:
            printf("div2(x) ");
            break;
        case 6:
            printf("div3(x) ");
            break;
        case 7:
            printf("is_zero(x) ");
            break;
        default:
            printf("wrong op_id\n");
            exit(1);
            break;
    }
}


NFA *Parser(char *formula){
    Stack *op_stack = Stack_init();
    Stack2 *a_stack = Stack2_init();
    Operator op;
    Operator curr;
    Operator curr2;
    for(int i = 0; i < strlen(formula); ++i){
        switch( formula[i] ){

            case '(':
                op = op_init(0);
                Stack_push(op_stack,op);
                break;

            case ')':
                curr = op_stack->data[op_stack->size-1];
                while( curr.id != 0 ){
                    curr2 = Stack_pop(op_stack);
                    if(curr2.id==2){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_union(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==3){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==4){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_complement(tmp1));
                        NFA_free(tmp1);
                    }
                    curr = op_stack->data[op_stack->size-1];
                }
//                op_print(curr.id);
                Stack_pop(op_stack);
//                if(op_stack->size != 0){
//                    curr = op_stack->data[op_stack->size-1];
//                    if( (curr.id == 5) || (curr.id == 6) || (curr.id == 7) ){
//                        op_print(curr.id);
//                        Stack_pop(op_stack);
//                    }
//                }
//                op_print(op.id);
                break;

            case '|':
                op = op_init(2);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
//                    op_print(curr.id);
                    curr2 = Stack_pop(op_stack);
                    if(curr2.id==2){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_union(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==3){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==4){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_complement(tmp1));
                        NFA_free(tmp1);

                    }
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '&':
                op = op_init(3);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
//                    op_print(curr.id);
                    curr2 = Stack_pop(op_stack);
                    if(curr2.id==2){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_union(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==3){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==4){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_complement(tmp1));
                        NFA_free(tmp1);

                    }
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '!':
                op = op_init(4);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
//                    op_print(curr.id);
                    curr2 = Stack_pop(op_stack);
                    if(curr2.id==2){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_union(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==3){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        NFA *tmp2 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
                        NFA_free(tmp1);
                        NFA_free(tmp2);
                    }
                    if(curr2.id==4){
                        NFA *tmp1 = Stack2_pop(a_stack);
                        Stack2_push(a_stack, NFA_complement(tmp1));
                        NFA_free(tmp1);

                    }
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '$':
                if(formula[i+4]=='2'){
                    Stack2_push(a_stack, NFA_from_file("../automatons/lsd/2|x.txt"));
                }
                if(formula[i+4]=='3'){
                    Stack2_push(a_stack, NFA_from_file("../automatons/lsd/3|x.txt"));
                }
                if(formula[i+4]=='z'){
                    Stack2_push(a_stack, NFA_from_file("../automatons/lsd/zeros.txt"));
                }
                break;

            default:
                break;
        }
    }

    while(op_stack->size != 0){
        curr2 = Stack_pop(op_stack);
        if(curr2.id==2){
            NFA *tmp1 = Stack2_pop(a_stack);
            NFA *tmp2 = Stack2_pop(a_stack);
            Stack2_push(a_stack, NFA_union(tmp1,tmp2));
            NFA_free(tmp1);
            NFA_free(tmp2);
        }
        if(curr2.id==3){
            NFA *tmp1 = Stack2_pop(a_stack);
            NFA *tmp2 = Stack2_pop(a_stack);
            Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
            NFA_free(tmp1);
            NFA_free(tmp2);
        }
        if(curr2.id==4){
            NFA *tmp1 = Stack2_pop(a_stack);
            Stack2_push(a_stack, NFA_complement(tmp1));
            NFA_free(tmp1);
        }
//        op_print(curr.id);
    }

    Stack_free(op_stack);
    NFA *res = a_stack->data[0];
    Stack2_free(a_stack);
    return res;
}


void RPN_print(char *formula){
    Stack *op_stack = Stack_init();
//    Stack *a_stack = Stack_init(1);
    Operator op;
    Operator curr;
    for(int i = 0; i < strlen(formula); ++i){
        switch( formula[i] ){

            case '(':
                op = op_init(0);
                Stack_push(op_stack,op);
                break;

            case ')':
                op = op_init(1);
                curr = op_stack->data[op_stack->size-1];
                while( curr.id != 0 ){
                    op_print(curr.id);
                    Stack_pop(op_stack);
                    curr = op_stack->data[op_stack->size-1];
                }
//                op_print(curr.id);
                Stack_pop(op_stack);
                if(op_stack->size != 0){
                    curr = op_stack->data[op_stack->size-1];
                    if( (curr.id == 5) || (curr.id == 6) || (curr.id == 7) ){
                        op_print(curr.id);
                        Stack_pop(op_stack);
                    }
                }
//                op_print(op.id);
                break;

            case '|':
                op = op_init(2);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
                    op_print(curr.id);
                    Stack_pop(op_stack);
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '&':
                op = op_init(3);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
                    op_print(curr.id);
                    Stack_pop(op_stack);
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '!':
                op = op_init(4);
                if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                while( (op_stack->size != 0) && ( curr.priority != -1 ) && ( curr.priority > op.priority ) ){
                    op_print(curr.id);
                    Stack_pop(op_stack);
                    if(op_stack->size!=0){curr = op_stack->data[op_stack->size-1];}
                }
                Stack_push(op_stack,op);
                break;

            case '$':
                if(formula[i+4]=='2'){
                    op = op_init(5);
                    op_print(op.id);
                }
                if(formula[i+4]=='3'){
                    op = op_init(6);
                    op_print(op.id);
                }
                if(formula[i+4]=='z'){
                    op = op_init(7);
                    op_print(op.id);
                }
                break;

            default:
                break;
        }
    }

    while(op_stack->size != 0){
        curr = Stack_pop(op_stack);
        op_print(curr.id);
    }

    Stack_free(op_stack);
    printf("\n");
}

/*
 * while there are tokens to be read:
    read a token
    if the token is:
    - a function:
        push it onto the operator stack
    - an operator o1:
        while (
            there is an operator o2 at the top of the operator stack which is not a left parenthesis,
            and o2 has greater precedence than o1
        ):
            pop o2 from the operator stack into the output queue
        push o1 onto the operator stack
    - a left parenthesis (i.e. "("):
        push it onto the operator stack
    - a right parenthesis (i.e. ")"):
        while the operator at the top of the operator stack is not a left parenthesis:
            pop the operator from the operator stack into the output queue
        pop the left parenthesis from the operator stack and discard it
        if there is a function token at the top of the operator stack, then:
            pop the function from the operator stack into the output queue
while there are tokens on the operator stack:
    pop the operator from the operator stack onto the output queue
 * */