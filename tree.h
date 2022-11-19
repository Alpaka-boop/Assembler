#ifndef TREEE_H
#define TREEE_H

#include <stdlib.h>

struct Tree {
    char *str;
    int str_lenth;
    unsigned char height;
    size_t cmd_code;
    Tree *left;
    Tree *right;
};

int height (Tree* tree);
int balance_fact (Tree *tree);
Tree* rotate_right (Tree **tree);
Tree* rotate_left (Tree **tree);
void balance (Tree **tree);
int comparator (const char *l1, const char *l2);
void insert (Tree **tree, const char *str, const size_t lenth, const size_t cmd_code);
void tree_dtor (Tree **tree);
const Tree *exist (const Tree *tree, char *str);
void tree_ctor (Tree **tree);

#endif
