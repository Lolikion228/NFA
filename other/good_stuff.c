//
// Created by lolikion on 28.03.24.
//

#include "good_stuff.h"
#include "stdlib.h"

int *zeros(int n){
    return (int *)calloc(n,sizeof(int));
}