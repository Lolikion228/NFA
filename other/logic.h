//
// Created by lolikion on 29.03.24.
//

#ifndef NFA_LOGIC_H
#define NFA_LOGIC_H

typedef struct operation operation;


typedef struct operation{
    int is_unary;
    int priority;
    int id;
}operation;


operation OP_init(int id);

#endif //NFA_LOGIC_H
