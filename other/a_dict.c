//
// Created by lolikion on 27.04.24.
//

#include "a_dict.h"
#include "nfa.h"
#include "stdlib.h"
#include "string.h"

a_dict *dict_init(){
    a_dict *dict = malloc(sizeof(struct a_dict));
    dict->keys = NULL;
    dict->automata = NULL;
    dict->len = 0;
    return dict;
}


NFA  *dict_get_a(a_dict *dict, char *key){
    NFA *res = NULL;

    for(int i=0; i<dict->len; ++i){

        if(!strcmp(dict->keys[i],key)){
            res = dict->automata[i];
            break;
        }

    }

    return res;
}


void dict_free(a_dict *dict){

    for(int i=0; i<dict->len; ++i){
        if(i>=3){
            free(dict->keys[i]);
        }
        NFA_free(dict->automata[i]);
    }

    free(dict->keys);
    free(dict->automata);
    free(dict);
}

void dict_add(a_dict *dict, char *key, NFA *a){

    for(int i=0; i<dict->len; ++i){
        if(!strcmp(dict->keys[i],key)){
            NFA_free(dict->automata[i]);
            dict->automata[i]=a;
            return;
        }
    }

    dict->automata = (NFA**)realloc(dict->automata, (dict->len + 1) * (sizeof(NFA*)) );
    dict->automata[dict->len] = a;
    dict->keys = (char**)realloc(dict->keys, (dict->len + 1) * (sizeof(char*)) );
    dict->keys[dict->len] = key;
    ++dict->len;
}