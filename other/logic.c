//
// Created by lolikion on 01.04.24.
//

#include "logic.h"
#include "stdlib.h"
#include "stdio.h"
#include "nfa.h"
#include "other/Stack.h"
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
//NFA *parser(char *formula){
//    Stack *op_stack = Stack_init(0);
//    Stack *a_stack = Stack_init(1);
//
//
//
//    return a_stack->data[0];
//}

void parser(char *formula){
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
}