//
// Created by lolikion on 15.04.24.
//

#include "CLI.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "logic.h"
#include "nfa.h"
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


/*
 * def name "formula"
 * eval name(number)
 * */
void app(){
    typedef enum cli_state {def, invalid_command, exit_, eval, list} cli_state;
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

        switch (curr_state) {


            case exit_:
                printf("exiting...\n");
                exiting = 1;
                break;

            case list:
                // print predefined automata names
                printf("available automata names:\n");
                for(int j=0; j<dict->len; ++j){
                    printf("%d. %s (dim = %d)\n",j,dict->keys[j],dict->automata[j]->dim);
                }
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
                    printf("result = %d\n", NFA_th_check(tmp));
                }
                else{
                    int *numbers = get_numbers_for_eval(command,tmp->dim);
                    printf("result = %d\n", NFA_check(tmp,numbers));
                    free(numbers);
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
