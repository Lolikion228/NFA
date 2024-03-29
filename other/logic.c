//
// Created by lolikion on 29.03.24.
//
#include "logic.h"
#include "stdlib.h"
#include "stdio.h"

//  0  1  2  3 4 5
//  ! /\ \/ -> ( )
operation OP_init(int id){
    operation *res = malloc(sizeof(struct operation));
    switch(id) {
        case(0):
            res->id=0;
            res->priority=0;
            res->is_unary=1;
            break;
        case(1):
            res->id=1;
            res->priority=1;
            res->is_unary=0;
            break;
        case(2):
            res->id=2;
            res->priority=2;
            res->is_unary=0;
            break;
        case(3):
            res->id=3;
            res->priority=3;
            res->is_unary=0;
            break;
        case(4):
            res->id=4;
            res->priority=4;
            res->is_unary=1;
            break;
        case(5):
            res->id=5;
            res->priority=4;
            res->is_unary=1;
            break;
        default:
            printf("wrong operation id\n");
            exit(1);
    }
    return *res;
}