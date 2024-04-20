//
// Created by lolikion on 15.04.24.
//

#include "CLI.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "logic.h"
#include "nfa.h"

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

int get_number_for_eval(const char *command){

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
    int number = (int)strtol(num_str, NULL, 10);
    free(num_str);

    return number;
}


/*
 * def name "formula"
 * eval name(number)
 * */
void app(){
    typedef enum cli_state {def, invalid_command, exit_, eval} cli_state;
    int automata_cnt=0, exiting = 0;
    char *command = malloc(330 * sizeof(char));
    char **automata_names = NULL;
    NFA **automata = NULL;

    while(1){
        cli_state curr_state = invalid_command;


        printf(">>> ");
        command = fgets(command,322,stdin);

        if(!strcmp(command,"exit\n")){ curr_state = exit_; }
        if(strstr(command,"def") == command){ curr_state = def; }
        if(strstr(command,"eval") == command){ curr_state = eval; }


        switch (curr_state) {
            case exit_:
                printf("exiting...\n");
                exiting = 1;
                break;

            case invalid_command:
                printf("invalid command\n");
                break;

            case def:

                char *formula = get_formula_for_def(command);
                automata_names = (char**)realloc(automata_names, (automata_cnt + 1) * (sizeof(char*)) );
                automata = (NFA**)realloc(automata, (automata_cnt + 1) * (sizeof(NFA*)) );
                automata_names[automata_cnt] = get_name_for_def(command);;
                automata[automata_cnt] = Parser(formula,automata_names,automata,automata_cnt);
                ++automata_cnt;

                free(formula);
                break;

            case eval:
                char *name_ = get_name_for_eval(command);
                int number = get_number_for_eval(command);
                for(int i=0; i<automata_cnt; ++i){
                    if(!strcmp(automata_names[i],name_)){
                        printf("result = %d\n", NFA_check(automata[i],&number));
                        break;
                    }
                }
                free(name_);
                break;
        }


        if(exiting){
            for(int i=0; i<automata_cnt; ++i){
                free(automata_names[i]);
                NFA_free(automata[i]);
            }
            free(automata_names);
            free(automata);
            break;
        }

    }
    free(command);

}
