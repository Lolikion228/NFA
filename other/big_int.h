//
// Created by lolikion on 02.10.23.
//

#include <math.h>
#include <stdio.h>
#ifndef UNTITLED_BIG_INT_H
#define UNTITLED_BIG_INT_H

typedef struct big_int {
    int bit_len;
    unsigned char *number;
    unsigned int length;
    char sign;
} big_int;

char* int_to_binary_string(int number);

big_int *big_int_get(const char *bin_number);

int big_int_equal(const big_int *n1, const big_int *n2);//ok

int big_int_equal_sgn(const big_int *n1,const big_int *n2);//ok

void big_int_print(const big_int *n);//ok

void big_int_print2(const unsigned int n, ...);

void big_int_free(big_int **n);//ok

void big_int_free2(const unsigned int n0, ...);

void big_int_test_loop(long long n, int (*func)(big_int*, unsigned int));

big_int* big_int_mul(const big_int* n1,const big_int* n2);

void big_int_dlz(big_int *n);//ok

void big_int_swap(big_int *n1, big_int *n2);//ok

void big_int_swap2(big_int *n1, big_int *n2);//ok

int big_int_leq(const big_int *n1,const big_int *n2);//ok

int big_int_geq(const big_int *n1,const big_int *n2);//ok

big_int* big_int_disj(const big_int *n1,const big_int *n2);//ok

void big_int_bin_shft_r(big_int *n);//ok

void big_int_bin_shft_l(big_int *n);//ok

void big_int_bin_shft_r2(big_int *n,int cnt);//ok

void big_int_bin_shft_l2(big_int *n,unsigned int cnt);//ok

big_int *big_int_add(big_int *n1, big_int *n2);//ok

void big_int_add2(big_int *n1, big_int *n2);//ok

big_int *big_int_sub(big_int *n1, big_int *n2);//ok

void big_int_sub2(big_int *n1, big_int *n2);//ok

big_int *big_int_euclid_binary(const big_int *x,const big_int *y);//ok

big_int *big_int_copy(const big_int*x);//ok

big_int *big_int_rl_mod_pow(const big_int *x,const big_int *n,const big_int *m);//ok

big_int *big_int_lr_mod_pow(const big_int *x0,const big_int *n, big_int *m);//ok

big_int *big_int_rl_mod_pow2(const big_int *x,const big_int *n,const big_int *m);//ok

big_int *big_int_lr_mod_pow2(const big_int *x0,const big_int *n, big_int *m);//ok

big_int *big_int_karatsuba_mult2(const big_int *n1,const big_int *n2);//ok

big_int *big_int_rnd(unsigned int n);//ok

big_int *big_int_get_prime(unsigned int len,unsigned int tst_cnt);//ok

big_int *big_int_mul_inverse(const big_int *n1,big_int *mod);

void big_int_set_bit(big_int *n, long long num, int x);

void big_int_div2(const big_int *n1, big_int *n2, big_int *res1, big_int *rmdr);

void big_int_div3(const big_int *n1, big_int *n2, big_int *res1);

int big_int_primality_test(big_int *n, unsigned int tst_cnt);

void big_int_div2_for_pow(const big_int *n1, big_int *n2, big_int *rmdr);

big_int *big_int_lr3(const big_int *x,const big_int *n);

void big_int_fprint(const big_int *n,FILE *f);
#endif //UNTITLED_BIG_INT_H
