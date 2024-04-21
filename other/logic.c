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
        case 8:
            printf("div8(x) ");
            break;
        case 9:
            printf("div4(x) ");
            break;
        default:
            printf("wrong op_id\n");
            exit(1);
            break;
    }
}


void parser_helper(Operator op, Stack2 *a_stack){
    if(op.id==2){
        NFA *tmp1 = Stack2_pop(a_stack);
        NFA *tmp2 = Stack2_pop(a_stack);
        Stack2_push(a_stack, NFA_union(tmp1,tmp2));
        NFA_free(tmp1);
        NFA_free(tmp2);
    }
    if(op.id==3){
        NFA *tmp1 = Stack2_pop(a_stack);
        NFA *tmp2 = Stack2_pop(a_stack);
        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
        NFA_free(tmp1);
        NFA_free(tmp2);
    }
    if(op.id==4){
        NFA *tmp1 = Stack2_pop(a_stack);
        Stack2_push(a_stack, NFA_complement(tmp1));
        NFA_free(tmp1);
    }
}


char *get_name_parser(char *cmd){

    char *end = cmd;
    while( (*end) != '('){
        ++end;
    }

    char *name = malloc(sizeof(char)*(end-cmd));
    memcpy(name,cmd+1,end-cmd-1);
    name[end-cmd-1]='\0';

    return name;
}




NFA *Parser(char *formula, char **automata_names, NFA **automata, int automata_cnt ){
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

            case '&':
                op_id = 3;
                break;

            case '!':
                op_id = 4;
                break;

            case '$':
                char *name = get_name_parser(i);
                int valid_id = 0;
                if(strstr(i+1,"div")==i+1){
                    valid_id=1;
                    int num = (int) strtol(name+3,NULL,10);
                    Stack2_push(a_stack, NFA_div_n(num));
//                    printf("num=%d\n",num);
                }
                if(strstr(i+1,"is_zero")==i+1){
                    valid_id=1;
                    Stack2_push(a_stack, NFA_from_file("../automatons/lsd/zeros.txt"));
                }




                if(!valid_id){
                    if(automata_cnt!=0){
                        for(int j=0; j<automata_cnt; ++j){
                            if(!strcmp(name,automata_names[j])){
                                NFA *tmp = NFA_copy(automata[j]);
                                Stack2_push(a_stack, tmp);
                                valid_id=1;
                            }
                        }
                    }
                }
                if(!valid_id){
                    printf("invalid automaton name\n");
                    printf("%s\n",i);
                    exit(1);
                }
                free(name);


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

    NFA *res = Stack2_pop(a_stack);
    Stack_free(op_stack);
    Stack2_free(a_stack);
    return res;
}


void RPN_print(char *formula){
    Stack *op_stack = Stack_init();
    Operator op,curr;

    for(int i = 0; i < strlen(formula); ++i){
        int op_id = -1;
        switch( formula[i] ){

            case '(':
                Stack_push(op_stack,op_init(0));
                break;

            case ')':
                curr = Stack_pop(op_stack);
                while( curr.id != 0 ){
                    op_print(curr.id);
                    curr = Stack_pop(op_stack);
                }
                break;

            case '|':
                op_id = 2;
                break;

            case '&':
                op_id = 3;
                break;

            case '!':
                op_id = 4;
                break;

            case '$':
                if(formula[i+4] == '2'){
                    op_print(5);
                }
                if(formula[i+4] == '3'){
                    op_print(6);
                }
                if(formula[i+4] == '8'){
                    op_print(8);
                }
                if(formula[i+4] == '4'){
                    op_print(9);
                }
                if(formula[i+4] == 'z'){
                    op_print(7);
                }

                break;

            default:
                break;
        }
        if(op_id != -1){
            op = op_init(op_id);
            while( (op_stack->size != 0) && ( Stack_top(op_stack).priority > op.priority ) ){
                op_print(Stack_pop(op_stack).id);
            }
            Stack_push(op_stack,op);
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
