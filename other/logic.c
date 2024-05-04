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
#include "a_dict.h"

/*
 * id:  0  1  2  3  4   5     6     7        8  9
 * op:  (  )  |  &  !   div2  div3  is_zero  E  A
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
        case 8:
            res.id = 8;
            res.priority = 100;
            res.pr_x=0;
            res.pr_y=0;
            res.pr_z=0;
            break;
        case 9:
            res.id = 9;
            res.priority = 100;
            res.pr_x=0;
            res.pr_y=0;
            res.pr_z=0;
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

void projection_helper(int pr_x, int pr_y, int pr_z, NFA **tmp){
    if(pr_x){
        NFA *tmp1 = NFA_project(*tmp,0);
        NFA_free(*tmp);
        *tmp = tmp1;
    }

    if(pr_y){
        if(pr_x){
            NFA *tmp2 = NFA_project(*tmp,0);
            NFA_free(*tmp);
            *tmp = tmp2;
        }
        else{
            NFA *tmp2 = NFA_project(*tmp,1);
            NFA_free(*tmp);
            *tmp = tmp2;
        }
    }

    if(pr_z){

        if(pr_y){
            if(pr_x){
                NFA *tmp2 = NFA_project(*tmp,0);
                NFA_free(*tmp);
                *tmp = tmp2;
            }
            else{
                NFA *tmp2 = NFA_project(*tmp,1);
                NFA_free(*tmp);
                *tmp = tmp2;
            }
        }
        else{
            if(pr_x){
                NFA *tmp2 = NFA_project(*tmp,1);
                NFA_free(*tmp);
                *tmp = tmp2;
            }
            else{
                NFA *tmp2 = NFA_project(*tmp,2);
                NFA_free(*tmp);
                *tmp = tmp2;
            }
        }
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
    else if(op.id==3){
        NFA *tmp1 = Stack2_pop(a_stack);
        NFA *tmp2 = Stack2_pop(a_stack);
        Stack2_push(a_stack, NFA_intersection(tmp1,tmp2));
        NFA_free(tmp1);
        NFA_free(tmp2);
    }
    else if(op.id==4){
        NFA *tmp1 = Stack2_pop(a_stack);
        Stack2_push(a_stack, NFA_complement(tmp1));
        NFA_free(tmp1);
    }
    else if(op.id==8){
        NFA *tmp1 = Stack2_pop(a_stack);
        projection_helper(op.pr_x,op.pr_y,op.pr_z,&tmp1);
        Stack2_push(a_stack, tmp1);
    }
    else if(op.id==9){
        NFA *tmp1 = Stack2_pop(a_stack);
        NFA *tmp2 = NFA_complement(tmp1);
        NFA_free(tmp1);
        projection_helper(op.pr_x,op.pr_y,op.pr_z,&tmp2);
        tmp1 = NFA_complement(tmp2);
        NFA_free(tmp2);
        Stack2_push(a_stack, tmp1);
    }

}


char *get_name_parser(char *cmd, char **i){

    char *end = cmd;
    while( (*end) != '('){
        ++end;
    }

    char *name = malloc(sizeof(char)*(end-cmd));
    memcpy(name,cmd+1,end-cmd-1);
    name[end-cmd-1]='\0';


    while( **i!='('  && **i>'9' ){
        ++*i;
    }
    return name;
}


void var_permutations(int var_cnt, Stack2 *a_stack, NFA *a_to_push,char *i){
    if(var_cnt==1){
        Stack2_push(a_stack,a_to_push);
    }
    if(var_cnt==2){

        int x_first=0;
        for (char *j = i; *j != ')'; ++j) {
            if (*j == 'x') { x_first=1; break; }
            if (*j == 'y') { break; }
        }

        if(x_first){
            Stack2_push(a_stack,a_to_push);
        }
        else{
            NFA *tmp = NFA_swap_digits(a_to_push,0,1);
            Stack2_push(a_stack,tmp);
            NFA_free(a_to_push);
        }

    }

    if(var_cnt==3){
        int x_pos=-1;
        int y_pos=-1;
        int z_pos=-1;
        int cnt_=0;
        for (char *j = i; *j != ')'; ++j) {
            if (*j == 'x') { x_pos=cnt_++; }
            if (*j == 'y') { y_pos=cnt_++; }
            if (*j == 'z') { z_pos=cnt_++; }
        }

        NFA *tmp1 = NFA_swap_digits(a_to_push,0,x_pos);
        NFA_free(a_to_push);
        if(x_pos==1){
            NFA *tmp2 = NFA_swap_digits(tmp1,0,y_pos);
            NFA_free(tmp1);
            Stack2_push(a_stack,tmp2);
        }
        else{
            NFA *tmp2 = NFA_swap_digits(tmp1,1,y_pos);
            Stack2_push(a_stack,tmp2);
            NFA_free(tmp1);
        }

    }
}





NFA *lin_helper(char *cmd){
    char *l_par = cmd;
    while(*l_par != '('){
        ++l_par;
    }
    int factor,bias;
    sscanf(l_par,"%*c%d%*c%*c%d",&factor,&bias);
    return NFA_lin_term(factor,bias);
}

NFA *read_cool_lin(char *str){

}


NFA *Parser(char *formula, a_dict *dict ){
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

            case 'E':
                Operator op_= op_init(8);
                char *first_bracket=i;
                while(*first_bracket != '('){
                    if(*first_bracket == 'x'){ op_.pr_x = 1;}
                    if(*first_bracket == 'y'){ op_.pr_y = 1;}
                    if(*first_bracket == 'z'){ op_.pr_z = 1;}
                    ++first_bracket;
                }
                Stack_push(op_stack,op_);
                break;

            case 'A':
                Operator op_1= op_init(9);
                char *first_bracket_=i;
                while(*first_bracket_ != '('){
                    if(*first_bracket_ == 'x'){ op_1.pr_x = 1;}
                    if(*first_bracket_ == 'y'){ op_1.pr_y = 1;}
                    if(*first_bracket_ == 'z'){ op_1.pr_z = 1;}
                    ++first_bracket_;
                }
                Stack_push(op_stack,op_1);
                break;

            case '$':
                ++i;
                NFA *a_to_push;
                int from_dict = 0;
                char *symbol=i;
                char *name = get_name_parser(i-1,&symbol);//нельзя цифры в именах???

                if(*symbol >= '0' && *symbol <= '9') {
                    if (strstr(i, "div") == i) {
                        int num = (int) strtol(symbol, NULL, 10);
                        a_to_push = NFA_div_n(num);
                    } else if (strstr(i, "const") == i) {
                        int num = (int) strtol(symbol, NULL, 10);
                        a_to_push = NFA_const(num);
                    } else if (strstr(i, "mult") == i) {
                        int num = (int) strtol(symbol, NULL, 10);
                        a_to_push = NFA_mult_scalar(num);
                    }
                }


                else {
                    NFA *tmp = NFA_copy(dict_get_a(dict,name));
                    if(!tmp){
                        printf("invalid automaton name\n");
                        printf("%s\n",i);
                        free(name);
                        break;
                    }
                    else{
                        a_to_push = tmp;
                        from_dict = 1;
                    }
                }
                int lin = 0;
                char *j = symbol;
                while(*j != '('){

                    ++j;
                } // otherwise -> unexpected symbol: '(' expected

                while(*j != ')'){

                    ++j;
                    if(*j >= '0' && *j <= '9') //*j=='0' || *j=='1' || *j=='2' || *j=='3' || *j=='4' || *j=='5' || *j=='6' || *j=='7' || *j=='8' || *j=='9'){
                        lin=1;
                }
                // we always expect linear term: x, x+1, 1+x 2*x, 2*x+1, etc
                if(lin){

                    NFA *lin_a = lin_helper(i);

                    NFA *res = NFA_subs(a_to_push,lin_a);
                    NFA_free(lin_a);
                    Stack2_push(a_stack,res);
                    if(!from_dict){
                        NFA_free(a_to_push);
                    }
                }
                else
                    Stack2_push(a_stack,a_to_push);

                free(name);
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


