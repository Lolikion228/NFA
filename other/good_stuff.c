//
// Created by lolikion on 28.03.24.
//

#include "good_stuff.h"
#include "stdlib.h"
#include "stdio.h"

int *zeros(int n){
    return (int *)calloc(n,sizeof(int));
}

void print_array(int a[], int len){
    printf("[");
    for(int i=0; i<len; i++){
        printf(" %d", a[i]);
    }
    printf(" ]\n");
}