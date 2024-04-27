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

            case 'E':
                int project_x = 0;
                int project_y = 0;
                int project_z = 0;
                char *first_bracket=i;
                while(*first_bracket != '['){
                    if(*first_bracket == 'x'){ project_x = 1;}
                    if(*first_bracket == 'y'){ project_y = 1;}
                    if(*first_bracket == 'z'){ project_z = 1;}
                    ++first_bracket;
                }
                int cnt = 0;
                while(*(first_bracket+cnt)!=']'){
                    cnt++;
                }

                char *new_formula = calloc(cnt,sizeof(char));
                memcpy(new_formula,first_bracket+1,cnt-1);
                new_formula[cnt-1] = '\0';

                NFA *tmp = Parser(new_formula,automata_names,automata,automata_cnt); // everywhere in the code
                // (automata_names,automata,automata_cnt) into a single structure

                free(new_formula);

                projection_helper(project_x, project_y, project_z, &tmp);

                Stack2_push(a_stack,tmp);

                i = first_bracket + cnt;

                break;

//            case 'A':
//                // Ax[ formula(x) ]  <=>   !Ex[ !formula(x) ]
//                break;


            case '$':

                ++i;
                NFA *a_to_push;

                char *name = get_name_parser(i-1);

                if(strstr(i,"div")==i){
                    int num = (int) strtol(name+3,NULL,10);
                    a_to_push = NFA_div_n(num);
                }
                else if(strstr(i,"is_zero")==i)
                    a_to_push = NFA_from_file("../automata/lsd/zeros.txt");
                else if(strstr(i,"is_equal")==i)
                    a_to_push = NFA_from_file("../automata/lsd/x_eq_y.txt");
                else if(strstr(i,"sum")==i)
                    a_to_push = NFA_from_file("../automata/lsd/sum.txt");
                else {

                    int valid_id = 0;

                    if(automata_cnt!=0){
                        for(int j=0; j<automata_cnt; ++j){
                            if(!strcmp(name,automata_names[j])){
                                NFA *tmp = NFA_copy(automata[j]);
                                a_to_push = tmp;
                                valid_id=1;
                            }
                        }

                    }

                    if(!valid_id){
                        printf("invalid automaton name\n");
                        printf("%s\n",i);
                        break;
                    }

                }

                int var_cnt=0;
                char *l_par=i;
                while(*l_par!='('){
                    ++l_par;
                }
                for (char *j = l_par; *j != ')'; ++j) {
                    if (*j == 'x') { ++var_cnt; }
                    if (*j == 'y') { ++var_cnt; }
                    if (*j == 'z') { ++var_cnt; }
                }

                var_permutations(var_cnt,a_stack,a_to_push,i);

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
