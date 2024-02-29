//
// Created by lolikion on 02.10.23.
//
#include <time.h>
#include <string.h>
#include "big_int.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include<math.h>
#define MAX_BINARY_LENGTH 160000
#define const1 4



big_int *big_int_get(const char *bin_number) {
    big_int *res = (big_int *) malloc(sizeof(big_int));
    int bit_len = strlen(bin_number), sign = 0;
    res->bit_len=bit_len;
    if (bin_number[0] == '-') {
        sign = 1;
        res->sign = '-';
    } else res->sign = '+';
    res->length = (bit_len + 7 - sign) >> 3;
    res->number = (unsigned char *) calloc(res->length, sizeof(res->number[0]));
    if (res->number == NULL) {
        printf("memory error in big_int_get\n");
        return NULL;
    }
    int i;
    for (i = 0; i < bit_len - sign; i++) {
        res->number[i / 8] += (bin_number[bit_len - i - 1] - '0') << (i % 8);
    }
    big_int_dlz(res);
    return res;
}


int big_int_equal_sgn(const big_int *n1,const big_int *n2) {
    if (n1->sign != n2->sign) {
        return 0;
    }
    if (n1->length != n2->length) {
        return 0;
    }
    for (int i = 0; i < n1->length; i++)
        if ((n1->number[i]) != (n2->number[i])) return 0;
    return 1;
}


int big_int_equal(const big_int *n1,const big_int *n2) {
    if (n1->length != n2->length) {
        return 0;
    }

    for (int i = 0; i < n1->length; i++)
        if ((n1->number[i]) != (n2->number[i])) return 0;
    return 1;
}


void big_int_dlz(big_int *n) {
    int i = (n->length) - 1;
    while (((i > 0) * (n->number[i] == 0)) != 0) {
        i--;
    }
    if ((n->length) != i + 1) {
        n->length = i + 1;
        n->number = (unsigned char *) realloc(n->number, n->length);
        if (n->number == NULL) {
            printf("memory error in big_int_dlz\n");
        }
    }
}


void big_int_print(const big_int *n) {
    putchar(n->sign);
    for (int i = n->length - 1; i > -1; i--) {
        int x = n->number[i];
        int bit = 128;
        for (int j = 7; j > -1; j--) {
            printf("%i", (x & bit) != 0);
            bit >>= 1;
        }
        if (i)printf(" ");//REDO
    }
    printf("\n");
}

void big_int_fprint(const big_int *n,FILE *f) {
    for (int i = n->length - 1; i > -1; i--) {
        int x = n->number[i];
        int bit = 128;
        for (int j = 7; j > -1; j--) {
            fprintf(f,"%i", (x & bit) != 0);
            bit >>= 1;
        }
        if (i)printf(" ");//REDO
    }
}

void big_int_print2(const unsigned int n0, ...) {
    va_list ptr;
    va_start(ptr, n0);
    for (int j = 0; j < n0; j++) {
        big_int *n = (big_int *) va_arg(ptr, big_int * );
        putchar(n->sign);
        for (int i = n->length - 1; i > -1; i--) {
            int x = n->number[i];
            int bit = 128;
            for (int j = 7; j > -1; j--) {
                printf("%i", (x & bit) != 0);
                bit >>= 1;
            }
//        if (i)printf(" ");
        }
        printf("\n");
    }
    va_end(ptr);
}





void big_int_swap(big_int *n1, big_int *n2) {
    char sgn = n1->sign;
    unsigned int len = n1->length;
    unsigned char *num = (unsigned char *) calloc(n1->length, sizeof(n1->number[0]));
    if (num == NULL) {
        printf("memory error in big_int_swap\n");
    }
    memmove(num, n1->number, n1->length);//ok
    n1->sign = n2->sign;
    n1->length = n2->length;
    n1->number = (unsigned char *) realloc(n1->number, n2->length);

    memmove(n1->number, n2->number, n2->length);
    n2->sign = sgn;
    n2->length = len;
    n2->number = (unsigned char *) realloc(n2->number, len);
    memmove(n2->number, num, len);
    free(num);
}

void big_int_swap2(big_int *n1, big_int *n2) {
    big_int temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

big_int *big_int_copy(const big_int *x) {
    big_int *n3 = (big_int *) malloc(sizeof(big_int));
    n3->sign = x->sign;
    n3->bit_len=x->bit_len;
    n3->length = x->length;
    n3->number = (unsigned char *) calloc(n3->length, sizeof(x->number[0]));
    if (n3->number == NULL) {
        printf("memory error in big_int_copy\n");
        return NULL;
    }
    memmove(n3->number, x->number, x->length * sizeof(x->number[0]));
    return n3;
}


int big_int_leq(const big_int *n1,const big_int *n2) //n1<=n2
{
    if (n1->length < n2->length) return 1;
    if (n1->length > n2->length) return 0;
    for (int i = n1->length - 1; i > -1; i--) {
        if (n1->number[i] < n2->number[i]) return 1;
        if (n1->number[i] > n2->number[i]) return 0;
    }
    return 1;
}


int big_int_geq(const big_int *n1,const big_int *n2)//n1<=n2
{
    if (n1->sign != n2->sign) return (n1->sign == '+') ? 0 : 1;
    else {
        if (n1->length < n2->length) return 1 * (n1->sign == '+');
        if (n1->length > n2->length) return 0 + (n1->sign != '+');
        for (int i = n1->length; i > -1; i--) {
            if (n1->number[i] < n2->number[i]) return 1 * (n1->sign == '+');
            if (n1->number[i] > n2->number[i]) return 0 + (n1->sign != '+');
        }
        return 1;
    }
}


big_int *big_int_disj(const big_int *n1,const big_int *n2) {
    int min = (int) fmin(n1->length, n2->length);
    big_int *n3 = (big_int *) malloc(sizeof(big_int));
    n3->length = min;
    n3->number = (unsigned char *) calloc(n3->length, sizeof(n3->number[0]));
    if (n3->number == NULL) {
        printf("memory error in big_int_disj\n");
        return NULL;
    }
    for (int i = 0; i < min; i++) {
        n3->number[i] = (n2->number[i]) | (n1->number[i]);
    }
    n3->sign = '+';
    big_int_dlz(n3);
    return n3;
}


void big_int_bin_shft_r(big_int *n) {
    for (int i = 0; i < n->length; i++) {
        n->number[i] >>= 1;
        if (i != ((n->length) - 1)) n->number[i] += (((n->number[i + 1])) & 1) << 7;
    }
    big_int_dlz(n);
}


void big_int_bin_shft_l(big_int *n) {
    int t = (n->number[(n->length) - 1]) & 128;
    if (t) {
        n->length++;
        n->number = (unsigned char *) realloc(n->number, n->length);
        if (n->number == NULL) printf("memory error in big_int_bin_shft_l\n");
        memset(n->number + (n->length) - 1, 0, 1);
    }
    for (int i = n->length - 1; i > -1; i--) {
        n->number[i] <<= 1;
        if (i) n->number[i] += (n->number[i - 1] & 128) != 0;
    }
}


void big_int_bin_shft_r2(big_int *n, int cnt) {
    for (int i = 0; i < (cnt & 7); i++) big_int_bin_shft_r(n);
    unsigned int x = (cnt >> 3);
    if (x >= n->length) {
        free(n->number);
        n->sign = '+';
        n->length = 1;
        n->number = (unsigned char *)  calloc(1, 1);
    } else {
        if (x) {
            n->length -= x;
            memmove(n->number, n->number + x, sizeof(n->number[0]) * (n->length));
            n->number = (unsigned char *) realloc(n->number, (n->length) * sizeof(n->number[0]));
        }
    }
    big_int_dlz(n);
}


void big_int_bin_shft_l2(big_int *n, unsigned int cnt) {
    for (int i = 0; i < cnt % 8; i++) big_int_bin_shft_l(n);
    unsigned int x = cnt / 8;
    n->length += x;
    n->number = (unsigned char *) realloc(n->number, (n->length) * sizeof(n->number[0]));
    if (n->number == NULL) printf("memory error in big_int_bin_shft_l2\n");
    memmove(n->number + x, n->number, n->length - x);
    for (int i = 0; i < x; i++) n->number[i] = 0;
}


void big_int_set_bit(big_int *n, long long num, int x) {
    if ((num / 8) >= (n->length)) {
        n->number = (unsigned char *) realloc(n->number, (num / 8 + 1) * sizeof(n->number[0]));
        if (n->number == NULL) printf("memory error in big_int_set_bit\n");
        memset((n->number) + (n->length), 0, (num / 8) - (n->length) + 1);
        n->length = num / 8 + 1;
    }
    if (x) { // set bit
        n->number[num / 8] |= 1 << (num % 8);
    } else { // clear bit
        n->number[num / 8] &= ~(1 << (num % 8));
    }
    big_int_dlz(n);
}

void big_int_set_bit2(big_int *n, long long num, int x) {
    if ((num / 8) >= (n->length)) {
        n->number = (unsigned char *) realloc(n->number, (num / 8 + 1) * sizeof(n->number[0]));
        if (n->number == NULL) {
            printf("memory error in big_int_set_bit\n");
            return;
        }
        memset((n->number) + (n->length), 0, (num / 8) - (n->length) + 1);
        n->length = num / 8 + 1;
    }
    if (x) { // set bit
        n->number[num / 8] |= 1 << (num % 8);
    } else { // clear bit
        n->number[num / 8] &= ~(1 << (num % 8));
    }
    big_int_dlz(n);
}

big_int *big_int_add(big_int * n1,big_int *n2) {
    if (n1->sign != n2->sign) {
        int t = n1->sign == '+';
        if (t) {
            n2->sign = '+';
            big_int *n4 = (big_int_sub(n1, n2));
            n2->sign = '-';
            big_int_dlz(n4);
            return n4;
        } else {
            n1->sign = '+';
            big_int *n4 = (big_int_sub(n2, n1));
            n1->sign = '-';
            big_int_dlz(n4);
            return n4;
        }
    }
    int mx = (int) fmax(n1->length, n2->length), carry = 0, x;
    big_int *n3 = (big_int *) malloc(sizeof(big_int));
    n3->length = mx + 1;
    n3->number = (unsigned char *) calloc(n3->length, sizeof(n3->number[0]));
    if (n3->number == NULL) { printf("memory error in big_int_add\n"); }
    int t = n1->length <= n2->length;
    if (t) big_int_swap2(n1, n2);
    for (int i = 0; i < mx; i++) {
        if (i < n2->length) x = n1->number[i] + n2->number[i] + carry;
        else x = n1->number[i] + carry;
        n3->number[i] = x & 0xFF;
        carry = x >> 8;
    }
    if (t) big_int_swap2(n1, n2);
    n3->number[mx] = carry;
    n3->sign = n1->sign;
    big_int_dlz(n3);
    return n3;
}


void big_int_add2(big_int *n1, big_int *n2) {
    if (n1->sign != n2->sign) {
        int t = n1->sign == '+';
        if (t) {
            n2->sign = '+';
            big_int *n4 = (big_int_sub(n1, n2));
            n2->sign = '-';
            n1->sign = n4->sign;
            n1->length = n4->length;
            n1->number=(unsigned char*) realloc(n1->number,n4->length);
            memmove(n1->number, n4->number, n4->length);
            big_int_free(&n4);
            big_int_dlz(n1);
        } else {
            n1->sign = '+';
            big_int *n4 = (big_int_sub(n2, n1));
            n1->sign = '-';
            n1->sign = n4->sign;
            n1->length = n4->length;
            n1->number=(unsigned char*) realloc(n1->number,n4->length);
            memmove(n1->number, n4->number, n4->length);
            big_int_free(&n4);
            big_int_dlz(n1);
        }
    } else {
        int mx = (int) fmax(n1->length, n2->length), carry = 0, x;
        big_int *n3 = (big_int *) malloc(sizeof(big_int));
        n3->length = mx + 1;
        n3->number = (unsigned char *) calloc(n3->length, sizeof(n3->number[0]));
        if (n3->number == NULL) { printf("memory error in big_int_add2\n"); }
        int t = n1->length <= n2->length;
        if (t) big_int_swap2(n1, n2);
        for (int i = 0; i < mx; i++) {
            if (i < n2->length) x = n1->number[i] + n2->number[i] + carry;
            else x = n1->number[i] + carry;
            n3->number[i] = x & 0xFF;
            carry = x >> 8;
        }
        if (t) big_int_swap2(n1, n2);
        n3->number[mx] = carry;
        n3->sign = n1->sign;
        big_int_dlz(n3);
        n1->length = n3->length;
        n1->number=(unsigned char*) realloc(n1->number,n3->length);
        memmove(n1->number, n3->number, n3->length);
        big_int_free(&n3);
    }
}


big_int *big_int_sub(big_int *n1, big_int *n2) {
    if (n1->sign != n2->sign) {
        if (n1->sign == '+') {
            n2->sign = '+';
            big_int *n4 = (big_int_add(n1, n2));
            big_int_dlz(n4);
            n2->sign = '-';
            return n4;
        } else {
            n2->sign = '-';
            big_int *n4 = (big_int_add(n2, n1));
            big_int_dlz(n4);
            n2->sign = '+';
            return n4;
        }
    }
    int mx = (int) fmax(n1->length, n2->length), carry = 0;
    big_int *n3 = (big_int *) malloc(sizeof(big_int));
    n3->length = mx;
    n3->number = (unsigned char *) calloc(n3->length, sizeof(n3->number[0]));
    if (n3->number == NULL) { printf("memory error in big_int_sub\n"); }
    int t = big_int_leq(n1, n2);
    if (t)big_int_swap2(n1, n2);
    for (int i = 0; i < mx; i++) {
        if (i < n2->length) {
            if (n1->number[i] > n2->number[i]) {
                n3->number[i] = n1->number[i] - n2->number[i] - carry;
                carry = 0;
            }
            if (n1->number[i] == n2->number[i]) {
                if (carry) {
                    n3->number[i] = 0xFF;
                } else {
                    n3->number[i] = 0;
                }
            }
            if (n1->number[i] < n2->number[i]) {
                if (carry) {
                    n3->number[i] = 0xFF + n1->number[i] - n2->number[i];
                } else {
                    n3->number[i] = 0x100 + n1->number[i] - n2->number[i];
                    carry = 1;
                }
            }
        } else {
            if (carry) {
                if (n1->number[i]) {
                    n3->number[i] = n1->number[i] - 1;
                    carry = 0;
                } else {
                    n3->number[i] = 0xFF;
                }
            } else {
                n3->number[i] = n1->number[i];
            }
        }
    }
    if (t) big_int_swap2(n1, n2);
    if (n1->sign == '+') {
        if (t)n3->sign = '-';
        else n3->sign = '+';
    } else {
        if (t)n3->sign = '+';
        else n3->sign = '-';
    }
    big_int_dlz(n3);
    return n3;
}


void big_int_sub2(big_int *n1, big_int *n2) {
    if (n1->sign != n2->sign) {
        if (n1->sign == '+') {
            n2->sign = '+';
            big_int *n4 = (big_int_add(n1, n2));
            n2->sign = '-';
            n1->sign = n4->sign;
            n1->length = n4->length;
            n1->number=(unsigned char*) realloc(n1->number,n4->length);
            memmove(n1->number, n4->number, n4->length);
            big_int_free(&n4);
            big_int_dlz(n1);
        } else {
            n2->sign = '-';
            big_int *n4 = (big_int_add(n2, n1));
            n2->sign = '+';
            n1->sign = n4->sign;
            n1->length = n4->length;
            n1->number=(unsigned char*) realloc(n1->number,n4->length);
            memmove(n1->number, n4->number, n4->length);
            big_int_free(&n4);
            big_int_dlz(n1);
        }
    } else {
        int mx = (int) fmax(n1->length, n2->length), carry = 0;
        big_int *n3 = (big_int *) malloc(sizeof(big_int));
        n3->length = mx;
        n3->number = (unsigned char *) calloc(n3->length, sizeof(n3->number[0]));
        if (n3->number == NULL) { printf("memory error in big_int_sub2\n"); }
        int t = big_int_leq(n1, n2);
        if (t)big_int_swap2(n1, n2);
        for (int i = 0; i < mx; i++) {
            if (i < n2->length) {
                if (n1->number[i] > n2->number[i]) {
                    n3->number[i] = n1->number[i] - n2->number[i] - carry;
                    carry = 0;
                }
                if (n1->number[i] == n2->number[i]) {
                    if (carry) {
                        n3->number[i] = 0xFF;
                    } else {
                        n3->number[i] = 0;
                    }
                }
                if (n1->number[i] < n2->number[i]) {
                    if (carry) {
                        n3->number[i] = 0xFF + n1->number[i] - n2->number[i];
                    } else {
                        n3->number[i] = 0x100 + n1->number[i] - n2->number[i];
                        carry = 1;
                    }
                }
            } else {
                if (carry) {
                    if (n1->number[i]) {
                        n3->number[i] = n1->number[i] - 1;
                        carry = 0;
                    } else {
                        n3->number[i] = 0xFF;
                    }
                } else {
                    n3->number[i] = n1->number[i];
                }
            }
        }
        if (t) big_int_swap2(n1, n2);
        if (n1->sign == '+') {
            if (t)n3->sign = '-';
            else n3->sign = '+';
        } else {
            if (t)n3->sign = '+';
            else n3->sign = '-';
        }
        big_int_dlz(n3);
        n1->sign = n3->sign;
        n1->length = n3->length;
        n1->number=(unsigned char*) realloc(n1->number,n3->length);
        memmove(n1->number, n3->number, n3->length);
        big_int_free(&n3);
    }
}


big_int *big_int_euclid_binary(const big_int *x, const big_int *y) {
    big_int *zero = big_int_get("0");
    int k, n = 0;
    big_int *x0 = big_int_copy(x);

    big_int *y0 = big_int_copy(y);
    x0->sign='+';
    y0->sign='+';
    big_int *a = big_int_disj(x0, y0);

    if (!big_int_equal(a, zero)) {
        while ((a->number[0] & 1) != 1) {
            big_int_bin_shft_r(a);
            big_int_bin_shft_r(x0);
            big_int_bin_shft_r(y0);
            n++;
        }
    }

    while ((!big_int_leq(x0, zero)) && (!big_int_leq(y0, zero))) {
        if ((x0->number[0] & 1) == 1) {
            while ((y0->number[0] & 1) == 0) big_int_bin_shft_r(y0);
        } else {
            while ((x0->number[0] & 1) == 0) big_int_bin_shft_r(x0);
        }
        if (big_int_leq(y0, x0)) {
            big_int_sub2(x0, y0);
        } else {
            big_int_sub2(y0, x0);
        }
    }

    big_int *n3 = big_int_add(x0, y0);
    big_int_bin_shft_l2(n3, n);
    big_int_free2(4, &x0, &y0, &a, &zero);
    return n3;
}


big_int *big_int_mul(const big_int *n1,const big_int *n2) {

    unsigned int new_length = n1->length + n2->length;

    big_int *result = (big_int *) malloc(sizeof(big_int));
    if (!result) return NULL;
    result->length = new_length;

    result->number = (unsigned char *) calloc(result->length, sizeof(unsigned char));

    for (size_t i = 0; i < n1->length; i++) {
        for (size_t j = 0; j < n2->length; j++) {
            unsigned short mult = n1->number[i] * n2->number[j];
            unsigned short carry = (result->number[i + j] + mult) >> 8;
            result->number[i + j] += mult;
            int k = 1;
            while (carry) {
                unsigned short carry2 = (result->number[i + j + k] + carry) >> 8;
                result->number[i + j + k] += carry;
                carry = carry2;
                k++;
            }
        }
    }
    result->sign = n1->sign != n2->sign ? '-' : '+';
    big_int_dlz(result);
    return result;
}


void big_int_div2(const big_int *n1, big_int *n2, big_int *res1, big_int *rmdr) {
    big_int *r = big_int_get("0");
    big_int *q = big_int_get("0");
    for (int i = (n1->length) - 1; i >= 0; i--) {
        for (int bit = 7; bit >= 0; bit--) {
            big_int_bin_shft_l(r);
            r->number[0] |= ((n1->number[i]) & (1 << bit)) != 0;
            if (big_int_leq(n2, r)) {
                r->sign = '+';
                big_int_sub2(r, n2);
                big_int_set_bit(q, i * 8 + bit, 1);
            }
        }
    }
    res1->sign = (n1->sign == n2->sign) ? '+' : '-';
    res1->length = q->length;
    res1->number = (unsigned char *) realloc(res1->number, res1->length);
    memmove(res1->number, q->number, q->length);
    rmdr->sign = '+';
    rmdr->length = r->length;
    rmdr->number = (unsigned char *) realloc(rmdr->number, rmdr->length);
    memmove(rmdr->number, r->number, r->length);
    big_int_free2(2,&r, &q);
}


void big_int_div3(const big_int *n1, big_int *n2, big_int *res1) {
    big_int *r = big_int_get("0");
    big_int_div2(n1,n2,res1,r);
    big_int_free(&r);
}

void big_int_rem(const big_int *n1, big_int *n2, big_int *rem) {
    big_int *q = big_int_get("0");
    big_int_div2(n1,n2,q,rem);
    big_int_free(&q);
}

void big_int_div2_for_pow(const big_int *n1, big_int *n2, big_int *rmdr) {
    big_int *r = big_int_get("0");
    for (int i = (n1->length) - 1; i >= 0; i--) {
        for (int bit = 7; bit >= 0; bit--) {
            big_int_bin_shft_l(r);
            r->number[0] |= ((n1->number[i]) & (1 << bit)) != 0;
            if (big_int_leq(n2, r)) {
                r->sign = '+';
                big_int_sub2(r, n2);
            }
        }
    }
    rmdr->sign = '+';
    rmdr->length = r->length;

    rmdr->number = (unsigned char *) realloc(rmdr->number, rmdr->length);
    memmove(rmdr->number, r->number, r->length);
    big_int_free(&r);
}

big_int *big_int_rl_mod_pow(const big_int *x,const big_int *n,const big_int *m) {
    big_int *ans = big_int_get("1");
    big_int *zero = big_int_get("0");
    big_int *x0 = big_int_copy(x);
    big_int *n0 = big_int_copy(n);
    big_int *m0 = big_int_copy(m);
    big_int *n4;
    big_int *xmodm;
    big_int *sq;
    while (!big_int_leq(n0, zero)) {
        if ((n0->number[0]) & 1) {
            xmodm = big_int_get("0");
            big_int_div2_for_pow(x0, m0, xmodm);
            n4 = big_int_mul(ans, xmodm);
            big_int_swap2(ans, n4);
            big_int_free(&n4);
            big_int_free(&xmodm);
        }
        sq = big_int_mul(x0, x0);
        big_int_div2_for_pow(sq, m0, x0);
        big_int_bin_shft_r(n0);//n>>=1
        big_int_free(&sq);
    }
    big_int_div2_for_pow(ans, m0, ans);
    big_int_free2(4, &zero, &x0, &m0, &n0);
    return ans;
}


big_int *big_int_rl_mod_pow2(const big_int *x,const big_int *n,const big_int *m) {
    big_int *ans = big_int_get("1");
    big_int *zero = big_int_get("0");
    big_int *x0 = big_int_copy(x);
    big_int *n0 = big_int_copy(n);
    big_int *m0 = big_int_copy(m);
    big_int *n4;
    big_int *xmodm;
    big_int *sq;
    while (!big_int_leq(n0, zero)) {
        if ((n0->number[0]) & 1) {
            xmodm = big_int_get("0");
            big_int_div2_for_pow(x0, m0, xmodm);
            n4 = big_int_karatsuba_mult2(ans, xmodm);
            big_int_swap2(ans, n4);
            big_int_free(&n4);
            big_int_free(&xmodm);
        }
        sq = big_int_karatsuba_mult2(x0, x0);
        big_int_div2_for_pow(sq, m0, x0);
        big_int_bin_shft_r(n0);//n>>=1
        big_int_free(&sq);
    }
    big_int_div2_for_pow(ans, m0, ans);
    big_int_free2(4,& zero, &x0, &m0,&n0);
    return ans;
}


big_int *big_int_lr_mod_pow(const big_int *x,const big_int *n,big_int *m) {
    big_int *n3 = big_int_get("1");
    big_int *sq;
    big_int *mul;
    big_int *mul2;
    for (int i = n->length - 1; i > -1; i--) {
        for (int j = 7; j > -1; j--) {
            sq = big_int_mul(n3, n3);
            big_int_div2_for_pow(sq, m, n3);
            big_int_free(&sq);
            if ((n->number[i]) & (1 << j)) {
                mul = big_int_get("0");
                big_int_div2_for_pow(x, m, mul);
                mul2 = big_int_mul(n3, mul);
                big_int_swap2(mul2, n3);
                big_int_free(&mul);
                big_int_free(&mul2);
            }
        }
    }
    big_int_div2_for_pow(n3, m, n3);
    return n3;
}


big_int *big_int_lr_mod_pow2(const big_int *x,const big_int *n, big_int *m) {
    big_int *n3 = big_int_get("1");
    big_int *sq;
    big_int *mul;
    big_int *mul2;
    for (int i = n->length - 1; i > -1; i--) {
        for (int j = 7; j > -1; j--) {
            big_int_div2_for_pow(n3, m, n3);
            sq = big_int_karatsuba_mult2(n3, n3);
            big_int_div2_for_pow(sq, m, n3);
            big_int_free(&sq);
            if ((n->number[i]) & (1 << j)) {
                mul = big_int_get("0");
                big_int_div2_for_pow(x, m, mul);
                mul2 = big_int_karatsuba_mult2(n3, mul);
                big_int_swap2(mul2, n3);
                big_int_free(&mul);
                big_int_free(&mul2);
            }
        }
    }
    big_int_div2_for_pow(n3, m, n3);
    return n3;
}

big_int *big_int_lr3(const big_int *x,const big_int *n) {
    big_int *n3 = big_int_get("1");
    big_int *sq;
    big_int *mul2;
    for (int i = n->length - 1; i > -1; i--) {
        for (int j = 7; j > -1; j--) {

            sq = big_int_karatsuba_mult2(n3, n3);
            big_int_swap2(sq,n3);//n3=sq
            big_int_free(&sq);
            if ((n->number[i]) & (1 << j)) {
                mul2 = big_int_karatsuba_mult2(n3, x);//mul2=n3*mul
                big_int_swap2(mul2, n3);//n3=mul2
                big_int_free(&mul2);
            }
        }
    }
//    big_int_div2_for_pow(n3, m, n3);
    return n3;
}

big_int *big_int_slice(const big_int *n1, long l1, long l2) {
    big_int *n = (big_int *) malloc(sizeof(big_int));
    n->sign = n1->sign;
    n->length = (unsigned int) (l2 - l1 + 1);
    if (l2 >= n1->length) {
        n->length = (unsigned int) (n1->length - l1);
    }
    if (l1 >= n1->length) {
        n->length = 1;
        n->number = (unsigned char *)  calloc(1, sizeof(n1->number[0]));
        return n;
    }
    n->number =(unsigned char *)  calloc(n->length, sizeof(n1->number[0]));
    memcpy(n->number, n1->number + l1, n->length);
    big_int_dlz(n);
    return n;
}


big_int *big_int_karatsuba_mult2(const big_int *n1,const big_int *n2) {
    if (n1->length + n2->length <= const1) { return big_int_mul(n1, n2); }
    else {
        unsigned int mx = (n1->length >= n2->length) ? n1->length : n2->length;
        mx += (mx & 1);

        big_int *q = big_int_slice(n1, 0, mx / 2 - 1);
        big_int *p = big_int_slice(n1, mx / 2, mx - 1);
        big_int *s = big_int_slice(n2, 0, mx / 2 - 1);
        big_int *r = big_int_slice(n2, mx / 2, mx - 1);

        big_int *pr = big_int_karatsuba_mult2(p, r);//A1
        big_int *qs = big_int_karatsuba_mult2(q, s);//A2

        big_int *sm1 = big_int_add(p, q);
        big_int *sm2 = big_int_add(r, s);

        big_int *a3 = big_int_karatsuba_mult2(sm1, sm2);//A3=(p+q)(r+s)
        big_int *sm3 = big_int_add(pr, qs);

        big_int_sub2(a3, sm3);//A3=(p+q)(r+s)-pr-qs=ps+qr

        big_int_bin_shft_l2(pr, mx << 3);//A1<<n
        big_int_bin_shft_l2(a3, mx << 2);//A3=(A3-(A1+A2))<<n/2


        big_int *res = big_int_add(pr, a3);
//        big_int_print(res);
//        printf("228\n");
        big_int_add2(res, qs);//A1<<n + (A3-(A1+A2))<<n/2 + A2
//        big_int_print(q);big_int_print(p);
//        big_int_print(r);big_int_print(s);
//        big_int_print(pr);big_int_print(qs);
//        big_int_print(a3);big_int_print(sm1);
//        big_int_print(sm2);big_int_print(sm3);
//        big_int_print(res);
//        big_int_print2(11, &q, &p, &r, &s, &pr, &qs, &a3, &sm1, &sm2, &sm3,&res);
//        printf("---------------------\n");


        big_int_free2(10, &q, &p, &r, &s, &pr, &qs, &a3, &sm1, &sm2, &sm3);
        return res;
    }
}


big_int *big_int_rnd(unsigned int n) {
    big_int *res = (big_int *) malloc(sizeof(big_int));
    res->length = n;
    res->sign = '+';
    res->number = (unsigned char *) calloc(n, sizeof(unsigned char));
    for (long i = 0; i < n; i++) {
        res->number[i] = rand();
    }
    if (!((res->number[0]) & 1)) {
        res->number[0]++;
    }
    if (!(res->number[(res->length) - 1])) {
        res->number[(res->length) - 1]++;
    }

    return res;
}

void big_int_free(big_int **n) {
    free((*n)->number);
    free(*n);
    *n=NULL;
}

void big_int_free2(const unsigned int n0, ...) {
    va_list ptr;
    va_start(ptr, n0);
    for (int j = 0; j < n0; j++) {
        big_int **n = va_arg(ptr, big_int ** );
        if((*n)){
            big_int_free(n);}
    }
    va_end(ptr);
}

int big_int_primality_test(big_int *n, unsigned int tst_cnt) {
    long cnt_of_two = 0;
    int fl = 0;
    big_int *one = big_int_get("1");
    big_int *two = big_int_get("10");
    if(big_int_equal(one,n)){return 0;}
    if(big_int_equal(two,n)){return 1;}

    big_int *r2 = big_int_sub(n, one);
    big_int *d = big_int_copy(r2);

    while ((d->number[0] & 1) != 1) {
        big_int_bin_shft_r(d);
        cnt_of_two++;
    }


    big_int *r = big_int_sub(n, two);
    big_int *a=big_int_get("0");
    big_int *x= big_int_get("0");
    big_int *y= big_int_get("0");

    for (unsigned int i = 1; i < tst_cnt + 1; i++) {

        if (n->length != 1) { big_int_free2(1,&a); a = big_int_rnd(1 + rand() % ((n->length) - 1)); }//[0;len-1]
        else { big_int_free2(1,&a); a = big_int_rnd(1);a->number[0]=(2+rand())%((n->number[0])-2); }

        if (big_int_leq(a, one)) {
            big_int_add2(a, two);
        }


        big_int_free2(1,&x);
        x = big_int_lr_mod_pow2(a, d, n);

        if (big_int_equal(x, one) || big_int_equal(x, r2)) { continue; }

        for (long i = 1; i < cnt_of_two + 1; i++) {

            big_int_free2(1,&y);
            y = big_int_karatsuba_mult2(x, x);
            big_int_div2_for_pow(y, n, y);

            if ((big_int_equal(y, one)) && (!big_int_equal(x, one)) && (!big_int_equal(x, r2))) {
                big_int_free2(8, &one, &r, &r2, &x, &a, &y, &two, &d);
                return 0;
            }

            big_int_swap(x, y);
        }

        if (!big_int_equal(x, one)) {
            big_int_free2(8, &one, &r, &r2, &a, &x, &two, &d,&y);
            return 0;
        }

        big_int_free2(2, &x, &a);
    }
    big_int_free2(8,&a,&one, &r, &r2, &two, &d,&y,&x);
    return 1;
}


big_int *big_int_get_prime(unsigned int len, unsigned int tst_cnt) {
    int prime = 0;
    big_int *res;
    //first 100 primes
    while (!prime) {
        res = big_int_rnd(len);
//        printf("%li\n", clock()/CLOCKS_PER_SEC );
        prime = big_int_primality_test(res, tst_cnt);
        if (prime)return res;
        big_int_free(&res);
    }
}

big_int *big_int_get_prime2(unsigned int len, unsigned int tst_cnt) {
    int prime = 0;
    big_int *res = (big_int *) malloc(sizeof(big_int));
    res->length = len;
    res->sign = '+';
    big_int *two = big_int_get("10");
    res->number = (unsigned char *) calloc(res->length, sizeof(unsigned char));
    res->number[len - 1] = 128;
    res->number[0] = 1;
    while ((!prime)&&(res->length!=len+1)) {
        printf("%li\n", clock() / 1000000);
        prime = big_int_primality_test(res, tst_cnt);
        if (prime)return res;
        big_int_add2(res, two);
    }
}

big_int *big_int_get_prime3(unsigned int len, unsigned int tst_cnt) {
    int prime = 0;

    big_int *two = big_int_get("10");
    while(1==1) {
        big_int *res = big_int_rnd(len);
        prime = big_int_primality_test(res, tst_cnt);
        while ((!prime) && (res->length != len + 1)) {
            printf("%li\n", clock() / 1000000);
            prime = big_int_primality_test(res, tst_cnt);
            if (prime)return res;
            big_int_add2(res, two);
        }
        if (prime)return res;
    }
}

void big_int_test_loop(long long n, int (*func)(big_int *, unsigned int)) {
    int start = clock();
    big_int *num1 = big_int_rnd(100);
    for (long long i = 0; i < n; i++) {
        func(num1, 20);    big_int *sq;

//        big_int_free(t);
    }
    int end = clock();
    printf("%f\n", (double) (end - start) / CLOCKS_PER_SEC);
}


big_int *big_int_mul_inverse(const big_int *n1, big_int *mod) {

    big_int *one= big_int_get("1");
    big_int *m_one= big_int_get("-1");
    big_int *gcd=big_int_euclid_binary(n1,mod);
    if(!big_int_equal(gcd,one)){ big_int_free2(2,&gcd,&one); return m_one; }
    big_int *x = big_int_get("1");
    big_int *y = big_int_get("0");
    big_int *x_next = big_int_get("0");
    big_int *y_next = big_int_get("1");
    big_int *a = big_int_copy(n1);
    big_int *b = big_int_copy(mod);
    big_int *zero = big_int_get("0");
    big_int *q = big_int_get("0");

    while (!big_int_leq(b, zero)) {

        big_int_div3(a, b, q);//q=a/b

        big_int *tmp1 = big_int_copy(b);//tmp=b

        big_int_div2_for_pow(a, b, b);//b=a%b

        big_int_swap(a, tmp1);//a=tmp
        big_int_free2(1,&tmp1);

        tmp1 = big_int_copy(x_next);//tmp=x_next

        big_int *q_times_x_next = big_int_karatsuba_mult2(q, x_next);//x_next=x-q*x_next //x*=y
        big_int_free(&x_next);
        x_next = big_int_sub(x, q_times_x_next);

        big_int_swap(x, tmp1);//x=tmp1
        big_int_free2(2,&q_times_x_next,&tmp1);

        tmp1 = big_int_copy(y_next);//tmp2=y_next

        big_int *q_times_y_next = big_int_karatsuba_mult2(q, y_next);//y_next=y-q*y_next
        big_int_free(&y_next);
        y_next = big_int_sub(y, q_times_y_next);

        big_int_swap(tmp1, y);//y=tmp2
        big_int_free2(2,&q_times_y_next,&tmp1);
    }
    while(x->sign=='-'){
        big_int_add2(x,mod);
    }

    big_int_free2(10,&y,&x_next,&y_next,&a,&b,&zero,&q,&one,&m_one,&gcd);
    return x;

}












