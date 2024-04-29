//
// Created by lolikion on 15.04.24.
//

#include "nfa.h"
#include "CLI.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "logic.h"
#include "a_dict.h"

char *get_name_for_def(const char *command){

    int first_ix = -1, second_ix = -1;

    for(int i=0; i<strlen(command); ++i){
        if(command[i] == ' '){
            if(first_ix == -1){
               first_ix = i;
            }
            else{
                second_ix = i;
                break;
            }
        }
    }

    char *name = malloc(sizeof(char) * (second_ix - first_ix ) );
    memcpy(name,command + first_ix + 1 ,sizeof(char) * (second_ix - first_ix) );
    name[second_ix - first_ix - 1] = '\0';

    return name;
}

char *get_formula_for_def(const char *command){

    int first_ix = -1, second_ix = -1;

    for(int i=0; i<strlen(command); ++i){
        if(command[i] == '"'){
            if(first_ix == -1){
                first_ix = i;
            }
            else{
                second_ix = i;
                break;
            }
        }
    }

    char *formula = malloc(sizeof(char) * (second_ix - first_ix ) );
    memcpy(formula, command + first_ix + 1 , sizeof(char) * (second_ix - first_ix) );
    formula[second_ix - first_ix - 1] = '\0';

    return formula;
}

char *get_name_for_eval(const char *command){

    int first_ix = -1, second_ix = -1;

    for(int i=0; i<strlen(command); ++i){
        if(command[i] == ' ')
            first_ix = i;
        if(command[i] == '('){
            second_ix = i;
            break;
        }
    }

    char *name = malloc(sizeof(char) * (second_ix - first_ix ) );
    memcpy(name,command + first_ix + 1 ,sizeof(char) * (second_ix - first_ix) );
    name[second_ix - first_ix - 1] = '\0';

    return name;
}

int *get_numbers_for_eval(const char *command, int dim){

    int first_ix = -1, second_ix = -1;

    for(int i=0; i<strlen(command); ++i){
        if(command[i] == '(')
            first_ix = i;
        if(command[i] == ')'){
            second_ix = i;
            break;
        }
    }

    char *num_str = malloc(sizeof(char) * (second_ix - first_ix ) );
    memcpy(num_str, command + first_ix + 1 , sizeof(char) * (second_ix - first_ix) );
    num_str[second_ix - first_ix - 1] = '\0';

    int *numbers = malloc(3*sizeof(int));
    if(dim==1){
        sscanf (num_str,"%d", &numbers[0]);
    }
    if(dim==2){
        sscanf (num_str,"%d%*c%d", &numbers[0],&numbers[1]);
    }
    if(dim==3){
        sscanf (num_str,"%d%*c%d%*c%d", &numbers[0],&numbers[1],&numbers[2]);
    }
    free(num_str);

    return numbers;
}

// i = [12;124]
int *get_var_bounds(char* command,char variable){
    char *var = command;
    int *res = malloc(2 * sizeof(int));

    while(*var != ')')
        ++var;

    while(*var != variable)
        ++var;

    while(*var != '[')
        ++var;

    sscanf (var+1,"%d%*c%d", &res[0],&res[1]);

    return res;
}

void loop_helper(NFA *tmp, char *command){
    switch (tmp->dim) {

        case 1:
            int *i_bounds_1 = get_var_bounds(command,'i');

            for(int i_=i_bounds_1[0]; i_ <= i_bounds_1[1]; ++i_){
                printf("[ i ] = [ %d ]; res = %d\n",i_, NFA_check(tmp,&i_));
            }

            free(i_bounds_1);
            break;

        case 2:
            int *i_bounds_2 = get_var_bounds(command,'i');
            int *j_bounds_2 = get_var_bounds(command,'j');
            int *nums_2 = malloc(2 * sizeof(int) );

            for(int i_=i_bounds_2[0]; i_ <= i_bounds_2[1]; ++i_)
                for(int j_=j_bounds_2[0]; j_ <= j_bounds_2[1]; ++j_){
                    nums_2[0]=i_;
                    nums_2[1]=j_;
                    printf("[ i, j ] = [ %d, %d ]; res = %d\n",i_, j_, NFA_check(tmp,nums_2));
                }

            free(i_bounds_2);
            free(j_bounds_2);
            free(nums_2);
            break;

        case 3:
            int *i_bounds_3 = get_var_bounds(command,'i');
            int *j_bounds_3 = get_var_bounds(command,'j');
            int *k_bounds_3 = get_var_bounds(command,'k');
            int *nums_3 = malloc(3 * sizeof(int) );

            for(int i_=i_bounds_3[0]; i_ <= i_bounds_3[1]; ++i_)
                for(int j_=j_bounds_3[0]; j_ <= j_bounds_3[1]; ++j_)
                    for(int k_=k_bounds_3[0]; k_ <= k_bounds_3[1]; ++k_){
                        nums_3[0]=i_;
                        nums_3[1]=j_;
                        nums_3[2]=k_;
                        printf("[ i, j, k ] = [ %d, %d, %d ]; res = %d\n",i_, j_,k_, NFA_check(tmp,nums_3));
                    }

            free(i_bounds_3);
            free(j_bounds_3);
            free(k_bounds_3);
            free(nums_3);
            break;

    }
}

//pic id\n
void pic_helper(a_dict *dict, char *cmd){
    char *space=cmd;
    while(*space != ' ')
        ++space;

    char *name = malloc(sizeof(char) * ( strlen(cmd)-4 ) );
    name[strlen(cmd)-5]='\0';
    memcpy(name,space+1,strlen(cmd)-5);

    NFA *tmp = dict_get_a(dict,name);
    if(!tmp){
        printf("invalid automaton name: %s\n",name);
    }
    else{
        NFA_to_pic(tmp,0);
    }
    free(name);
}

void min_helper(a_dict *dict, char *cmd){

    char *space=cmd;
    while(*space != ' ')
        ++space;

    char *name = malloc(sizeof(char) * ( strlen(cmd)-9 ) );
    name[strlen(cmd)-10]='\0';
    memcpy(name,space+1,strlen(cmd)-10);

    int tmp_ix = dict_get_a_ix(dict,name);

    if(tmp_ix == -1){
        printf("invalid automaton name: %s\n",name);
    }
    else{
        NFA *tmp1 = NFA_to_DFA(dict->automata[tmp_ix]);
        NFA_free(dict->automata[tmp_ix]);
        dict->automata[tmp_ix] = DFA_minimization(tmp1);
        NFA_free(tmp1);
    }

    free(name);

}


void write_help(){
    printf("*-*-*-*-*-*-*-*-*-*-*-* NFA_prover_CLI *-*-*-*-*-*-*-*-*-*-*-*\n\n");

    printf("help: help\n    just prints this text.\n\n");
    printf("exit: exit\n    interrupts the program.\n\n");
    printf("list: list\n    prints available automata names and some additional info about each of them.\n\n");
    printf("minimize: minimize [automaton_name]\n    applies NFA->DFA and DFA_minimization to the chosen automaton\n\n");
    printf("    example: minimize div322\n\n");
    printf("pic: pic [automaton_name]\n    creates an image of the automaton '/temp/automaton0.png'.\n\n");
    printf("    example: pic is_equal\n\n");
    printf("def: def [automaton_name] \"formula\" \n    defines a new automaton. you can use automata that were defined earlier.\n\n");
    printf("    use of a specific automaton: \"$div17(x)\" \n");
    printf("    logical operations: \"($div43(x) | !$div2(x)) & $is_zero(x)\" \n");
    printf("    universal quantifier: \"Ax[ formula ]\" or \"Ax,y,z[ formula ]\" \n");
    printf("    existential quantifier: \"Ex[ formula ]\" or \"Ex,y,z[ formula ]\" \n\n");
    printf("    example1: def geq \"!Ey[ $sum(x,y,z) ] | $is_equal(x,y)\"\n");
    printf("    example2: def th3 \"Ax[ Ay[ Ez[ $sum(x,y,z) ] ] ]\"\n\n");
    printf("eval: eval [automaton_name](x_1,x_2,...,x_[automaton_dim])\n");
    printf("    evaluates x_i w.r.t. automaton_name. prints 1 if accepted, 0 otherwise.\n\n");
    printf("    example1: eval div15(45)\n       output: res = 1\n");
    printf("    example2: eval div10(101)\n      output: res = 0\n");
    printf("    example3 (when automaton_dim=0) : eval theorem1()\n      output: res = 1\n\n");
    printf("eval_loop: eval [automaton_name](i) i=[i_min,i_max]\n");
    printf("    simply executes eval many times for 'i' from the given range\n\n");
    printf("    example1: eval div3(i) i=[13,16]\n");
    printf("        output:\n"
           "          [ i ] = [ 13 ]; res = 0\n"
           "          [ i ] = [ 14 ]; res = 0\n"
           "          [ i ] = [ 15 ]; res = 1\n"
           "          [ i ] = [ 16 ]; res = 0\n");
    printf("    example2: eval sum(i,j,k) i=[0,1] j=[1,2] k=[1,2]\n");
    printf("        output:\n"
           "          [ i, j, k ] = [ 0, 1, 1 ]; res = 1\n"
           "          [ i, j, k ] = [ 0, 1, 2 ]; res = 0\n"
           "          [ i, j, k ] = [ 0, 2, 1 ]; res = 0\n"
           "          [ i, j, k ] = [ 0, 2, 2 ]; res = 1\n"
           "          [ i, j, k ] = [ 1, 1, 1 ]; res = 0\n"
           "          [ i, j, k ] = [ 1, 1, 2 ]; res = 1\n"
           "          [ i, j, k ] = [ 1, 2, 1 ]; res = 0\n"
           "          [ i, j, k ] = [ 1, 2, 2 ]; res = 0\n\n");
    printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
}
/*
 * def name "formula"
 * eval name(number)
 * */
void app(){
    typedef enum cli_state {def, invalid_command, exit_, eval, list, pic, help, minimize} cli_state;
    int exiting = 0;
    char *command = malloc(330 * sizeof(char));

    a_dict *dict = dict_init();

    dict->keys = malloc(3 * sizeof(char*) ); // load all from /automata/lsd
    dict->automata = malloc(3 * sizeof(NFA*) );
    dict->len = 3;

    dict->keys[0]="sum";
    dict->keys[1]="is_equal";
    dict->keys[2]="is_zero";

    dict->automata[0] = NFA_from_file("../automata/lsd/sum.txt");
    dict->automata[1] = NFA_from_file("../automata/lsd/x_eq_y.txt");
    dict->automata[2] = NFA_from_file("../automata/lsd/zeros.txt");


    while(1){
        cli_state curr_state = invalid_command;

        printf(">>> ");
        command = fgets(command,322,stdin);

        if(!strcmp(command,"exit\n")){ curr_state = exit_; }
        if(strstr(command,"def") == command){ curr_state = def; }
        if(strstr(command,"eval") == command){ curr_state = eval; }
        if(strstr(command,"list") == command){ curr_state = list; }
        if(strstr(command,"pic") == command){ curr_state = pic; }
        if(strstr(command,"help") == command){ curr_state = help; }
        if(strstr(command,"minimize") == command){ curr_state = minimize; }

        switch (curr_state) {


            case exit_:
                printf("exiting...\n");
                exiting = 1;
                break;

            case pic:
                pic_helper(dict,command);
                break;

            case minimize:
                min_helper(dict,command);
                break;

            case help:
                write_help();
                break;

            case list:
                // print predefined automata names
                printf("available automata names:\n");
                for(int j=0; j<dict->len; ++j){
                    printf("%d. %s (dim = %d) (states_cnt=%d) (is_dfa=%d)",
                           j,
                           dict->keys[j],
                           dict->automata[j]->dim,
                           dict->automata[j]->states_cnt,
                           NFA_is_dfa(dict->automata[j]));
                    if(dict->automata[j]->dim==0){printf(" (truth=%d)",dict->automata[j]->truth );}
                    printf("\n");
                }
                printf("%d. constN (dim = 1)  e.g. you can use $const13(x)\n",dict->len);
                printf("%d. multN (dim = 2)  an automaton for N*x=y, e.g. you can use $mult15(x,y) \n",dict->len+1);
                printf("%d. divN (dim = 1)  an automaton for N | x, e.g. you can use $div23(x) \n",dict->len+2);
                break;

            case invalid_command:
                printf("invalid command\n");
                break;

            case def:

                char *formula = get_formula_for_def(command);
                char *name = get_name_for_def(command);
                NFA *tmp_1 = Parser(formula,dict);

                if(tmp_1!=NULL){
                    dict_add(dict,name,tmp_1);
                }
                else{
                    free(name);
                }

                free(formula);
                break;

            case eval:
                char *name_ = get_name_for_eval(command);

                NFA *tmp = dict_get_a(dict,name_);

                if(!tmp){
                    printf("invalid automaton name: %s\n",name_);
                    free(name_);
                    break;
                }

                if(tmp->dim==0){
                    printf("result = %d\n",tmp->truth);
                }
                else{

                    int loop = 0;
                    char *l_par = command;

                    while(*l_par != '(')
                        ++l_par;

                    while(*l_par != ')'){
                        if(*l_par == 'i' ){loop = 1;}
                        ++l_par;
                    }

                    if(loop){
                        loop_helper(tmp,command);
                    }
                    else{
                        int *numbers = get_numbers_for_eval(command,tmp->dim);
                        printf("result = %d\n", NFA_check(tmp,numbers));
                        free(numbers);
                    }

                }

                free(name_);
                break;
        }


        if(exiting){
            dict_free(dict);
            break;
        }

    }
    free(command);

}
