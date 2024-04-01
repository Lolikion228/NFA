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

int project_int(int n, int num_cord){
    int n1 = n & ( (1 << num_cord) - 1 );
    int n2 = n >> (num_cord + 1);
    return n1 + (n2 << num_cord);
}


int extend_int(int n, int num_cord, int bit){
    int n1 = n & ( (1 << num_cord) - 1 );
    int n2 = n >> (num_cord);
    return n1 + ( n2 << (num_cord + 1) ) + (bit << num_cord);
}

int swap_bits(int n, int n1, int n2){
    int bit1 = ( n & (1 << n1) ) != 0;
    int bit2 = ( n & (1 << n2) ) != 0;

    int x = bit1^bit2;

    x = (x << n1) | (x << n2);

    return n^x;

}
